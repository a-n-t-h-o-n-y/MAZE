#include <iostream>

#include <maze/generate_kruskal.hpp>
#include <maze/generate_recursive_backtracking.hpp>
#include <maze/maze.hpp>
#include <maze/solve.hpp>
#include <maze/utility.hpp>

int main()
{
    constexpr auto width  = 20;
    constexpr auto height = 10;

    // solve(maze::generate_kruskal<50, 50>());

    std::cout << maze::generate_recursive_backtracking<width, height>(
        maze::utility::random_point<width, height>());

    return 0;
}
