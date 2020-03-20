from setuptools import setup, find_packages, Extension

minibox_ext = Extension("persty.minibox", ["persty/minibox.c"])
util_ext = Extension("persty.c_util", ["persty/c_util.c"])

setup(
    name = "persty",
    version = "0.2.1",
    author="Gabriele Beltramo",
    author_email="gabri.beltramo@gmail.com",
    description = "Implementation of Minibox and Delauany edges algorithms.",
    packages = find_packages(exclude=['*test']),
    ext_modules = [minibox_ext, util_ext],
    url="https://github.com/gbeltramo/persty",
    license='GPLv3',
    install_requires=['numpy'],
    keywords='topology data analysis, minibox graph, delaunay graph'
)
