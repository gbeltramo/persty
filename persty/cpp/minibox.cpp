#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

using namespace std;

vector<tuple<double, double>> get_minibox(const vector<double> &p,
                                          const vector<double> &q) {
   size_t dim = p.size();
   vector<tuple<double, double>> mini = {};
   for (size_t k = 0; k < dim; ++k) {
       tuple<double, double> range_k = {};
       if (p[k] < q[k]) {
           get<0>(range_k) = p[k];
           get<1>(range_k) = q[k];
       } else {
           get<0>(range_k) = q[k];
           get<1>(range_k) = p[k];
       }
       mini.push_back(range_k);
   }
   return mini;
}

bool is_inside(const vector<double> &p,
               const vector<tuple<double, double>> &box) {
    size_t dim = p.size();
    tuple<double, double> range_k = {};
    for (size_t k = 0; k < dim; ++k) {
        range_k = box[k];
        if (p[k] <= get<0>(range_k) || get<1>(range_k) <= p[k]) {
            return false;
        }
    }
    return true;
}

vector<tuple<size_t, size_t>> edges(vector<vector<double>> points) {
    size_t n = points.size();

    vector<tuple<size_t, size_t>> edges = {};
    vector<tuple<double, double>> mini = {};
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i+1; j < n; ++j) {
            mini = get_minibox(points[i], points[j]);
            bool add_edge = true;
            for (size_t m = 0; m < n; ++m) {
                if (m != i && m != j && is_inside(points[m], mini)) {
                    add_edge = false;
                    break;
                }
            }
            if (add_edge == true) {
                tuple<size_t, size_t> e = {i, j};
                edges.push_back(e);
            }
        }
    }

    return edges;
}


//================================================

PYBIND11_MODULE(minibox, m) {
    m.doc() = "";
    m.def("get_minibox", &get_minibox);
    m.def("is_inside", &is_inside);
    m.def("edges", &edges);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
