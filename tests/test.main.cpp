#include <iostream>
#include <utility>

#include <maze/display.hpp>
#include <maze/generate_aldous_broder.hpp>
#include <maze/generate_kruskal.hpp>
#include <maze/generate_prims.hpp>
#include <maze/generate_recursive_backtracking.hpp>
#include <maze/graph/adjacency_list.hpp>
#include <maze/graph/connected_components.hpp>
#include <maze/graph/disjoint_set.hpp>
#include <maze/longest_path.hpp>
#include <maze/maze.hpp>
#include <maze/utility.hpp>

int main()
{
    constexpr auto width  = 41;
    constexpr auto height = 21;

    using namespace maze;
    auto const maze     = generate_recursive_backtracking<width, height>();
    auto const solution = longest_path(maze);

    std::cout << std::pair{maze, solution} << '\n';
    std::cout << "Steps: " << solution.size() << '\n';

    auto k_maze           = generate_kruskal<width, height>();
    auto const k_solution = longest_path(k_maze);
    std::cout << std::pair{k_maze, k_solution} << '\n';
    std::cout << "Steps: " << k_solution.size() << '\n';

    auto p_maze           = generate_prims<width, height>();
    auto const p_solution = longest_path(p_maze);
    std::cout << std::pair{p_maze, p_solution} << '\n';
    std::cout << "Steps: " << p_solution.size() << '\n';

    auto ab_maze           = generate_aldous_broder<width, height>();
    auto const ab_solution = longest_path(ab_maze);
    std::cout << std::pair{ab_maze, ab_solution} << '\n';
    std::cout << "Steps: " << ab_solution.size() << '\n';

    // maze::graph::Adjacency_list<maze::Point> al;
    // add_undirected_edge(al, {7, 3}, {3, 3});
    // al.edges_of({7, 3}).push_back(al.add_vertex({7, 2}).first);
    // // al.edges_of({7, 2});

    // al.add_vertex({76, 23}).second.push_back(al.find_vertex({7, 3}));
    // al.add_vertex({2, 5}).second.push_back(al.find_vertex({3, 3}));
    // al.add_vertex({10, 4});
    // al.add_vertex({11, 22});
    // al.add_vertex({43, 842});
    // add_directed_edge(al, {11, 22}, {333, 334});

    // for (auto const& [point, edges] : al) {
    //     std::cout << point.x << ' ' << point.y << '\n';
    //     for (auto const& e : edges) {
    //         std::cout << '\t' << e.get().x << ' ' << e.get().y << '\n';
    //     }
    // }

    // auto const connected = connected_components(al);
    // auto count           = 0;
    // for (auto const& set : connected) {
    //     std::cout << "group " << count++ << ":\n";
    //     for (auto const& value : set) {
    //         std::cout << '\t' << '{' << value.x << ' ' << value.y << '}'
    //                   << '\n';
    //     }
    // }

    // std::cout << "is_connected({76, 23}, {3, 3}): " << std::boolalpha
    //           << same_component(connected, {76, 23}, {3, 3}) << '\n';
    // std::cout << "is_connected({11, 22}, {2, 5}): " << std::boolalpha
    //           << same_component(connected, {11, 22}, {2, 5}) << '\n';

    return 0;
}
