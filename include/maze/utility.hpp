#ifndef MAZE_UTILITY_HPP
#define MAZE_UTILITY_HPP
#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <optional>
#include <random>
#include <stdexcept>

#include <maze/cell.hpp>
#include <maze/direction.hpp>
#include <maze/distance.hpp>
#include <maze/maze.hpp>
#include <maze/point.hpp>

namespace maze::utility {

inline auto random_gen = std::mt19937{std::random_device{}()};

inline constexpr auto directions = std::array{
    Direction::North, Direction::South, Direction::East, Direction::West};

/// Take this as a function parameter with deduced template Direction type.
template <Direction...>
struct Directions_pack {};

/// Use this to call a function taking a Directions_pack.
auto make_directions_pack() -> Directions_pack<Direction::North,
                                               Direction::South,
                                               Direction::East,
                                               Direction::West>
{
    return {};
}

/// Return all four Directions in a random order.
[[nodiscard]] auto shuffled_directions() -> std::array<Direction, 4>
{
    auto init = directions;
    std::shuffle(std::begin(init), std::end(init), random_gen);
    return init;
}

/// Generate a random Point between { [0, Width), [0, Height) };
template <Distance Width, Distance Height>
[[nodiscard]] auto random_point() -> Point
{
    static_assert(Width != 0 && Height != 0);
    static auto x_dist = std::uniform_int_distribution<Distance>{0, Width - 1};
    static auto y_dist = std::uniform_int_distribution<Distance>{0, Height - 1};

    return {x_dist(random_gen), y_dist(random_gen)};
}

/// Return adjacent Point to \p p  in Direction \p d.
/** Returns std::nullopt if outside of x: [0, Width-1); y: [0, Height-1); */
template <Distance Width, Distance Height>
[[nodiscard]] auto next_point(Point p, Direction d) -> std::optional<Point>
{
    switch (d) {
        case Direction::North:
            return (p.y - 1 < 0)
                       ? std::nullopt
                       : std::optional<Point>{{p.x, (Distance)(p.y - 1)}};
        case Direction::South:
            return (p.y + 1 >= Height)
                       ? std::nullopt
                       : std::optional<Point>{{p.x, (Distance)(p.y + 1)}};
        case Direction::East:
            return (p.x + 1 >= Width)
                       ? std::nullopt
                       : std::optional<Point>{{(Distance)(p.x + 1), p.y}};
        case Direction::West:
            return (p.x - 1 < 0)
                       ? std::nullopt
                       : std::optional<Point>{{(Distance)(p.x - 1), p.y}};
        default: throw std::logic_error{"Invalid Direction"};
    }
}

/// Return the opposite direction of \p d.
[[nodiscard]] auto opposite(Direction d) -> Direction
{
    switch (d) {
        case Direction::North: return Direction::South;
        case Direction::South: return Direction::North;
        case Direction::East: return Direction::West;
        case Direction::West: return Direction::East;
        default: throw std::logic_error{"Invalid Direction"};
    }
}

/// Returns true if \p p  is a Cell::Passage in \p maze.
template <Distance Width, Distance Height>
[[nodiscard]] auto is_passage(Maze<Width, Height> const& maze, Point p) -> bool
{
    return maze.get(p) == Cell::Passage;
}

/// Return true if there is only a single adjacent Cell::Passage to \p p.
template <Distance Width, Distance Height>
[[nodiscard]] auto is_dead_end(Maze<Width, Height> const& maze, Point p) -> bool
{
    auto passage_count = 0;
    for (auto const direction : directions) {
        auto const next = next_point<Width, Height>(p, direction);
        if (next.has_value() && maze.get(*next) == Cell::Passage)
            ++passage_count;
    }
    return passage_count == 1;
}

/// Iterator wrapper that returns the dereference of the value_t on operator*.
template <std::random_access_iterator Iter>
class Dereference_iterator {
   public:
    using value_type        = decltype(*std::declval<Iter::value_type>());
    using difference_type   = typename Iter::difference_type;
    using pointer           = value_type*;
    using reference         = value_type&;
    using iterator_category = typename Iter::iterator_category;

   public:
    explicit Dereference_iterator(Iter iter) : iter_{iter} {}

   public:
    // check decltype auto is correct
    auto operator*() const -> decltype(auto) { return *(*iter_); }

    auto operator++() -> Dereference_iterator&
    {
        ++iter_;
        return *this;
    }

    [[nodiscard]] auto operator++(int) -> Dereference_iterator
    {
        return Dereference_iterator{iter_++};
    }

    auto operator+=(difference_type n) -> Dereference_iterator&
    {
        iter_ += n;
        return *this;
    }

    [[nodiscard]] auto operator+(difference_type n) const
        -> Dereference_iterator
    {
        Dereference_iterator{iter_ + n};
    }

    [[nodiscard]] friend auto operator+(difference_type n,
                                        Dereference_iterator const& iter)
        -> Dereference_iterator
    {
        Dereference_iterator{iter + n};
    }

    auto operator-=(difference_type n) -> Dereference_iterator&
    {
        iter_ -= n;
        return *this;
    }

    [[nodiscard]] auto operator-(difference_type n) const
        -> Dereference_iterator
    {
        Dereference_iterator{iter_ - n};
    }

    [[nodiscard]] auto operator-(Dereference_iterator const& other) const
        -> difference_type
    {
        return iter_ - other.iter_;
    }

    [[nodiscard]] auto operator[](difference_type n) -> decltype(auto)
    {
        return iter_[n];
    }

   public:
    [[nodiscard]] auto operator==(Dereference_iterator const& other) -> bool
    {
        return iter_ == other.iter_;
    }

    [[nodiscard]] auto operator!=(Dereference_iterator const& other) -> bool
    {
        return iter_ != other.iter_;
    }

    [[nodiscard]] auto operator<(Dereference_iterator const& other) -> bool
    {
        return iter_ < other.iter_;
    }

    [[nodiscard]] auto operator<=(Dereference_iterator const& other) -> bool
    {
        return iter_ <= other.iter_;
    }

    [[nodiscard]] auto operator>(Dereference_iterator const& other) -> bool
    {
        return iter_ > other.iter_;
    }

    [[nodiscard]] auto operator>=(Dereference_iterator const& other) -> bool
    {
        return iter_ >= other.iter_;
    }

   private:
    Iter iter_;
};

/// Return true if \p d is odd.
[[nodiscard]] inline auto is_odd(Distance d) -> bool { return (d % 2) == 1; }

/// Makes a single value even, without going over Limit.
template <Distance Limit>
[[nodiscard]] auto make_even(Distance const at) -> Distance
{
    if (utility::is_odd(at)) {
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

[[nodiscard]] auto times_two(Point p) -> Point
{
    return {(Distance)(p.x * 2), (Distance)(p.y * 2)};
}

/// Result of subtracting two Distances.
using Distance_diff = std::int16_t;

struct Point_diff {
    Distance_diff x;
    Distance_diff y;
};

[[nodiscard]] auto subtract(Point a, Point b) -> Point_diff
{
    return {(Distance_diff)(a.x - b.x), (Distance_diff)(a.y - b.y)};
}

[[nodiscard]] auto add(Point a, Point b) -> Point
{
    return {(Distance)(a.x + b.x), (Distance)(a.y + b.y)};
}

[[nodiscard]] auto add(Point a, Point_diff b) -> Point
{
    return {(Distance)(a.x + b.x), (Distance)(a.y + b.y)};
}

[[nodiscard]] auto add(Point_diff a, Point b) -> Point
{
    return {(Distance)(a.x + b.x), (Distance)(a.y + b.y)};
}

[[nodiscard]] auto half(Point p) -> Point
{
    return {(Distance)(p.x / 2), (Distance)(p.y / 2)};
}

[[nodiscard]] auto half(Point_diff p) -> Point_diff
{
    return {(Distance_diff)(p.x / 2), (Distance_diff)(p.y / 2)};
}

/// Generates a random index from [0, limit].
[[nodiscard]] auto random_index(std::size_t limit) -> std::size_t
{
    auto dist = std::uniform_int_distribution<std::size_t>{0, limit};
    return dist(random_gen);
}

[[nodiscard]] constexpr auto ceil(float x) -> Distance
{
    Distance const dist_x = static_cast<Distance>(x);
    if (static_cast<float>(dist_x) == x)
        return dist_x;
    else
        return dist_x + 1;
}

}  // namespace maze::utility
#endif  // MAZE_UTILITY_HPP
