import numpy as np
import persty.cpp.binding as _cpp
import delaunaynd.delaunay as dd
import persty.minibox
import persty.delaunay
from time import time, sleep
print("="*60)

# input
np.random.seed(0)
N, d = 100, 3
points = np.random.rand(N, d)
print("n*(n-1)/2 =", N*(N-1)//2)
#
# correctness sanity check
print("="*10)
ed1 = persty.minibox.edges(points)
ed2 = persty.minibox.test_edges(points)
print("edges C++:", ed1[:3], "len(edges) -->", len(ed1))
print("edges Py:",  ed2[:3], "len(edges) -->", len(ed2))
print("Equal?", ed1 == ed2)

# test utils
p = np.array([0, 0, 0])
q = np.array([1, -1, 1])
z = np.array([0.5, -0.5, 0.5])
mini = _cpp.get_minibox(p, q)
print("minibox", mini)
print("True?", _cpp.is_inside(z, mini))
print("-"*10)


# timing
N = 1000
np.random.seed(0)

points2 = np.random.rand(N, 2)
points3 = np.random.rand(N, 3)
points10 = np.random.rand(N, 6)

s1 = time()
ed1 = persty.minibox.edges(points2)
print(f"Time minibox 2-D: {time() - s1:.5f} sec.")
print("-----\n")

s2 = time()
ed2 = persty.minibox.edges(points3)
print(f"Time minibox 3-D: {time() - s2:.5f} sec.")
print("-----\n")

s3 = time()
ed3 = persty.minibox.edges(points10)
print(f"Time minibox 6-D: {time() - s3:.5f} sec.")
print("-----\n")

s4 = time()
ed4 = dd.edges(points2.tolist())
print(f"Time delaunay 2-D (C): {time() - s4:.5f} sec.")
print("-----\n")

s5 = time()
ed5 =  persty.delaunay.edges(points2)
print(f"Time delaunay 2-D (C++): {time() - s4:.5f} sec.")
print("-----\n")

print("del_ed_2D_C == del_ed_2D_C++?", ed4 == ed5)
print()
print("mini edges:", ed1)
print()
print("correct edges:", ed4)
print()
print("wrong edges:", ed5)
