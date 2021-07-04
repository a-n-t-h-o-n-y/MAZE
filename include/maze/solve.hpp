#ifndef MAZE_SOLVE_HPP
#define MAZE_SOLVE_HPP
#include <vector>

#include <maze/distance.hpp>
#include <maze/maze.hpp>
#include <maze/point.hpp>

namespace maze {

/// Returns an ordered list of cells, from start of the Maze to the end.
/** Returns empty vector if \p maze is not solvable. */
template <Distance Width, Distance Height>
auto solve(Maze<Width, Height> const& maze) -> std::vector<Point>
{
    (void)maze;
    return {};
}

}  // namespace maze
#endif  // MAZE_SOLVE_HPP
