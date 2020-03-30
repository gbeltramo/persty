# `persty` - Minibox and Delaunay Edges Algorithms

This package provides an implementation of algorithms for finding the
*Minibox* and *Delaunay* edges on a finite set of points in d-dimensional
space with <a href="https://en.wikipedia.org/wiki/Chebyshev_distance">Chebysehv distance</a>.

## Installation

This package requires `setuptools` and `numpy`.

To use the functionality of the `persty.util` submodule also `gudhi` needs to
be installed. See **Computing Persistent Homology** section below.

Install it with

```
>>> pip install git+https://github.com/gbeltramo/persty.git
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

## Computing Persistent Homology

Minibox and Delaunay edges can be used to compute
persistent homology in homological dimensions zero and one.

The `pesty` package provides a wrapper function to generate a `gudhi.SimplexTree()`
object that can be used to compute persistence diagrams of Minibox and Alpha Clique
filtrations.

The <a href="https://anaconda.org/conda-forge/gudhi">`gudhi`</a>
package must be installed. If you installed <a href="https://docs.conda.io/en/latest/">`conda`</a> this can be obtained by running the following command in a
terminal window.

```
>>> conda install -c conda-forge gudhi
```

The following code computes the zero and one dimensional persistence diagrams
of 1000 randomly sample points in the unit cube in $5$ dimensional space.

```python
import numpy as np
import persty.minibox
import persty.util
from scipy.spatial.distance import chebyshev

np.random.seed(0)
points = np.random.rand(100, 3).tolist()
minibox_edges = persty.minibox.edges(points)
simplex_tree = persty.util.make_gudhi_simplex_tree(points,
                                                   minibox_edges,
                                                   max_simplex_dim=2,
                                                   metric=chebyshev)
persistence_diagrams = simplex_tree.persistence(homology_coeff_field=2,
                                                persistence_dim_max=False)
```
