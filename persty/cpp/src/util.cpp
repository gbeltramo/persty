#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

namespace persty_util {
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
}
