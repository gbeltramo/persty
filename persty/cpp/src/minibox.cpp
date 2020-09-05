#include <vector>
#include <tuple>
#include <algorithm>
#include <limits>

#include "../inc/util.hpp"

using namespace std;

namespace persty_minibox {

    // void print_v_d(vector<double> arr) {
    //     cout << "[ ";
    //     for (size_t i = 0; i < arr.size(); ++i) {
    //         cout << arr[i] << ", ";
    //     }
    //     cout << "]\n\n";
    // }
    //
    // void print_v_t_ds(vector<tuple<double, size_t>> arr) {
    //     cout << "[ ";
    //     for (size_t i = 0; i < arr.size(); ++i) {
    //         cout << "(" << get<0>(arr[i]) << ", " << get<1>(arr[i]) << "), ";
    //     }
    //     cout << "]\n\n";
    // }
    //
    // void print_v_v_d(vector<vector<double>> arr) {
    //     cout << "[ ";
    //     for (size_t i = 0; i < arr.size(); ++i) {
    //         cout << "[ ";
    //         for (size_t j = 0; j < arr[0].size(); ++j) {
    //             cout << arr[i][j] << ", ";
    //         }
    //         cout << "]\n";
    //     }
    //     cout << "]\n\n";
    // }

    vector<tuple<size_t, size_t>> edges_2D(vector<vector<double>> points) {
        // points need to be sorted along x-axis
        size_t n = points.size();

        vector<tuple<size_t, size_t>> edges = {};
        for (size_t i = 0; i < n; ++i) {
            double p_y = points[i][1];
            double front_above = numeric_limits<double>::infinity();
            double front_below = -1 * numeric_limits<double>::infinity();
            for (size_t j = i+1; j < n; ++j) {
                double q_x = points[j][0];
                double q_y = points[j][1];
                if (q_y > p_y) {
                    if (q_y <= front_above) {
                        tuple<size_t, size_t> e = {i, j};
                        edges.push_back(e);
                        front_above = q_y;
                    }
                } else if (q_y < p_y) {
                    if (q_y >= front_below) {
                        tuple<size_t, size_t> e = {i, j};
                        edges.push_back(e);
                        front_below = q_y;
                    }
                } else {   // collinear
                    tuple<size_t, size_t> e = {i, j};
                    edges.push_back(e);
                }
            }
        }
        return edges;
    }

    vector<tuple<size_t, size_t>> brute_edges(vector<vector<double>> points) {
        size_t n = points.size();

        vector<tuple<size_t, size_t>> edges = {};
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i+1; j < n; ++j) {
                vector<vector<double>> mini = persty_util::get_minibox(points[i], points[j]);
                bool add_edge = true;
                for (size_t m = 0; m < n; ++m) {
                    if (m != i && m != j && persty_util::is_inside(points[m], mini)) {
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
}
