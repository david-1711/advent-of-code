#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <deque>
#include <tuple> //tie() function
#include <initializer_list>

/**
 * @brief Calculates the perimeter of a given region.
 *
 * This function calculates the perimeter of a region represented by a set of coordinates.
 * Each coordinate is considered as a unit square, and the perimeter is calculated by
 * summing the edges of these squares, subtracting the common edges between adjacent squares.
 *
 * @param region A set of pairs representing the coordinates of the region.
 * @return The total perimeter of the region.
 */
long long calculatePerimeter(const std::set<std::pair<int, int>> &region)
{
    long long perimeter = 0;

    for (const auto &[x, y] : region)
    {
        perimeter += 4;

        for (const auto &[neighborX, neighborY] : std::initializer_list<std::pair<int, int>>{{x + 1, y}, {x - 1, y}, {x, y - 1}, {x, y + 1}})
        {
            if (region.find({neighborX, neighborY}) != region.end())
            { // for each neighbor reduce perimeter to skip counting common edges
                perimeter--;
            }
        }
    }
    return perimeter;
}

/**
 * @brief Calculates the total price of fences required for all regions in a grid.
 *
 * This function identifies all contiguous regions of the same type of plant in a grid,
 * calculates the perimeter of each region, and then computes the total price of fences
 * required to enclose all these regions. The price is calculated based on the size and
 * perimeter of each region.
 *
 * @param grid A 2D vector representing the grid of plants.
 */
void calculatePriceOfFences(const std::vector<std::vector<char>> &grid)
{
    std::set<std::set<std::pair<int, int>>> allRegions;
    std::pair<int, int> position;
    std::set<std::pair<int, int>> region;
    std::set<std::pair<int, int>> visited;
    std::deque<std::pair<int, int>> Q;
    std::pair<int, int> currentPosition;
    char plant;
    long long priceOfFences = 0;

    std::set<std::pair<int, int>> neighbors;

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            position = {i, j};

            if (visited.find(position) != visited.end())
            {
                continue;
            }
            region.clear();
            region.insert({i, j});
            visited.insert(position);
            Q.push_front(position);

            plant = grid[i][j];

            while (!Q.empty())
            {
                currentPosition = Q.back();
                Q.pop_back();

                neighbors = {
                    {currentPosition.first - 1, currentPosition.second},
                    {currentPosition.first + 1, currentPosition.second},
                    {currentPosition.first, currentPosition.second - 1},
                    {currentPosition.first, currentPosition.second + 1}};

                for (const auto &[neighborRow, neighborColumn] : neighbors)
                {
                    if (neighborRow < 0 || neighborColumn < 0 || neighborRow >= grid.size() || neighborColumn >= grid[0].size())
                    {
                        continue;
                    }
                    if (grid[neighborRow][neighborColumn] != plant)
                    {
                        continue;
                    }
                    if (region.find({neighborRow, neighborColumn}) != region.end())
                    {
                        continue;
                    }
                    region.insert({neighborRow, neighborColumn});
                    visited.insert({neighborRow, neighborColumn});
                    Q.push_front({neighborRow, neighborColumn});
                }
            }
            allRegions.insert(region);
        }
    }
    for (const auto &reg : allRegions)
    {
        // bool flag = false;
        // for (const auto &list : reg)
        // {
        //     if (!flag)
        //     {
        //         std::cout << grid[list.first][list.second] << std::endl;
        //         flag = true;
        //     }
        //     std::cout << "(" << list.first << "," << list.second << ") ";
        // }
        // std::cout << std::endl;
        priceOfFences += reg.size() * calculatePerimeter(reg);
    }
    std::cout << "Total price of fencing all regions on the map is: " << priceOfFences << std::endl;
}

/**
 * @brief Main function to read input data and calculate the price of fences.
 *
 * This function reads a grid from an input file specified as a command-line argument,
 * processes the grid to identify regions, and calculates the total price of fences
 * required to enclose all regions. The input file should contain a grid of characters
 * representing different types of plants.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return Returns 0 on success, or 1 if there is an error.
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_data>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);
    std::vector<std::vector<char>> grid;
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    while (std::getline(file, line))
    {
        std::vector<char> row(line.begin(), line.end());
        grid.push_back(row);
    }

    file.close();

    // for (const auto &row : grid)
    // {
    //     for (char ch : row)
    //     {
    //         std::cout << ch << " ";
    //     }
    //     std::cout << std::endl;
    // }

    calculatePriceOfFences(grid);

    return 0;
}