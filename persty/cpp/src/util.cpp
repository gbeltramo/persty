#include <vector>
#include <tuple>
#include <algorithm>

// #include <iostream>

using namespace std;

namespace persty_util {

    // void print_v_d(vector<double> arr) {
    //     cout << "[ ";
    //     for (size_t i = 0; i < arr.size(); ++i) {
    //         cout << arr[i] << ", ";
    //     }
    //     cout << "]\n\n";
    // }

    vector<vector<double>> get_minibox(const vector<double>& p,
                                       const vector<double>& q) {
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

    bool is_inside(const vector<double>& p,
                   const vector<vector<double>>& box) {
        size_t dim = p.size();
        for (size_t k = 0; k < dim; ++k) {
            if (p[k] <= box[k][0] || box[k][1] <= p[k]) {
                return false;
            }
        }
        return true;
    }

    vector<size_t> sorted_indices_one_dimension(const vector<vector<double>>& points,
                                                size_t k) {
        size_t n = points.size();
        vector<tuple<double, size_t>> values_indices = {};
        for (size_t i = 0; i < n; ++i) {
            tuple<double, size_t> tmp = {points[i][k], i};
            values_indices.push_back(tmp);
        }
        sort(values_indices.begin(), values_indices.end(),
             [](const tuple<double, size_t>& item1, const tuple<double, size_t>& item2) {
                    return get<0>(item1) < get<0>(item2);
             });

        vector<size_t> sorted_indices(n, 0);
        for (size_t i = 0; i < n; ++i) {
            sorted_indices[i] = get<1>(values_indices[i]);
        }
        return sorted_indices;
    }

    tuple<size_t, double> get_k_long_side_radius(const vector<double>& p,
                                                 const vector<double>& q) {
        size_t dim = p.size();
        size_t k_long_side = 0;
        double long_side = max(abs(p[0] - q[0]), 0.0);
        for (size_t k = 0; k < dim; ++k) {
            double len_side = max(abs(p[k] - q[k]), 0.0);
            if (len_side > long_side) {
                k_long_side = k;
                long_side = len_side;
            }
        }
        double radius = long_side / 2.0;
        tuple<size_t, double> res = {k_long_side, radius};
        return res;
    }

    vector<vector<double>> get_A_r(const vector<double>& p,
                                   const vector<double>& q) {
        size_t dim = p.size();
        tuple<size_t, double> k_long_radius = get_k_long_side_radius(p, q);
        size_t k_long_side = get<0>(k_long_radius);
        double radius = get<1>(k_long_radius);
        vector<vector<double>> A_r(dim, vector<double>(2,0));
        for (size_t k = 0; k < dim; ++k) {
            if (k != k_long_side) {
                A_r[k] = {max(p[k], q[k]) - radius, min(p[k], q[k]) + radius};
            } else {
                A_r[k] = {max(p[k], q[k]) - radius, max(p[k], q[k]) - radius};
            }
        }
        return A_r;
    }

}
