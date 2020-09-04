#ifndef PERSTY_MINIBOX_HPP
#define PERSTY_MINIBOX_HPP

using namespace std;

namespace persty_minibox {
    vector<tuple<size_t, size_t>> brute_edges(vector<vector<double>> points);
    vector<tuple<size_t, size_t>> edges_2D(vector<vector<double>> points);
}

#endif
