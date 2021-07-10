#ifndef MAZE_GRAPH_CONNECTED_COMPONENTS_HPP
#define MAZE_GRAPH_CONNECTED_COMPONENTS_HPP
#include <concepts>
#include <functional>
#include <memory>

#include <maze/graph/adjacency_list.hpp>
#include <maze/graph/disjoint_set.hpp>

namespace maze::graph {

/// Creates a Disjoint_set where each set is a group of connected componenets.
template <std::totally_ordered T>
[[nodiscard]] auto connected_components(Adjacency_list<T> const& list)
    -> Disjoint_set<T>
{
    auto ds = Disjoint_set<T>{};
    for (auto const& node : list)
        ds.make_set(node.first);
    for (auto const& node : list) {
        auto const& [x, edges] = node;
        for (auto const& y : edges) {
            if (std::addressof(ds.find_set(x)) !=
                std::addressof(ds.find_set(y))) {
                ds.merge(x, y);
            }
        }
    }
    return ds;
}

/// Returns true if both \p x and \p y are in the same componenet within \p ds.
/** Should only be used with connected_components output. */
template <std::totally_ordered T>
[[nodiscard]] auto same_component(Disjoint_set<T> const& ds,
                                  T const& x,
                                  T const& y) -> bool
{
    return std::addressof(ds.find_set(x)) == std::addressof(ds.find_set(y));
}

}  // namespace maze::graph
#endif  // MAZE_GRAPH_CONNECTED_COMPONENTS_HPP
