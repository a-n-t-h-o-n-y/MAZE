#ifndef MAZE_MAZE_HPP
#define MAZE_MAZE_HPP
#include <bitset>
#include <cassert>
#include <ostream>

#include <maze/cell.hpp>
#include <maze/distance.hpp>
#include <maze/point.hpp>

namespace maze {

/// 2D Representation of Cells that are either Walls or Passages.
template <Distance Width, Distance Height>
class Maze {
   public:
    /// Construct a maze where all Cells are initialize with \p all_cells.
    constexpr Maze(Cell all_cells) : data_{}
    {
        if (all_cells == Cell::Passage)
            data_.flip();
    }

   public:
    /// Return the Point where the maze starts.
    [[nodiscard]] constexpr auto start() const -> Point { return start_; }

    /// Set the point where the maze starts.
    /** asserts to check bounds in debug builds, logic error if out of bounds */
    constexpr void set_start(Point p) { start_ = p; }

    /// Return the Point where the maze ends.
    [[nodiscard]] constexpr auto end() const -> Point { return end_; }

    /// Set the point where the maze ends.
    /** asserts to check bounds in debug builds, logic error if out of bounds */
    constexpr void set_end(Point p) { end_ = p; }

    /// Get the cell representation at Point \p p.
    /** asserts to check bounds in debug builds, logic error if out of bounds */
    [[nodiscard]] constexpr auto get(Point p) const -> Cell
    {
        return to_cell(data_[to_index(p)]);
    }

    /// Set the cell at \p p to \p c.
    /** asserts to check bounds in debug builds, logic error if out of bounds */
    constexpr void set(Point p, Cell c) { data_[to_index(p)] = to_bit(c); }

   private:
    std::bitset<Width * Height> data_;

    Point start_{0, 0};
    Point end_{0, 0};

   private:
    [[nodiscard]] static constexpr auto to_bit(Cell c) -> bool
    {
        switch (c) {
            case Cell::Wall: return false;
            case Cell::Passage: return true;
        }
    }

    [[nodiscard]] static constexpr auto to_cell(bool bit) -> Cell
    {
        return bit ? Cell::Passage : Cell::Wall;
    }

    [[nodiscard]] static constexpr auto to_index(Point p) -> std::size_t
    {
        auto const index = (p.y * Width) + p.x;
        assert(index < (Width * Height));
        return index;
    }
};

/// Prints representation of \p maze to \p os
/** Walls are 'X' and Passages are ' '. */
template <Distance Width, Distance Height>
auto operator<<(std::ostream& os, Maze<Width, Height> const& maze)
    -> std::ostream&
{
    auto const to_char = [](Cell c) {
        switch (c) {
            case Cell::Wall: return 'X';
            case Cell::Passage: return ' ';
        }
    };
    for (Distance y = 0; y < Height; ++y) {
        for (Distance x = 0; x < Width; ++x) {
            if (maze.start() == Point{x, y})
                os << 'S';
            else if (maze.end() == Point{x, y})
                os << 'E';
            else
                os << to_char(maze.get({x, y}));
        }
        os << '\n';
    }
    return os;
}

}  // namespace maze
#endif  // MAZE_MAZE_HPP
