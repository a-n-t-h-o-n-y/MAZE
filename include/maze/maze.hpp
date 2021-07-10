#ifndef MAZE_MAZE_HPP
#define MAZE_MAZE_HPP
#include <bitset>
#include <cassert>
#include <cstddef>
#include <stdexcept>

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

   private:
    [[nodiscard]] static constexpr auto to_bit(Cell c) -> bool
    {
        switch (c) {
            case Cell::Wall: return false;
            case Cell::Passage: return true;
            default: throw std::logic_error{"Invalid Cell"};
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

}  // namespace maze
#endif  // MAZE_MAZE_HPP
