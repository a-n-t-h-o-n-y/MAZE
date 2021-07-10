#ifndef MAZE_GRAPH_ADJACENCY_LIST_HPP
#define MAZE_GRAPH_ADJACENCY_LIST_HPP
#include <algorithm>
#include <concepts>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <maze/utility.hpp>

namespace maze::graph {

/// Graph representation in adjacency list.
template <std::totally_ordered T>
class Adjacency_list {
   public:
    using Edges = std::vector<std::reference_wrapper<T const>>;

   public:
    /// Add a vertex with value \p value.
    /** Returns a pair referencing the inserted vertex and its edges. */
    auto add_vertex(T const& value) -> std::pair<T const&, Edges&>
    {
        auto const [iter, _] = nodes_.insert({value, Edges{}});
        return {iter->first, iter->second};
    }

    /// Remove the vertex equal to \p value. No-op if does not exist as vertex.
    void remove_vertex(T const& value)
    {
        auto const at = std::ranges::find_if(
            nodes_, [&](auto const& node) { return node.first == value; });
        if (at != std::cend(nodes_))
            nodes_.erase(at);
    }

    /// Return a const reference to the inserted vertex \p value.
    /** Throws std::range_error if value is not a vertex in *this. */
    [[nodiscard]] auto find_vertex(T const& value) const -> T const&
    {
        auto const at = std::ranges::find_if(
            nodes_, [&value](auto node) { return node.first == value; });
        if (at == std::cend(nodes_))
            throw std::range_error{"find_vertex: No such vertex."};
        else
            return at->first;
    }

    /// Return true if \p value is a vertex in *this.
    [[nodiscard]] auto contains(T const& value) -> bool
    {
        return nodes_.contains(value);
    }

    /// Return a mutable list of edges for \p vertex.
    /** Throws std::out_of_range if \p vertex is not a vertex in *this. */
    [[nodiscard]] auto edges_of(T const& vertex) -> Edges&
    {
        return nodes_.at(vertex);
    }

    /// Return a const list of edges for \p vertex.
    /** Throws std::out_of_range if \p vertex is not a vertex in *this. */
    [[nodiscard]] auto edges_of(T const& vertex) const -> Edges const&
    {
        return nodes_.at(vertex);
    }

   public:
    /// Return first const iterator to std::pair<T, Edges>.
    auto begin() const { return std::cbegin(nodes_); }

    /// Return first iterator to std::pair<T, Edges>.
    auto begin() { return std::begin(nodes_); }

    /// Return one past the last const iterator to std::pair<T, Edges>.
    auto end() const { return std::cend(nodes_); }

    /// Return one past the last iterator to std::pair<T, Edges>.
    auto end() { return std::end(nodes_); }

   private:
    std::map<T, Edges> nodes_;
};

/// Adds an undirected edge to \p list.
/** If either a or b doesn't exist in \p list, they are added to \p list. */
template <typename T>
void add_undirected_edge(Adjacency_list<T>& list, T const& a, T const& b)
{
    if (!list.contains(a))
        list.add_vertex(a);
    if (!list.contains(b))
        list.add_vertex(b);
    list.edges_of(a).push_back(list.find_vertex(b));
    list.edges_of(b).push_back(list.find_vertex(a));
}

/// Adds a directed edge to \p list.
/** If either from or to doesn't exist in \p list, they are added to \p list. */
template <typename T>
void add_directed_edge(Adjacency_list<T>& list, T const& from, T const& to)
{
    if (!list.contains(from))
        list.add_vertex(from);
    if (!list.contains(to))
        list.add_vertex(to);
    list.edges_of(from).push_back(list.find_vertex(to));
}

}  // namespace maze::graph
#endif  // MAZE_GRAPH_ADJACENCY_LIST_HPP
