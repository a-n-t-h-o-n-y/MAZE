#ifndef MAZE_GENERATE_PRIMS_HPP
#define MAZE_GENERATE_PRIMS_HPP
#include <array>
#include <cassert>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include <maze/cell.hpp>
#include <maze/direction.hpp>
#include <maze/distance.hpp>
#include <maze/edge.hpp>
#include <maze/maze.hpp>
#include <maze/utility.hpp>

namespace maze::detail {

/// Returns Edge in \p direction from \p at, two cells away.
/** Returns nullopt if outside of given limits from template parameters. */
template <Distance Width, Distance Height>
[[nodiscard]] auto make_edge(Point const at, Direction const direction)
    -> std::optional<Edge>
{
    auto const one = utility::next_point<Width, Height>(at, direction);
    if (one.has_value()) {
        auto const two = utility::next_point<Width, Height>(*one, direction);
        if (two.has_value())
            return Edge{at, *two};
    }
    return std::nullopt;
}

template <Distance Width, Distance Height, Direction... Ds>
[[nodiscard]] auto do_all_edges(Point at, utility::Directions_pack<Ds...>)
    -> std::array<std::optional<Edge>, 4>
{
    return {make_edge<Width, Height>(at, Ds)...};
}

/// An edge is two spaces from \p at in all coordinate combinations.
template <Distance Width, Distance Height>
[[nodiscard]] auto all_edges(Point at) -> std::array<std::optional<Edge>, 4>
{
    return do_all_edges<Width, Height>(at, utility::make_directions_pack());
}

void append(std::vector<Edge>& list,
            std::array<std::optional<Edge>, 4> const& edges)
{
    for (auto const& e : edges) {
        if (e.has_value())
            list.push_back(*e);
    }
}

/// Remove and return the Edge in \p list at \p index.
[[nodiscard]] auto pop(std::vector<Edge>& list, std::size_t index) -> Edge
{
    assert(index < list.size());
    auto const at   = std::next(std::cbegin(list), index);
    auto const edge = *at;
    list.erase(at);
    return edge;
}

/// Make a passage in \p m from edge.a to edge.b
/** edge.a is assumed to already be a passage, true for this use case. */
template <Distance Width, Distance Height>
void make_passage(Maze<Width, Height>& m, Edge const edge)
{
    m.set(edge.b, Cell::Passage);
    auto const middle =
        utility::add(edge.a, utility::half(utility::subtract(edge.b, edge.a)));
    m.set(middle, Cell::Passage);
}

/// Perform Randomized Prim's Algorithms over \p m, starting at \p start.
template <Distance Width, Distance Height>
void do_prims(Maze<Width, Height>& m, Point start)
{
    auto list = std::vector<Edge>{};
    append(list, all_edges<Width, Height>(start));

    while (!list.empty()) {
        auto const index = utility::random_index(list.size() - 1);
        auto const edge  = pop(list, index);
        if (m.get(edge.b) == Cell::Wall) {
            make_passage(m, edge);
            append(list, all_edges<Width, Height>(edge.b));
        }
    }
}

}  // namespace maze::detail

namespace maze {

/// Generate a maze with a randomized Prim's MST algorithm.
template <Distance Width, Distance Height>
[[nodiscard]] auto generate_prims() -> Maze<Width, Height>
{
    // Less unused space if coordinates are even.
    auto const start = utility::make_even<Width, Height>(
        utility::random_point<Width, Height>());

    auto maze = Maze<Width, Height>{Cell::Wall};
    maze.set(start, Cell::Passage);
    detail::do_prims(maze, start);
    return maze;
}

}  // namespace maze
#endif  // MAZE_GENERATE_PRIMS_HPP
