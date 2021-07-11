#ifndef MAZE_GENERATE_RECURSIVE_BACKTRACKING_HPP
#define MAZE_GENERATE_RECURSIVE_BACKTRACKING_HPP
#include <cassert>

#include <maze/cell.hpp>
#include <maze/direction.hpp>
#include <maze/distance.hpp>
#include <maze/maze.hpp>
#include <maze/utility.hpp>

namespace maze::detail {

/// Recursive function that fills in \p maze with Passages.
template <Distance Width, Distance Height>
void do_recursive_backtrack(Maze<Width, Height>& maze, Point const at)
{
    auto const directions = utility::shuffled_directions();
    for (auto const direction : directions) {
        auto const in_between =
            utility::next_point<Width, Height>(at, direction);
        if (!in_between)
            continue;
        auto const next =
            utility::next_point<Width, Height>(*in_between, direction);
        if (!next || utility::is_passage(maze, *next))
            continue;
        maze.set(*in_between, Cell::Passage);
        maze.set(*next, Cell::Passage);
        do_recursive_backtrack(maze, *next);
    }
}

/// Return true if \p d is odd.
[[nodiscard]] inline auto is_odd(Distance d) -> bool { return (d % 2) == 1; }

/// Makes a single value even, without going over Limit.
template <Distance Limit>
[[nodiscard]] auto make_even(Distance const at) -> Distance
{
    if (is_odd(at)) {
        if (at + 1 >= Limit)
            return at - 1;
        else
            return at + 1;
    }
    else
        return at;
}

/// Returns a point that has coordinates that are even and within limits(W/H).
template <Distance Width, Distance Height>
[[nodiscard]] auto make_even(maze::Point p) -> maze::Point
{
    return {make_even<Width>(p.x), make_even<Height>(p.y)};
}

}  // namespace maze::detail

namespace maze {

/// Generate a random maze with recursive backtracking technique.
template <Distance Width, Distance Height>
[[nodiscard]] auto generate_recursive_backtracking() -> Maze<Width, Height>
{
    auto start = utility::random_point<Width, Height>();
    // Less unused space if coordinates are even.
    start     = detail::make_even<Width, Height>(start);
    auto maze = Maze<Width, Height>{Cell::Wall};
    maze.set(start, Cell::Passage);
    detail::do_recursive_backtrack(maze, start);
    return maze;
}

}  // namespace maze
#endif  // MAZE_GENERATE_RECURSIVE_BACKTRACKING_HPP
