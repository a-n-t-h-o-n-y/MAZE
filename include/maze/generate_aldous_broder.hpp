#ifndef MAZE_ALDOUS_BRODER_HPP
#define MAZE_ALDOUS_BRODER_HPP
#include <stdexcept>

#include <maze/cell.hpp>
#include <maze/distance.hpp>
#include <maze/maze.hpp>
#include <maze/point.hpp>
#include <maze/utility.hpp>

namespace maze::detail {

/// Finds a random neighbor two spaces from \p point in a single direction.
template <Distance Width, Distance Height>
[[nodiscard]] auto random_neighbor(Point const p) -> Point
{
    auto const directions = utility::shuffled_directions();
    for (auto direction : directions) {
        auto const one = utility::next_point<Width, Height>(p, direction);
        if (one.has_value()) {
            auto const two =
                utility::next_point<Width, Height>(*one, direction);
            if (two.has_value())
                return *two;
        }
    }
    throw std::logic_error{"random_neighbor: Couldn't find a neighbor."};
}

/// Find the point between two Points.
[[nodiscard]] auto middle(Point const a, Point const b) -> Point
{
    return utility::add(a, utility::half(utility::subtract(b, a)));
}

}  // namespace maze::detail

namespace maze {

/// Generate a maze with Aldous Broder Uniform Spanning Tree algorithm.
/** This is a very inefficient maze generation algorithm. But it creates nice
 *  mazes. */
template <Distance Width, Distance Height>
[[nodiscard]] auto generate_aldous_broder() -> Maze<Width, Height>
{
    auto current = utility::make_even<Width, Height>(
        utility::random_point<Width, Height>());
    auto m = Maze<Width, Height>{Cell::Wall};
    m.set(current, Cell::Passage);
    auto count = (utility::ceil(Width / 2.) * utility::ceil(Height / 2.)) - 1;

    while (count != 0) {
        auto const neighbor = detail::random_neighbor<Width, Height>(current);
        if (m.get(neighbor) == Cell::Wall) {
            m.set(neighbor, Cell::Passage);
            m.set(detail::middle(current, neighbor), Cell::Passage);
            --count;
        }
        current = neighbor;
    }

    return m;
}

}  // namespace maze
#endif  // MAZE_ALDOUS_BRODER_HPP
