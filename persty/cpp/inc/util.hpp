#ifndef PERSTY_UTIL_HPP
#define PERSTY_UTIL_HPP

#include <set>
using namespace std;

namespace persty_util {
    vector<vector<double>> get_minibox(const vector<double> &p,
                                       const vector<double> &q);
    bool is_inside(const vector<double> &p,
                   const vector<vector<double>> &box);
    vector<size_t> sorted_indices_one_dimension(const vector<vector<double>>& points,
                                                size_t k);
    tuple<size_t, double> get_k_long_side_radius(const vector<double>& p,
                                                 const vector<double>& q);
    vector<vector<double>> get_A_r(const vector<double>& p,
                                   const vector<double>& q);
    vector<tuple<size_t, size_t, size_t>> get_clique_triangles(vector<tuple<size_t, size_t>> edges);
}

#endif
