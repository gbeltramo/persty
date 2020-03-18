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
import persty.minibox

points = [[1., 1.],
          [2., 3.],
          [4., -1.],
          [-3., 8.],
          [0., 2.3]]

mini_edges = persty.minibox.edges(points)
```
