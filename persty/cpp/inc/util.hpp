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
}

#endif
