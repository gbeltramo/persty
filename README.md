# `persty` - Minibox and Delaunay Edges Algorithms

This package provides an implementation of algorithms for finding the
*Minibox* and *Delaunay* edges on a finite set of points in d-dimensional
space with <a href="https://en.wikipedia.org/wiki/Chebyshev_distance">Chebyshev distance</a>.

## Installation

Install it with

```
>>> pip install persty
```

For the installation to complete succesfully you need

- `setuptools` and `numpy` installed in the current Python environment;
- a `C` compiler for the module to be built. On Windows download and install
  <a href="https://visualstudio.microsoft.com/vs/">Visual Studio community</a>.

To use the functionality of the `persty.util` submodule it is also necessary to
install the <a href="https://anaconda.org/conda-forge/gudhi">`gudhi`</a> Python package.
If you installed <a href="https://docs.conda.io/en/latest/">`conda`</a> this can be obtained by running the following command in a terminal window.

```
>>> conda install -c conda-forge gudhi
```

## Basic usage

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

The following code computes the zero and one dimensional persistence diagrams
of 100 three-dimensional randomly sampled points in the unit cube.

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

## Tests

To check that this package was installed correctly you can run the tests in
the `test/` directory of this repository.

- Download this repository on you computer by running the following command in
a terminal window.

```
>>> git clone https://github.com/gbeltramo/persty.git
```

*Note*

On Windows you can obtain `git` by installing the <a href="https://chocolatey.org/">
`chocolatey`</a> package manager, and running

```
>>> choco install git
```

- In a terminal window move to the `persty` directory you just downloaded.

- If you do not have the <a href="https://pypi.org/project/pytest/">`pytest`</a>
package installed run

```
>>> pip install pytest
```

- Finally run

```
>>> pytest
```

or

```
>>> pytest -q
```

The second option decreases the verbosity of the output of this command.
