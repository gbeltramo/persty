#include <vector>
#include <tuple>
#include <algorithm>
#include <limits>

#include <iostream>

#include "../inc/util.hpp"

using namespace std;

namespace persty_delaunay {

    void print_v_d(vector<double> arr) {
        cout << "[ ";
        for (size_t i = 0; i < arr.size(); ++i) {
            cout << arr[i] << ", ";
        }
        cout << "]\n\n";
    }

    void print_v_t_ds(vector<tuple<double, size_t>> arr) {
        cout << "[ ";
        for (size_t i = 0; i < arr.size(); ++i) {
            cout << "(" << get<0>(arr[i]) << ", " << get<1>(arr[i]) << "), ";
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

    vector<tuple<size_t, size_t>> horizontal_edges_2D(vector<vector<double>> points) {
        // points need to be sorted along x-axis
        size_t n = points.size();
        vector<double> x_values(n, 0);
        for (size_t i = 0; i < n; ++i) {
            x_values[i] = points[i][0];
        }

        vector<tuple<size_t, size_t>> edges = {};
        for (size_t i = 0; i < n; ++i) {
            double p_x = points[i][0];
            double p_y = points[i][1];
            double front_above = numeric_limits<double>::infinity();
            double front_below = -1 * numeric_limits<double>::infinity();
            //cout << "---\nfront ab: " << front_above << "\n";
            //cout << "front be: " << front_below << "\n";
            for (size_t j = i+1; j < n; ++j) {
                double q_x = points[j][0];
                double q_y = points[j][1];
                if ( (front_above - front_below) < (q_x - p_x) ) {
                    break;
                }
                if (q_y > p_y) {
                    if (q_y <= front_above) {
                        // (p,q) is minibox, check A^{\bar{r}} is not covered
                        if ((q_x - p_x) >= (q_y - p_y)) { // A_r is vertical --> add edge
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                            front_above = q_y;
                        } else {
                            front_above = q_y;
                            //vector<vector<double>> A_r = persty_util::get_A_r(points[i], points[j]);
                        }
                    }
                } else if (q_y < p_y) {
                    if (q_y >= front_below) {
                        // (p,q) is minibox, check A^{\bar{r}} is not covered
                        if ( (q_x - p_x) >= (p_y - q_y) ) { // A_r is vertical --> add edge
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                            front_below = q_y;
                        } else {
                            front_below = q_y;
                            //vector<vector<double>> A_r = persty_util::get_A_r(points[i], points[j]);
                            // cout << "---\nbefore below\n";
                            // cout << "A_r[0] = " << A_r[0][0] << ", " << A_r[0][1] << "\n";
                            // cout << "radius = " << radius <<" | points\n";
                            // print_v_v_d(points);
                        }
                    }
                } else {   // collinear
                    tuple<size_t, size_t> e = {i, j};
                    edges.push_back(e);
                }
            }
        }
        return edges;
    }

}
