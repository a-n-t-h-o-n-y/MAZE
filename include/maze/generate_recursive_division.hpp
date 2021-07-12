#ifndef MAZE_GENERATE_RECURSIVE_DIVISION_HPP
#define MAZE_GENERATE_RECURSIVE_DIVISION_HPP
#include <array>
#include <random>
#include <stdexcept>

#include <maze/cell.hpp>
#include <maze/maze.hpp>
#include <maze/point.hpp>
#include <maze/utility.hpp>

namespace maze::detail {

/// Points are inclusive, each point an inner corner of the rectangular Chamber.
struct Chamber {
    Point top_left;
    Point bottom_right;
};

/// Which way the chamber will be divided.
enum class Wall_direction { Horizontal, Vertical };

[[nodiscard]] inline auto opposite(Wall_direction x) -> Wall_direction
{
    switch (x) {
        case Wall_direction::Horizontal: return Wall_direction::Vertical;
        case Wall_direction::Vertical: return Wall_direction::Horizontal;
        default: throw std::logic_error{"Invalid Wall_direction enum"};
    }
}

/// Return a random value in the range [lo, hi].
[[nodiscard]] auto random_value(Distance const lo, Distance const hi)
    -> Distance
{
    return std::uniform_int_distribution{lo, hi}(utility::random_gen);
}

template <Distance Width, Distance Height>
void insert_horizontal_wall(Maze<Width, Height>& m, Distance y, Chamber chamber)
{
    for (Distance x = chamber.top_left.x; x <= chamber.bottom_right.x; ++x)
        m.set({x, y}, Cell::Wall);
}

template <Distance Width, Distance Height>
void insert_vertical_wall(Maze<Width, Height>& m, Distance x, Chamber chamber)
{
    for (Distance y = chamber.top_left.y; y <= chamber.bottom_right.y; ++y)
        m.set({x, y}, Cell::Wall);
}

/// Returns the closest odd value to \p x, without going over \p limit.
inline auto make_odd(Distance const x, Distance limit) -> Distance
{
    if ((x % 2) == 0)
        return (x == limit) ? (x - 1) : (x + 1);
    else
        return x;
}

/// Returns the closest even value to \p x, without going over \p limit.
inline auto make_even(Distance const x, Distance limit) -> Distance
{
    if ((x % 2) == 0)
        return x;
    else
        return (x == limit) ? (x - 1) : (x + 1);
}

template <Distance Width, Distance Height>
[[nodiscard]] auto do_horizontal_division(Maze<Width, Height>& m,
                                          Chamber chamber)
    -> std::array<Chamber, 2>
{
    // Walls are on odd intervals
    auto const split_y = make_odd(
        random_value(chamber.top_left.y + 1, chamber.bottom_right.y - 1),
        chamber.bottom_right.y - 1);
    insert_horizontal_wall(m, split_y, chamber);

    auto const opening_x =
        make_even(random_value(chamber.top_left.x, chamber.bottom_right.x),
                  chamber.bottom_right.x);
    m.set({opening_x, split_y}, Cell::Passage);

    auto const first = Chamber{
        {chamber.top_left}, {chamber.bottom_right.x, (Distance)(split_y - 1)}};
    auto const second = Chamber{{chamber.top_left.x, (Distance)(split_y + 1)},
                                {chamber.bottom_right}};
    return {first, second};
}

template <Distance Width, Distance Height>
[[nodiscard]] auto do_vertical_division(Maze<Width, Height>& m,
                                        Chamber const chamber)
    -> std::array<Chamber, 2>
{
    // Walls are on odd intervals
    auto const split_x = make_odd(
        random_value(chamber.top_left.x + 1, chamber.bottom_right.x - 1),
        chamber.bottom_right.x - 1);
    insert_vertical_wall(m, split_x, chamber);

    auto const opening_y =
        make_even(random_value(chamber.top_left.y, chamber.bottom_right.y),
                  chamber.bottom_right.y);
    m.set({split_x, opening_y}, Cell::Passage);

    auto const first = Chamber{
        {chamber.top_left}, {(Distance)(split_x - 1), chamber.bottom_right.y}};
    auto const second = Chamber{{Distance(split_x + 1), chamber.top_left.y},
                                {chamber.bottom_right}};
    return {first, second};
}

/** top_left and bottom_right are inclusive, they are not walls. */
template <Distance Width, Distance Height>
void do_recursive_division(Maze<Width, Height>& m,
                           Chamber const chamber,
                           Wall_direction const wall_direction)
{
    auto const& [top_left, bottom_right] = chamber;
    // End recursion if chamber is a single passage width.
    if ((bottom_right.x - top_left.x) < 2 || (bottom_right.y - top_left.y) < 2)
        return;

    auto next_chambers = std::array<Chamber, 2>{};
    if (wall_direction == Wall_direction::Horizontal)
        next_chambers = do_horizontal_division(m, {top_left, bottom_right});
    else
        next_chambers = do_vertical_division(m, {top_left, bottom_right});

    for (auto const c : next_chambers)
        do_recursive_division(m, c, opposite(wall_direction));
}

}  // namespace maze::detail

namespace maze {

/// Generate a maze with a Recursive Division algorithm.
template <Distance Width, Distance Height>
[[nodiscard]] auto generate_recursive_division() -> Maze<Width, Height>
{
    auto m = Maze<Width, Height>{Cell::Passage};
    do_recursive_division(m, {{0, 0}, {Width - 1, Height - 1}},
                          detail::Wall_direction::Vertical);
    return m;
}

}  // namespace maze
#endif  // MAZE_GENERATE_RECURSIVE_DIVISION_HPP
