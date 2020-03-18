import numpy as _np
from itertools import combinations as _combinations
import persty.minibox
import persty.util

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

def exists_empty_hypercube(search_space, delta,
                           proj_z_points, proj_minibox):
    """z_points and hyperrectangle are in d-1 dimensions"""

    sides_large_enough = check_hyperrectangle_sides(search_space, delta)

    if not sides_large_enough:
        return False
    elif len(proj_z_points) == 0 and sides_large_enough:
        return True
    else:
        # remove first point from those in search_space
        # and use it to split the search space recursively
        z = proj_z_points[0]
        new_proj_z_points = proj_z_points[1:]
        # slide if z_k outside proj_minibox and within search_space
        for k in range(len(z)):
            bottom_k, top_k = search_space[k]
            mini_bottom_k, mini_top_k = proj_minibox[k]
            new_search_space = _np.copy(search_space)
            if (bottom_k < z[k] <= mini_bottom_k):
                new_search_space[k][0] = z[k]
                if exists_empty_hypercube(new_search_space, delta,
                                          new_proj_z_points, proj_minibox):
                    return True
            elif (mini_top_k <= z[k] < top_k):
                new_search_space[k][1] = z[k]
                if exists_empty_hypercube(new_search_space, delta,
                                          new_proj_z_points, proj_minibox):
                    return True
            elif (z[k] <= bottom_k) or (top_k <= z[k]):
                if exists_empty_hypercube(new_search_space, delta,
                                          new_proj_z_points, proj_minibox):
                    return True
        return False

#=================================================

def delaunay_edges(points):
    """
    Delaunay graph of points in d dimensional space with Chebyshev
    distance.
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
        # if minibox of p,q empty, check with recursion
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


def flag_simplices(edges, n, dim=3):
    simplices = []
    all_simplices = _combinations(range(n), dim)

    edges_in_simplex = list(_combinations(range(dim), 2))
    for k, sim in enumerate(all_simplices):
        sim_k_in = True
        for i, j in edges_in_simplex:
            if (sim[i], sim[j]) not in edges:
                sim_k_in = False
                break
        if sim_k_in:
            simplices.append(sim)

    return simplices

#=================================================
