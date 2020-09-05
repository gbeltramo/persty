#ifndef PERSTY_UTIL_HPP
#define PERSTY_UTIL_HPP

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
}

#endif
