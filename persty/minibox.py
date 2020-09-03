import persty.cpp.minibox as _cpp
import numpy as np

def test_edges(points):
    edges = list()
    for i, p in enumerate(points):
        for j in range(i+1, len(points)):
            q = points[j]
            mini = [[min(a, b), max(a,b)] for a, b in zip(p,q)]
            add_edge = True
            for z in points:
                cond = all([ (a < z[k]) and (z[k] < b) for k, (a, b) in enumerate(mini)])
                if cond:
                    add_edge = False
                    break
            if add_edge:
                edges.append((i,j))
    return edges

def edges_n_dim(points):
    """
    Minibox edges of d-dimensional points.

    Parameters
    ----------
    points: (n,d) numpy array

    Return
    ------
    edges: list of tuples
        Pairs of indices of `points` representing
        their Minibox edges.
    """

    return _cpp.edges(points)
