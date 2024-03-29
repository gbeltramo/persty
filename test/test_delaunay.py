import numpy as np
import persty.delaunay

np.random.seed(0)
points2 = np.random.rand(50, 2)

result1 = [
    (0, 2),
    (0, 6),
    (0, 9),
    (0, 11),
    (0, 16),
    (0, 18),
    (0, 22),
    (1, 5),
    (1, 16),
    (1, 18),
    (1, 25),
    (2, 11),
    (2, 16),
    (2, 20),
    (2, 45),
    (3, 6),
    (3, 11),
    (3, 13),
    (3, 15),
    (4, 5),
    (4, 26),
    (4, 28),
    (4, 31),
    (4, 33),
    (4, 35),
    (5, 18),
    (5, 19),
    (5, 22),
    (5, 25),
    (5, 28),
    (5, 35),
    (5, 36),
    (6, 9),
    (6, 11),
    (6, 13),
    (6, 44),
    (7, 17),
    (7, 30),
    (7, 43),
    (7, 48),
    (8, 13),
    (8, 17),
    (8, 41),
    (8, 46),
    (9, 10),
    (9, 19),
    (9, 22),
    (9, 44),
    (10, 19),
    (10, 26),
    (10, 36),
    (11, 15),
    (11, 45),
    (12, 17),
    (12, 20),
    (12, 32),
    (12, 40),
    (12, 41),
    (12, 45),
    (12, 46),
    (13, 15),
    (13, 44),
    (13, 46),
    (14, 16),
    (14, 20),
    (14, 25),
    (14, 29),
    (14, 42),
    (15, 45),
    (15, 46),
    (16, 18),
    (16, 20),
    (16, 25),
    (17, 30),
    (17, 32),
    (17, 41),
    (18, 22),
    (19, 22),
    (19, 36),
    (20, 29),
    (20, 40),
    (20, 45),
    (21, 31),
    (21, 33),
    (21, 37),
    (21, 48),
    (23, 27),
    (23, 30),
    (23, 38),
    (23, 47),
    (24, 29),
    (24, 32),
    (24, 40),
    (24, 47),
    (25, 28),
    (25, 42),
    (26, 33),
    (26, 34),
    (26, 35),
    (26, 36),
    (27, 30),
    (27, 32),
    (27, 47),
    (28, 31),
    (28, 42),
    (28, 43),
    (29, 39),
    (29, 40),
    (29, 42),
    (29, 43),
    (29, 47),
    (30, 32),
    (30, 38),
    (30, 39),
    (30, 43),
    (31, 33),
    (31, 43),
    (31, 48),
    (32, 40),
    (32, 47),
    (33, 34),
    (33, 37),
    (33, 49),
    (34, 49),
    (35, 36),
    (37, 48),
    (37, 49),
    (38, 39),
    (38, 47),
    (39, 43),
    (39, 47),
    (41, 46),
    (42, 43),
    (43, 48),
    (45, 46),
    (48, 49),
]


def test_edges1():
    assert persty.delaunay.edges(points2) == result1, "delauany 2D points wrong"
