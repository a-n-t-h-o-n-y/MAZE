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

}  // namespace maze::detail

namespace maze {

/// Generate a random maze with recursive backtracking technique.
template <Distance Width, Distance Height>
[[nodiscard]] auto generate_recursive_backtracking() -> Maze<Width, Height>
{
    auto const start = utility::random_point<Width, Height>();
    auto maze        = Maze<Width, Height>{Cell::Wall};
    maze.set(start, Cell::Passage);
    detail::do_recursive_backtrack(maze, start);
    return maze;
}

}  // namespace maze
#endif  // MAZE_GENERATE_RECURSIVE_BACKTRACKING_HPP
