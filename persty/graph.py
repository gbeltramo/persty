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

def indices_in_box(rect, points, i, j, sorted_indices):
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

def minibox_graph(points, print_info=False):
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
                                            points, i, j,
                                            sorted_indices)
        if len(indices_in_minibox) == 0:
            minibox_edges.append((i,j))

    return minibox_edges