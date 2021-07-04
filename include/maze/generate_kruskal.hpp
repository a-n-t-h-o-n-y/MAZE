#ifndef MAZE_GENERATE_KRUSKAL_HPP
#define MAZE_GENERATE_KRUSKAL_HPP
#include <maze/cell.hpp>
#include <maze/distance.hpp>
#include <maze/maze.hpp>

namespace maze {

template <Distance Width, Distance Height>
auto generate_kruskal() -> Maze<Width, Height>
{
    return {Cell::Wall};
}

}  // namespace maze
#endif  // MAZE_GENERATE_KRUSKAL_HPP
