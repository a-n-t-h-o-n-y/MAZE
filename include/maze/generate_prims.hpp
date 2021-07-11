#ifndef MAZE_GENERATE_PRIMS_HPP
#define MAZE_GENERATE_PRIMS_HPP
#include <maze/cell.hpp>
#include <maze/distance.hpp>
#include <maze/maze.hpp>

namespace maze {

/// Generate a maze with a randomized Prim's MST algorithm.
template <Distance Width, Distance Height>
[[nodiscard]] auto generate_prims() -> Maze<Width, Height>
{
    auto result = Maze<Width, Height>{Cell::Wall};

    return result;
}

}  // namespace maze
#endif  // MAZE_GENERATE_PRIMS_HPP
