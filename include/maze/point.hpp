#ifndef MAZE_POINT_HPP
#define MAZE_POINT_HPP
#include <maze/distance.hpp>

namespace maze {

/// Describes a Maze Point as the Distance from the top-left in each dimension.
/** Top left is {0, 0} and increases left to right and top to bottom. */
struct Point {
    Distance x;
    Distance y;

    friend auto operator==(Point, Point) -> bool = default;
    friend auto operator!=(Point, Point) -> bool = default;
};

}  // namespace maze
#endif  // MAZE_POINT_HPP
