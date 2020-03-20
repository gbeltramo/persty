from itertools import combinations

def clique_simplices(edges, number_points, dimension=2):
    """Return the clique simplices on `edges`

    Parameters
    ----------
    edges: list pairs of indices, list of list of int
        The edges of a graph built on some finite set of points
    number_points: int
        The number of vertices of the graph to which the edges belong
    dimension: int >=2
        Dimension of clique simplices returned

    Return
    ------
    clique_simplices: list of list of int
        The cliques on the given edges

    """
    edges = set(edges)
    all_simplices = list(combinations(range(number_points), dimension+1))
    edges_in_simplex = list(combinations(range(dimension+1), 2))
    simplices = []
    not_found = False

    for sim in all_simplices:
        not_found = False
        for e in edges_in_simplex:
            v1, v2 = sim[e[0]], sim[e[1]]
            if (v1, v2) not in edges:
                not_found = True
                break
        if not not_found:
            simplices.append(sim)
    return simplices
