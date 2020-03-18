import persty.minibox

pts5 = [[1., 1.],
        [2., 3.],
        [4., -1.],
        [-3., 8.],
        [0., 2.3]]

result5 = [[0, 1], [0, 2], [0, 4],
           [1, 2], [1, 3], [1, 4], [3, 4]]

def test_edges_five_points():
    assert persty.minibox.edges(pts5) == result5
