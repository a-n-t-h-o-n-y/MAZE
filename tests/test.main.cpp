#include <iostream>
#include <utility>

#include <maze/adjacency_list.hpp>
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
    auto const solution = longest_path(maze);
    auto const solution2 = longest_path_from(maze, start);

    std::cout << std::pair{maze, solution};
    std::cout << "Steps: " << solution.size() << '\n';

    maze::Adjacency_list<maze::Point> al;
    make_undirected_edge(al, {7, 3}, {3, 3});
    al.edges({7, 3}).push_back(al.add_vertex({7, 2}).first);
    // al.edges({7, 2});

    for (auto const& [point, edges] : al) {
        std::cout << point.x << ' ' << point.y << '\n';
        for (auto const& e : edges) {
            std::cout << '\t' << e.get().x << ' ' << e.get().y << '\n';
        }
    }

    return 0;
}
