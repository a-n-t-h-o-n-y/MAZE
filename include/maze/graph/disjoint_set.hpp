#ifndef MAZE_GRAPH_DISJOINT_SET_HPP
#define MAZE_GRAPH_DISJOINT_SET_HPP
#include <algorithm>
#include <concepts>
#include <memory>
#include <set>
#include <stdexcept>
#include <vector>

namespace maze::graph {

/// Disjoint set data structure.
template <std::totally_ordered T>
class Disjoint_set {
   public:
    /// Create a new set with single element \p x within the set.
    /** Does not check that \p x is not already in *this. */
    void make_set(T x) { sets_.push_back(std::set<T>{{std::move(x)}}); }

    /// Return a const reference to the set containing \p x. Non-Mutable.
    /** Throws std::range_error if \p x is not contained in *this. */
    [[nodiscard]] auto find_set(T const& x) const -> std::set<T> const&
    {
        auto const at = std::ranges::find_if(
            sets_, [&](auto const& s) { return s.contains(x); });
        if (at == std::cend(sets_))
            throw std::range_error{"Disjoint_set::find_set: invalid value."};
        return *at;
    }

    /// Combines the set that contains x with the set the contains y.
    /** Throws std::range_error if \p x or \p y are not contained in *this. */
    void merge(T const& x, T const& y)
    {
        auto const [x_set, y_set] = std::tie(mut_find_set(x), mut_find_set(y));
        if (std::addressof(x_set) != std::addressof(y_set)) {
            x_set.merge(std::move(y_set));
            auto const y_at =
                std::ranges::find_if(sets_, [&ys = y_set](auto const& s) {
                    return std::addressof(s) == std::addressof(ys);
                });
            if (y_at != std::cend(sets_))
                sets_.erase(y_at);
        }
    }

   public:
    /// Returns an iterator pointing to the first std::set<T> of *this.
    auto begin() -> typename std::vector<std::set<T>>::iterator
    {
        return std::begin(sets_);
    }

    /// Returns a const iterator pointing to the first std::set<T> of *this.
    auto begin() const -> typename std::vector<std::set<T>>::const_iterator
    {
        return std::cbegin(sets_);
    }

    /// Returns an iterator pointing to one past the last std::set<T> of *this.
    auto end() -> typename std::vector<std::set<T>>::iterator
    {
        return std::end(sets_);
    }

    /// Returns a const iter pointing to one past the last std::set<T> of *this.
    auto end() const -> typename std::vector<std::set<T>>::const_iterator
    {
        return std::cend(sets_);
    }

   private:
    std::vector<std::set<T>> sets_;

   private:
    /// Return a const reference to the set containing \p x. Mutable version.
    /** Throws std::range_error if \p x is not contained in *this. */
    [[nodiscard]] auto mut_find_set(T const& x) -> std::set<T>&
    {
        auto const at = std::ranges::find_if(
            sets_, [&](auto const& s) { return s.contains(x); });
        if (at == std::cend(sets_)) {
            throw std::range_error{
                "Disjoint_set::mut_find_set: invalid value."};
        }
        return *at;
    }
};

}  // namespace maze::graph
#endif  // MAZE_GRAPH_DISJOINT_SET_HPP
