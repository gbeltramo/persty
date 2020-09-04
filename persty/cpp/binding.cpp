#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "inc/util.hpp"
#include "inc/minibox.hpp"

using namespace std;

PYBIND11_MODULE(binding, m) {
    m.doc() = "";
    m.def("get_minibox", &persty_util::get_minibox);
    m.def("is_inside", &persty_util::is_inside);
    m.def("sort_indices_one_dimension", &persty_util::sort_indices_one_dimension);
    m.def("minibox_edges_2D", &persty_minibox::edges_2D);
    m.def("brute_minibox_edges", &persty_minibox::brute_edges);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
