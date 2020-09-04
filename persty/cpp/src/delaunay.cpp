#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <limits>

#include "../inc/util.hpp"

using namespace std;

namespace persty_delaunay {

    vector<tuple<size_t, size_t>> edges_2D(vector<vector<double>> points) {
        size_t n = points.size();

        //obtain indices sorted along 0 dimension, then sort points
        vector<size_t> sorted_indices = persty_util::sort_indices_one_dimension(points, 0);
        sort(points.begin(), points.end(),
             [](const vector<double>& x, const vector<double>& y) {
                    return x[0] < y[0];
             });

        vector<tuple<size_t, size_t>> edges = {};
        for (size_t i = 0; i < n; ++i) {
            double p_x = points[i][0];
            double p_y = points[i][1];
            double front_above = std::numeric_limits<double>::infinity();
            double front_below = -1 * std::numeric_limits<double>::infinity();
            //cout << "---\nfront ab: " << front_above << "\n";
            //cout << "front be: " << front_below << "\n";
            for (size_t j = i+1; j < n; ++j) {
                double q_x = points[j][0];
                double q_y = points[j][1];
                if (q_y > p_y) {
                    if (q_y <= front_above) {
                        tuple<size_t, size_t> e = {sorted_indices[i],
                                                   sorted_indices[j]};
                        edges.push_back(e);
                        front_above = q_y;
                    }
                } else if (q_y < p_y) {
                    if (q_y >= front_below) {
                        tuple<size_t, size_t> e = {sorted_indices[i],
                                                   sorted_indices[j]};
                        edges.push_back(e);
                        front_below = q_y;
                    }
                } else {   // collinear
                    tuple<size_t, size_t> e = {sorted_indices[i],
                                               sorted_indices[j]};
                    edges.push_back(e);
                }
                if ( (front_above - front_below) < (q_x - p_x) ) {
                    break;
                }
            }
        }
        return edges;
    }

}
