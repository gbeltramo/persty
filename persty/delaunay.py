import persty.minibox
import persty.util
from scipy.spatial.distance import chebyshev

def edges(points):
    """Delaunay edges on d-dimensional points

    Find the Delaunay edges iterating on all possible
    pairs of indices in `points`.

    Parameters
    ----------
    points: list of `n` lists containing `d` floats each
        The list of d-dimensional points.

    Return
    ------
    delaunay_edges: list of pairs of integers
        The indices of elements in `points` forming a
        Delaunay edge.
    """
    print("="*30)
    minibox_edges = persty.minibox.edges(points)
    print("Minibox edges found are:\n", minibox_edges)
    delaunay_edges = []

    for i, j in minibox_edges:
        print(f"\n---\nMini edge {i , j}")
        p = points[i]
        q = points[j]
        r = chebyshev(p, q) / 2.0
        print(f"p={p}, {type(p)}")
        A = persty.util.get_A([p, q])
        hyperrect = [A]
        print("A and hyper found")
        for index in range(len(points)):
            if index == i or index == j:
                continue
            y = points[index]
            hyperrect = persty.util.update_list_hyperrectangles([hyperrect, y, r])
            if len(hyperrect) == 0:
                break

        if len(hyperrect) != 0:
            print("appending edge")
            delaunay_edges.append([i,j])

    return delaunay_edges
