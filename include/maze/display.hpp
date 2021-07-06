#ifndef MAZE_DISPLAY_HPP
#define MAZE_DISPLAY_HPP
#include <algorithm>
#include <ostream>
#include <utility>
#include <vector>

#include <maze/cell.hpp>
#include <maze/distance.hpp>
#include <maze/maze.hpp>
#include <maze/point.hpp>

namespace maze::detail {

/// Cell to display char.
[[nodiscard]] inline auto to_char(Cell c) -> char
{
    switch (c) {
        case Cell::Wall: return 'X';
        case Cell::Passage: return ' ';
    }
}

/// Returns true if \p container contains \p p. O(n) complexity.
[[nodiscard]] auto contains(std::vector<Point> container, Point p) -> bool
{
    return std::find(std::cbegin(container), std::cend(container), p) !=
           std::cend(container);
}

}  // namespace maze::detail

namespace maze {

/// Prints representation of \p maze to \p os.
/** Walls are 'X', Passages are ' ', start is 'S', and end is 'E'. */
template <Distance Width, Distance Height>
auto operator<<(std::ostream& os, Maze<Width, Height> const& maze)
    -> std::ostream&
{
    for (Distance y = 0; y < Height; ++y) {
        for (Distance x = 0; x < Width; ++x) {
            if (maze.start() == Point{x, y})
                os << 'S';
            else if (maze.end() == Point{x, y})
                os << 'E';
            else
                os << detail::to_char(maze.get({x, y}));
        }
        os << '\n';
    }
    return os;
}

/// Prints representation of \p maze_and_solution to \p os.
/** Walls are 'X', Passages are ' ', start is 'S', end is 'E', and solution is
 *  '.'. This is a relatively expensive function! */
template <Distance Width, Distance Height>
auto operator<<(
    std::ostream& os,
    std::pair<Maze<Width, Height>, std::vector<Point>> const& maze_and_solution)
    -> std::ostream&
{
    auto const& [maze, solution] = maze_and_solution;
    for (Distance y = 0; y < Height; ++y) {
        for (Distance x = 0; x < Width; ++x) {
            if (maze.start() == Point{x, y})
                os << 'S';
            else if (maze.end() == Point{x, y})
                os << 'E';
            else if (detail::contains(solution, {x, y}))
                os << '.';
            else
                os << detail::to_char(maze.get({x, y}));
        }
        os << '\n';
    }
    return os;
}

}  // namespace maze
#endif  // MAZE_DISPLAY_HPP
