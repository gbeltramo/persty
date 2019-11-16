# `persty` - minibox and Delauany graphs

This package provides an implementation of the algorithm for
computing the *minibox* and *Delaunay* graphs of a finite set of
points in d-dimensional space with Chebyshev diastance.

## Example usage

The following example generates 100 points in 3 dimensional space,
and uses `persty` to compute the edges belonging to the minibox and
Delaunay graphs on these points.

```python
import numpy as np
import persty.graph as pg
dimension = 3
points = np.random.rand(100, dimension)
minibox_edges = pg.minibox_graph(points)
delaunay_edges = pg.delaunay_graph(points)
```
