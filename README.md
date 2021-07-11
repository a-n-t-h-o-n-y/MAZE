# MAZE 🌽

C++20 Maze generation and solution finder library. Header Only.

## Algorithms

- Recursive Backtracking
- Randomized Kruskal's
- Randomized Prim's
- Aldous Broder

## Build

CMake is the supported build generator, it generates the `maze-lib` target.

## Example

```cpp
#include <iostream>
#include <utility>

#include <maze/display.hpp>
#include <maze/generate_prims.hpp>
#include <maze/longest_path.hpp>
#include <maze/maze.hpp>

int main()
{
    constexpr auto width  = 41;
    constexpr auto height = 21;

    using namespace maze;
    auto const maze     = generate_prims<width, height>();
    auto const solution = longest_path(maze);

    std::cout << std::pair{maze, solution};
}
```
