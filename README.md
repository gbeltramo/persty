# `persty` - Minibox and Delaunay Edges Algorithms

This package provides an implementation of algorithms for finding the
*Minibox* and *Delaunay* edges on a finite set of points in d-dimensional
space with maximum distance.

## Installation

This package requires `setuptools` and `numpy`.

Install it with

```
pip install git+https://github.com/gbeltramo/persty.git
```

## Usage

```python
import numpy as np
import persty.minibox
import persty.delaunay

np.random.seed(0)
points = np.random.rand(20, 2).tolist()

minibox_edges = persty.minibox.edges(points)
delaunay_edges = persty.delaunay.edges(points)
```
