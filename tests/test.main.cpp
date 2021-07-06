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
    auto const solution = longest_path(maze, start);

    std::cout << std::pair{maze, solution};
    std::cout << "Steps: " << solution.size() << '\n';
    return 0;
}
