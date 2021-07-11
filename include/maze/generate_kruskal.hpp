#ifndef MAZE_GENERATE_KRUSKAL_HPP
#define MAZE_GENERATE_KRUSKAL_HPP
#include <algorithm>
#include <memory>
#include <random>
#include <set>
#include <vector>

#include <maze/cell.hpp>
#include <maze/distance.hpp>
#include <maze/edge.hpp>
#include <maze/graph/disjoint_set.hpp>
#include <maze/maze.hpp>
#include <maze/point.hpp>
#include <maze/utility.hpp>

namespace maze::detail {

/// Returns a list of all possible connected edges in a maze.
template <Distance Width, Distance Height>
[[nodiscard]] auto generate_all_maze_edges() -> std::vector<Edge>
{
    auto result = std::vector<Edge>{};
    for (Distance x = 0; x < Width; ++x) {
        for (Distance y = 0; y < Height; ++y) {
            for (auto direction : utility::directions) {
                auto const next =
                    utility::next_point<Width, Height>({x, y}, direction);
                if (next.has_value())
                    result.push_back({{x, y}, *next});
            }
        }
    }
    return result;
}

/// shuffles the given vector
inline void randomize(std::vector<Edge>& edges)
{
    static auto gen = std::mt19937{std::random_device{}()};
    std::ranges::shuffle(edges, gen);
}

template <Distance Width, Distance Height>
[[nodiscard]] auto do_kruskal(std::vector<Edge> const& edges) -> std::set<Edge>
{
    auto ds = graph::Disjoint_set<maze::Point>{};
    // make a set for each point.
    for (Distance x = 0; x < Width; ++x) {
        for (Distance y = 0; y < Height; ++y) {
            ds.make_set({x, y});
        }
    }

    auto result = std::set<Edge>{};
    for (auto const& edge : edges) {
        if (std::addressof(ds.find_set(edge.a)) !=
            std::addressof(ds.find_set(edge.b))) {
            result.insert(edge);
            ds.merge(edge.a, edge.b);
        }
    }
    return result;
}

// Width and Height are the 2d Sureface values.
template <Distance Width, Distance Height>
[[nodiscard]] auto translate_to_maze(std::set<Edge> const& edges)
    -> maze::Maze<Width, Height>
{
    auto result = maze::Maze<Width, Height>{Cell::Wall};
    for (auto edge : edges) {
        using utility::add;
        using utility::subtract;
        using utility::times_two;
        result.set(times_two(edge.a), Cell::Passage);
        result.set(times_two(edge.b), Cell::Passage);
        auto const between = add(times_two(edge.a), subtract(edge.b, edge.a));
        result.set(between, Cell::Passage);
    }
    return result;
}

[[nodiscard]] constexpr auto ceil(float x) -> Distance
{
    Distance const dist_x = static_cast<Distance>(x);
    if (static_cast<float>(dist_x) == x)
        return dist_x;
    else
        return dist_x + 1;
}

}  // namespace maze::detail

namespace maze {

/// Generate a maze with a randomized Kruskal's MST algorithm.
/** Maze size should be odd to completely fill Maze. */
template <Distance Width, Distance Height>
[[nodiscard]] auto generate_kruskal() -> Maze<Width, Height>
{
    constexpr Distance half_width  = detail::ceil(Width / 2.);
    constexpr Distance half_height = detail::ceil(Height / 2.);
    auto all_edges = detail::generate_all_maze_edges<half_width, half_height>();

    detail::randomize(all_edges);

    auto const maze_edges =
        detail::do_kruskal<half_width, half_height>(all_edges);

    return detail::translate_to_maze<Width, Height>(maze_edges);
}

}  // namespace maze
#endif  // MAZE_GENERATE_KRUSKAL_HPP
