#ifndef MAZE_POINT_HPP
#define MAZE_POINT_HPP
#include <compare>

#include <maze/distance.hpp>

namespace maze {

/// Describes a Maze Point as the Distance from the top-left in each dimension.
/** Top left is {0, 0} and increases left to right and top to bottom. */
struct Point {
    Distance x;
    Distance y;

    friend auto constexpr operator<=>(Point lhs, Point rhs) = default;
};

/// Only useful for total ordering, y is considered first, then x.
[[nodiscard]] auto constexpr operator<(Point lhs, Point rhs) -> bool
{
    return (lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x);
}

}  // namespace maze
#endif  // MAZE_POINT_HPP
