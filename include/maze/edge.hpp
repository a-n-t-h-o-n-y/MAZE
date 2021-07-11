#ifndef MAZE_EDGE_HPP
#define MAZE_EDGE_HPP
#include <maze/point.hpp>

namespace maze {

/// A Graph edge, specific for Maze, connection between two Points.
struct Edge {
    maze::Point a;
    maze::Point b;

    friend auto operator<=>(Edge, Edge) = default;
};

}  // namespace maze
#endif  // MAZE_EDGE_HPP
