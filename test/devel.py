import os
import numpy as np
import psutil
from time import time, sleep
from itertools import combinations

import persty.cpp.binding as _cpp
import persty.util
import persty.minibox
import persty.delaunay
from gudhi import RipsComplex, SimplexTree

from scipy.spatial.distance import chebyshev
from sklearn.metrics import pairwise_distances

print("=" * 60)

# input
np.random.seed(0)
N, d = 100, 3
points = np.random.rand(N, d)
# print("n*(n-1)/2 =", N*(N-1)//2)
#
# correctness sanity check
print("=" * 10)
# print("SANITY CHECK MINIBOX EDGES")
# sorted_indices = np.argsort(points[:,0])
# points = points[sorted_indices]
#
# ed1 = persty.minibox.edges(points)
# ed2 = persty.minibox.edges_n_dim(points)
# print(f"shape input points is {points.shape}")
# print("edges:", ed1[:3], "len(edges) -->", len(ed1))
# print("edges n-D:", ed2[:3], "len(edges) -->", len(ed2))
# print("minibox edges equal?", ed1 == ed2)

# # test utils
# print("="*10)
# print("TEST UTIL")
# p = np.array([0, 0, 0])
# q = np.array([1.5, -0.9, 0.3])
# z = np.array([0.5, -0.6, 0.1])
# mini = _cpp.get_minibox(p, q)
# print("minibox:", mini)
# print("z is inside minibox?", _cpp.is_inside(z, mini))
# print()
# print("k side, radius 1:", _cpp.get_k_long_side_radius(p, q))
# print("k side, radius 2:", _cpp.get_k_long_side_radius(p, z))
# print("A_bar1:", _cpp.get_A_r(p, q))
# print("A_bar2:", _cpp.get_A_r(p, z))

# timing
# print("="*10)
# N = 100
# print(f"TIMINGS --> n = {N} points")
#
# np.random.seed(0)
# points2 = np.random.rand(N, 2)
# points3 = np.random.rand(N, 3)
# points10 = np.random.rand(N, 6)
#
# s1 = time()
# ed1 = persty.minibox.edges(points2)
# print(f"-----\nTime minibox 2-D: {time() - s1:.5f} sec.")
#
# s2 = time()
# ed2 = persty.minibox.edges(points3)
# print(f"-----\nTime minibox 3-D: {time() - s2:.5f} sec.")
#
# s3 = time()
# ed3 = persty.minibox.edges(points10)
# print(f"-----\nTime minibox 6-D: {time() - s3:.5f} sec.")
#
# s5 = time()
# ed5 =  persty.delaunay.edges(points2)
# print(f"-----\nTime delaunay 2-D : {time() - s5:.5f} sec.")


# timing minibox high-dim
print("=" * 10)
N = 1000
d = 5
print(f"TIMINGS --> n = {N} points, dim = {d}")

np.random.seed(0)
points = np.random.rand(N, d)

s = time()
ed = persty.minibox.edges(points)
print(f"-----\nTime minibox {d}-D: {time() - s:.5f} sec.")
triangles = _cpp.get_clique_triangles(ed)
tot = len(points) + len(ed) + len(triangles)
print(f"There are {len(ed)} Minibox edges --> {N*(N-1)//2} is max number of edges")
print(
    f"There are {len(triangles)} Minibox triangles --> {N*(N-1)*(N-2)//3} is max number of triangles"
)
print(f"Ratio simplices = {tot / (N + N*(N-1)//2 + N*(N-1)*(N-2)//3):.5f}")

print()


process = psutil.Process(os.getpid())
memory_start = process.memory_info().rss // 1048576
# s = time()
# tri, param = persty.util.clique_triangles(points, ed)
# print(f"-----\nTime find triangles {d}-D: {time() - s:.5f} sec.")

## MINIBOX
# print("="*20)
# process = psutil.Process(os.getpid())
# mmb1 = process.memory_info().rss // 1048576
# s = time()
# st = persty.util.make_gudhi_simplex_tree(points, ed)
# print(f"Gudhi Minibox build time: {time() - s:.5f} sec.")
# process = psutil.Process(os.getpid())
# mmb2 = process.memory_info().rss // 1048576
# print(f"Memory used Minibox: {mmb2 - mmb1} Mb.")
#
# process = psutil.Process(os.getpid())
# mmb1 = process.memory_info().rss // 1048576
# s = time()
# dgm1 = st.persistence()
# print(f"Gudhi Minibox time: {time() - s:.5f} sec.")
# process = psutil.Process(os.getpid())
# mmb2 = process.memory_info().rss // 1048576
# print(f"Memory used to compute Minibox: {mmb2 - mmb1} Mb.")

## RIPS
print("=" * 20)
# process = psutil.Process(os.getpid())
# mmb1 = process.memory_info().rss // 1048576
# s = time()
# rips = RipsComplex(distance_matrix=pairwise_distances(points, metric="chebyshev"),
#                    max_edge_length=2.0)
# st2 = rips.create_simplex_tree(2)
# print(f"Gudhi Rips build time: {time() - s:.5f} sec.")
# process = psutil.Process(os.getpid())
# mmb2 = process.memory_info().rss // 1048576
# print(f"Memory used: {mmb2 - mmb1} Mb.")
#
# process = psutil.Process(os.getpid())
# mmb1 = process.memory_info().rss // 1048576
# s = time()
# dgm2 = st2.persistence()
# print(f"Gudhi Rips time: {time() - s:.5f} sec.")
# process = psutil.Process(os.getpid())
# mmb2 = process.memory_info().rss // 1048576
# print(f"Memory used to compute Rips: {mmb2 - mmb1} Mb.")

## CECH
# print("="*20)
# process = psutil.Process(os.getpid())
# mmb1 = process.memory_info().rss // 1048576
# s = time()
# cech_tree = SimplexTree()
# vertices = list(range(len(points)))
# for v in vertices:
#     cech_tree.insert(simplex=[v], filtration=0.0)
# for e in list(combinations(range(len(points)), 2)):
#     p, q = points[e[0]], points[e[1]]
#     cech_tree.insert(simplex=e, filtration=chebyshev(p, q))
# cech_tree.expansion(2)
# print(f"Gudhi Cech build time: {time() - s:.5f} sec.")
# process = psutil.Process(os.getpid())
# mmb2 = process.memory_info().rss // 1048576
# print(f"Memory used Cech: {mmb2 - mmb1} Mb.")
#
# process = psutil.Process(os.getpid())
# mmb1 = process.memory_info().rss // 1048576
# s = time()
# dgm3 = cech_tree.persistence()
# print(f"Gudhi Cech time: {time() - s:.5f} sec.")
# mmb2 = process.memory_info().rss // 1048576
# print(f"Memory used to compute Cech: {mmb2 - mmb1} Mb.")

# final check
print("=" * 20)
# print("Equal Minibox and Rips?", dgm1 == dgm2)
# print("Equal Minibox and Cech?", dgm1 == dgm2)
# print("Equal Cech and Rips?", dgm3 == dgm2)

process = psutil.Process(os.getpid())
memory_end = process.memory_info().rss // 1048576
print(f"Total memory in use: {memory_end - memory_start}")
