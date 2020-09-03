#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <tuple>

using namespace std;

void print_v_d(vector<double> arr) {
    cout << "[ ";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i] << ", ";
    }
    cout << "]\n\n";
}

void print_v_v_d(vector<vector<double>> arr) {
    cout << "[ ";
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << "[ ";
        for (size_t j = 0; j < arr[0].size(); ++j) {
            cout << arr[i][j] << ", ";
        }
        cout << "]\n";
    }
    cout << "]\n\n";
}

vector<vector<double>> get_minibox(const vector<double> &p,
                                   const vector<double> &q) {
   size_t dim = p.size();
   vector<vector<double>> mini(dim, vector<double>(2, 0));
   for (size_t k = 0; k < dim; ++k) {
       if (p[k] < q[k]) {
           mini[k][0] = p[k];
           mini[k][1] = q[k];
       } else {
           mini[k][0] = q[k];
           mini[k][1] = p[k];
       }
   }
   return mini;
}

bool is_inside(const vector<double> &p,
               const vector<vector<double>> &box) {
    size_t dim = p.size();
    for (size_t k = 0; k < dim; ++k) {
        if (p[k] <= box[k][0] || box[k][1] <= p[k]) {
            return false;
        }
    }
    return true;
}

vector<tuple<size_t, size_t>> edges(vector<vector<double>> points) {
    size_t n = points.size();

    vector<tuple<size_t, size_t>> edges = {};
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i+1; j < n; ++j) {
            vector<vector<double>> mini = get_minibox(points[i], points[j]);
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
