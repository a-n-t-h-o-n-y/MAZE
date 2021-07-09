#ifndef MAZE_LONGEST_PATH_HPP
#define MAZE_LONGEST_PATH_HPP
#include <array>
#include <vector>

#include <maze/cell.hpp>
#include <maze/direction.hpp>
#include <maze/distance.hpp>
#include <maze/maze.hpp>
#include <maze/point.hpp>
#include <maze/utility.hpp>

namespace maze::detail {

/// Recursive implementation; inc depth first distance calc and saving solution.
/** Assumes \p at is a Passage cell. */
template <Distance Width, Distance Height>
void do_longest_path(Maze<Width, Height> const& maze,
                     Point const at,
                     Direction entry,
                     int const distance,
                     int& max_distance,
                     std::vector<Point>& current_path,
                     std::vector<Point>& solution_path)
{
    current_path.push_back(at);
    if (utility::is_dead_end(maze, at) && distance > max_distance) {
        max_distance  = distance;
        solution_path = current_path;
    }

    for (auto const direction : utility::directions) {
        if (direction != entry) {
            auto const next = utility::next_point<Width, Height>(at, direction);
            if (!next.has_value() || maze.get(*next) == Cell::Wall)
                continue;
            do_longest_path(maze, *next, utility::opposite(direction),
                            distance + 1, max_distance, current_path,
                            solution_path);
        }
    }

    current_path.pop_back();
}

}  // namespace maze::detail

namespace maze {

/// Finds the longest path along \p maze, beginning at \p start.
/** Returns an ordered list of Points, following Passage cells to the farthest
 *  point from \p start in \p maze. Returns an empty vector if \p maze and \p
 *  start are invalid in some way. \p start should only have one exit passage */
template <Distance Width, Distance Height>
[[nodiscard]] auto longest_path_from(Maze<Width, Height> const& maze,
                                     Point const start) -> std::vector<Point>
{
    auto solution_path = std::vector<Point>{};
    auto current_path  = std::vector<Point>{};
    auto max_distance  = 0;

    // Any direction that is a Wall is fine to use.
    auto start_entry = Direction::North;
    for (auto const direction : utility::directions) {
        auto const next = utility::next_point<Width, Height>(start, direction);
        if (next.has_value() && (maze.get(*next) == Cell::Wall)) {
            start_entry = direction;
            break;
        }
    }
    detail::do_longest_path(maze, start, start_entry, 0, max_distance,
                            current_path, solution_path);
    return solution_path;
}

/// finds all leaf nodes in \p Maze. Points with only a single edge.
template <Distance Width, Distance Height>
[[nodiscard]] auto find_all_leaves(Maze<Width, Height> const& m)
    -> std::vector<Point>
{
    auto result = std::vector<Point>{};
    for (Distance x = 0; x < Width; ++x) {
        for (Distance y = 0; y < Height; ++y) {
            if (m.get({x, y}) == Cell::Passage) {
                auto count = 0;
                for (auto direction : utility::directions) {
                    auto const neighbor =
                        utility::next_point<Width, Height>({x, y}, direction);
                    if (neighbor.has_value() &&
                        m.get(*neighbor) == Cell::Passage) {
                        ++count;
                    }
                }
                if (count == 1)
                    result.push_back({x, y});
            }
        }
    }
    return result;
}

template <Distance Width, Distance Height>
[[nodiscard]] auto longest_path(Maze<Width, Height> const& m)
    -> std::vector<Point>
{
    auto solution     = std::vector<Point>{};
    auto const leaves = find_all_leaves(m);
    for (auto const leaf : leaves) {
        auto path = longest_path_from(m, leaf);
        if (path.size() > solution.size())
            solution = std::move(path);
    }
    return solution;
}

}  // namespace maze
#endif  // MAZE_LONGEST_PATH_HPP
