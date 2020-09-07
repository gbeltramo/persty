#include <vector>
#include <tuple>
#include <map>
#include <utility>
#include <algorithm>
#include <limits>

#include "../inc/util.hpp"

#include <iostream>

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

    bool strictly_dominates(const vector<double>& p, const vector<double>& q, bool debug) {
        if (debug) {
            cout << "\t\tp --> " << p[0] << ", " << p[1] << "\n";
            cout << "\t\tq --> " << q[0] << ", " << q[1] << "\n";
        }
        size_t dim = p.size();
        for (size_t i = 0; i < dim; ++i) {
            if (p[i] >= q[i]) {   // q does not dominate p
                if (debug) cout << "\t\tfalse\n";
                return false;
            }
        }
        if (debug) cout << "\t\ttrue\n";
        return true;
    }

    void update_next_el_iter(map<double, vector<double>>& front,
                             const vector<double>& q,
                             map<double, vector<double>>::iterator& next_el_iter, bool debug) {
        if (debug) cout << "=====\n";
        while (std::distance(next_el_iter, front.end()) != 0 && strictly_dominates(q, (*next_el_iter).second, debug)) {
            ++next_el_iter;
            if (debug) cout << std::distance(next_el_iter, front.end()) << " ";
        }
        if (debug) cout << "\n=====\n";
    }

    bool check_update_front(const vector<double>& q_proj,
                            map<double, vector<double>>& front, bool debug) {
        size_t len_front = front.size();
        auto next_el_iter = front.upper_bound(q_proj[0]);
        size_t inverse_index = std::distance(next_el_iter, front.end());

        if (debug) {
            cout << "q_y = " << q_proj[0] << "\n";
            cout << "starting front = ";
            for (const auto& element: front) {
                auto first = element.first;
                auto second = element.second;
                cout << "(" << first << " | " << second[0] << " " << second[1] << ")" << "\n";
            }
            cout << "inverse index = " << inverse_index << "\n";
        }

        if (inverse_index == len_front) {
            pair<double, vector<double>> element = *next_el_iter;
            if (debug) {
                cout << "value before = " << element.first << "\n";
            }
            update_next_el_iter(front, q_proj, next_el_iter, debug);
            front.erase(front.begin(), next_el_iter);
            front.insert({q_proj[0], q_proj});

            if (debug) {
                cout << "front after = ";
                for (const auto& element: front) {
                    auto first = element.first;
                    auto second = element.second;
                    cout << "(" << first << " | " << second[0] << " " << second[1] << ")" << "\n";
                }
                cout << "inverse index = " << inverse_index << "\n";
            }

            return true;
        } else {
            pair<double, vector<double>> element = *(--next_el_iter);   // moving one position before .end()
            vector<double> y = element.second;
            if (debug) {
                cout << "final middle = " << element.first << "\n";
            }
            if (strictly_dominates(y, q_proj, debug)) {
                return false;
            } else {
                ++next_el_iter;
                map<double, vector<double>>::iterator start_iter = next_el_iter;
                update_next_el_iter(front, q_proj, next_el_iter, debug);
                front.erase(start_iter, next_el_iter);
                if (debug) cout << "q_proj[0] = " << q_proj[0] << "\n";
                front.insert({q_proj[0], q_proj});

                if (debug) {
                    cout << "front after = ";
                    for (const auto& element: front) {
                        auto first = element.first;
                        auto second = element.second;
                        cout << "(" << first << " | " << second[0] << " " << second[1] << ")" << "\n";
                    }
                }

                return true;
            }
        }
    }

    vector<tuple<size_t, size_t>> edges_3D(vector<vector<double>> points) {
        // points need to be sorted along x-axis
        // cout << "calling pesty_minibox::edges_3D\n";

        size_t n = points.size();

        vector<tuple<size_t, size_t>> edges = {};
        for (size_t i = 0; i < n; ++i) {
            // cout << "------------------\ni = " << i << "\n";
            double p_y = points[i][1];
            double p_z = points[i][2];
            map<double, vector<double>> front_above_left  = {};
            map<double, vector<double>> front_above_right = {};
            map<double, vector<double>> front_below_left  = {};
            map<double, vector<double>> front_below_right = {};
            for (size_t j = i+1; j < n; ++j) {
                bool debug = false;
                // cout << "---\nj = " << j << "\n";
                double q_y = points[j][1];
                double q_z = points[j][2];
                vector<double> q_proj = {q_y, q_z};
                // if (i == 3) {
                //     debug = true;
                // }
                if (debug) {
                    cout << "p_y, p_z = " << p_y << ", " << p_z << "\n";
                    cout << "q_x, q_y, q_z = " << points[j][0] << ", " << q_y << ", " << q_z << "\n";
                }
                if (q_z > p_z) {               // above
                    if (q_y < p_y) {           // left
                        if (debug) cout << "A L\n";
                        q_proj[0] = p_y - q_proj[0];   // reflection to first quadrant
                        if (front_above_left.empty()) {
                            front_above_left.insert({q_proj[0], q_proj});
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                        } else if (check_update_front(q_proj, front_above_left, debug)) {
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                        }
                    } else {                   // right
                        if (debug) cout << "A R\n";
                        if (front_above_right.empty()) {
                            front_above_right.insert({q_proj[0], q_proj});
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                        } else if (check_update_front(q_proj, front_above_right, debug)) {
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                        }
                    }
                } else if (q_z < p_z) {        // below
                    if (q_y < p_y) {           // left
                        if (debug) cout << "B L\n";
                        q_proj[0] = p_y - q_proj[0];   // reflection to first quadrant
                        q_proj[1] = p_z - q_proj[1];
                        if (front_below_left.empty()) {
                            if (debug) cout << "empty\n";
                            front_below_left.insert({q_proj[0], q_proj});
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                        } else if (check_update_front(q_proj, front_below_left, debug)) {
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                        }
                    } else {                   // right
                        if (debug) cout << "B R\n";
                        q_proj[1] = p_z - q_proj[1];   // reflection to first quadrant
                        if (front_below_right.empty()) {
                            front_below_right.insert({q_proj[0], q_proj});
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                        } else if (check_update_front(q_proj, front_below_right, debug)) {
                            tuple<size_t, size_t> e = {i, j};
                            edges.push_back(e);
                        }
                    }
                } else {                       // collinear
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
