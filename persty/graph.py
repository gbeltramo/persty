import numpy as _np
from itertools import combinations as _combinations

#=================================================

def find_delta(p, q):
    deltas = _np.abs(p - q)
    K = _np.argmax(deltas)
    return K, deltas[K]

#=================================================

def points_in_rect_along_k(rect, sorted_points, k=0):
    """
    Find range of indices in sorted ndarray falling inside the
    rectangle in the given direction
    """
    bottom, top = rect[k]
    index_bottom = _np.searchsorted(sorted_points[:,k],
                                    bottom, side='right')
    index_top = _np.searchsorted(sorted_points[:,k],
                                 top, side='left')
    return index_bottom, index_top

#=================================================

def indices_in_box(rect, points, sorted_indices):
    """Indices of `points` contained in `rect`"""
    dimension = len(points[0])
    indices_in = _np.arange(len(points))

    for k in range(dimension):
        sorted_indices_k = sorted_indices[k]
        ind_bottom, ind_top = points_in_rect_along_k(rect,
                                                     points[sorted_indices_k],
                                                     k)
        indices_in = _np.intersect1d(indices_in,
                                     sorted_indices_k[ind_bottom:ind_top])
        if len(indices_in) == 0:
            return indices_in
    return indices_in

#=================================================

def minibox_graph(points):
    """
    Minibox graph of points in d-dimensional space with 
    Chebyshev distance.

    Parameters
    ----------
    points: np.array of shape (n, d)
        vertices of the minibox graph

    Return
    ------
    minibox_edges: list of two-tuples
        sorted pairs of points, representing the edges of the 
        minibox graph
    """
    minibox_edges = []

    dimension = len(points[0])
    sorted_indices = [_np.argsort(points[:,i])
                      for i in range(dimension)]
    all_edges = _combinations(range(len(points)), 2)
    
    for i, j in all_edges:
        p, q = points[i], points[j]
        minibox = _np.array([(min(p[i], q[i]), max(p[i], q[i]))
                             for i in range(len(p))])
        indices_in_minibox = indices_in_box(minibox,
                                            points,
                                            sorted_indices)
        if len(indices_in_minibox) == 0:
            minibox_edges.append((i,j))

    return minibox_edges

#=================================================

def check_hyperrectangle_sides(hyperrectangle, delta):
    sides = _np.abs(hyperrectangle[:,0] - hyperrectangle[:,1])
    return (sides >= delta).all()

#=================================================

def remove_i_element(points, i):
    new = _np.copy(points)
    mask = _np.ones(len(points), dtype=bool)
    mask[i] = False
    return new[mask]

#=================================================

def resize_hyperrectangle(hyperrectangle, proj_minibox, z, k):
    recursion = False
    bottom_k, top_k = hyperrectangle[k]
    mini_bottom_k, mini_top_k = proj_minibox[k]
    new_hyperrectangle = _np.copy(hyperrectangle)
    
    # slide in direction k if z[k] outside minibox in d-1 dimensions
    if z[k] <= mini_bottom_k:
        recursion = True
        new_hyperrectangle[k][0] = max(z[k], bottom_k)
        new_hyperrectangle[k][1] = top_k
    elif mini_top_k <= z[k]:
        recursion = True
        new_hyperrectangle[k][0] = bottom_k
        new_hyperrectangle[k][1] = min(z[k], top_k)
    
    return recursion, new_hyperrectangle

#=================================================

def exists_empty_hypercube(hyperrectangle, delta, 
                           proj_z_points, proj_minibox):
    """z_points and hyperrectangle are in d-1 dimensions"""

    sides_large_enough = check_hyperrectangle_sides(hyperrectangle,
                                                    delta)
    
    if not sides_large_enough:
        return False
    elif len(proj_z_points) == 0 and sides_large_enough:
        return True
    else:
        dimension = len(proj_z_points[0])
            
        for i, z in enumerate(proj_z_points):
            new_proj_z_points = remove_i_element(proj_z_points, i)
            for k in range(dimension):
                recursion, new_hyperrectangle = resize_hyperrectangle(hyperrectangle, proj_minibox, z, k)
                
                if recursion:
                    return exists_empty_hypercube(new_hyperrectangle,
                                                  delta, 
                                                  new_proj_z_points, 
                                                  proj_minibox)

#=================================================

def delaunay_graph(points):
    """
    Delaunay graph of points in d dimensional space with Chebyshev distance
    """
    
    delaunay_edges = []

    dimension = len(points[0])
    sorted_indices = [_np.argsort(points[:,i])
                      for i in range(dimension)]
    all_edges = _combinations(range(len(points)), 2)
    
    for i, j in all_edges:
        p, q = points[i], points[j]
        minibox = _np.array([(min(p[i], q[i]), max(p[i], q[i]))
                             for i in range(len(p))])
        indices_in_minibox = indices_in_box(minibox,
                                            points,
                                            sorted_indices)
        if len(indices_in_minibox) == 0:
            K_delta, delta = find_delta(p, q)
            proj_minibox = remove_i_element(minibox, K_delta)
            maxibox = _np.array([(max(p[i], q[i]) - delta,
                                 min(p[i], q[i]) + delta) 
                                for i in range(dimension)])
            indices_z_points = indices_in_box(maxibox, points, sorted_indices)
            indices_z_points = indices_z_points[indices_z_points!=i]
            indices_z_points = indices_z_points[indices_z_points!=j]
            search_space = remove_i_element(maxibox, K_delta)
            proj_z_points = _np.delete(points[indices_z_points], K_delta, axis=1)
            
            if exists_empty_hypercube(search_space, delta,
                                      proj_z_points, proj_minibox):
                delaunay_edges.append((i,j))
                
    return delaunay_edges

#=================================================

def delaunay_simplices(delaunay_edges, n, dim=3):
    delaunay_simplices = []
    all_simplices = _combinations(range(n), dim)
    
    edges_in_simplex = list(_combinations(range(dim), 2))
    for k, sim in enumerate(all_simplices):
        sim_k_in = True
        for i, j in edges_in_simplex:
            if (sim[i], sim[j]) not in delaunay_edges:
                sim_k_in = False
                break
        if sim_k_in:
            delaunay_simplices.append(sim)
            
    return delaunay_simplices

#=================================================
