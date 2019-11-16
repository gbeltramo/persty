from setuptools import setup, find_packages

setup(
    name = "persty",
    version = "0.1",
    author = "Gabriele Beltramo",
    description = "Implementation of Minibox and Delauany graphs for persistent homology computations.",
    packages = find_packages(exclude=['*test']),
    install_requires = ['numpy']
)
