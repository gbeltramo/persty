import numpy as np
import persty.cpp.binding as _cpp

def edges(points):
    assert type(points) == np.ndarray, "points must be nd.array"
    assert len(points.shape) == 2, "points must have shape (n,d), with n number points and d their dimension"
    dimension = len(points[0])
    if dimension == 2:
        sorted_indices = np.argsort(points[:,0])
        points = points[sorted_indices]
        res = _cpp.delaunay_edges_2D(points)
        return [tuple(sorted((sorted_indices[i], sorted_indices[j]))) for i, j in res]
    else:
        print("only 2D array are valid")
        return None
