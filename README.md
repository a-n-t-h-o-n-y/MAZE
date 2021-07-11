# MAZE 🌽

C++20 Maze generation and solution finder library. Header Only.

## Build

CMake is the supported build generator, it generates the `maze-lib` target.

## Example

```cpp
#include <iostream>
#include <utility>

#include <maze/display.hpp>
#include <maze/generate_recursive_backtracking.hpp>
#include <maze/longest_path.hpp>
#include <maze/maze.hpp>
#include <maze/utility.hpp>

int main()
{
    constexpr auto width  = 40;
    constexpr auto height = 20;

    using namespace maze;
    auto const start    = utility::random_point<width, height>();
    auto const maze     = generate_recursive_backtracking<width, height>(start);
    auto const solution = longest_path_from(maze, start);

    std::cout << std::pair{maze, solution};
}
```
