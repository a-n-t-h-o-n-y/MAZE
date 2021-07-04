#ifndef MAZE_UTILITY_HPP
#define MAZE_UTILITY_HPP
#include <algorithm>
#include <array>
#include <iterator>
#include <optional>
#include <random>

#include <maze/cell.hpp>
#include <maze/direction.hpp>
#include <maze/distance.hpp>
#include <maze/maze.hpp>
#include <maze/point.hpp>

namespace maze::utility {

inline auto random_gen = std::mt19937{std::random_device{}()};

/// Return all four Directions in a random order.
[[nodiscard]] auto shuffled_directions() -> std::array<Direction, 4>
{
    auto init = std::array<Direction, 4>{Direction::North, Direction::South,
                                         Direction::East, Direction::West};
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
    }
}

/// Returns true if \p p  is a Cell::Passage in \p maze.
template <Distance Width, Distance Height>
[[nodiscard]] auto is_passage(Maze<Width, Height> const& maze, Point p) -> bool
{
    return maze.get(p) == Cell::Passage;
}

}  // namespace maze::utility
#endif  // MAZE_UTILITY_HPP
