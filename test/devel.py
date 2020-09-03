import numpy as np
import persty.cpp.minibox
import persty.minibox
from time import time, sleep
print("="*60)
print("dir(persty.minibox):", dir(persty.minibox))

# input
np.random.seed(0)
N, d = 100, 3
points = np.random.rand(N, d)
print("Points array:", points[:3], " ... ")
print("n*(n-1)/2 =", N*(N-1)//2)
#
# correctness sanity check
print("="*10)
ed1 = persty.minibox.edges_n_dim(points)
ed2 = persty.minibox.test_edges(points)
print("edges C++:", ed1[:3], "len(edges) -->", len(ed1))
print("edges Py:",  ed2[:3], "len(edges) -->", len(ed2))
print("Equal?", ed1 == ed2)

# test utils
# p = np.array([0, 0, 0])
# q = np.array([1, -1, 1])
# z = np.array([0.5, -0.5, 0.5])
# mini = persty.cpp.minibox.get_minibox(p, q)
# print("minibox", mini)
# print("True?", persty.cpp.minibox.is_inside(z, mini))

# timing
N, d = 5_000, 3
points2 = np.random.rand(N, d)
s1 = time()
_ = persty.minibox.edges_n_dim(points2)
print(f"STL vector time: {time() - s1} sec.")
print("-"*60)
