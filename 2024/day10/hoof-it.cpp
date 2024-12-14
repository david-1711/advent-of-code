#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <set>
#include <map>

/**
 * @brief Converts a character to an integer.
 *
 * This function converts a digit character to its corresponding integer value.
 * If the character is not a digit, it prints an error message and returns -1.
 *
 * @param c The character to convert.
 * @return The integer value of the character, or -1 if the character is not a digit.
 */
int charToInt(char c)
{
    if (isdigit(c))
    {
        return c - '0';
    }
    else
    {
        std::cerr << "ERROR: Non-digit character encountered: " << c << std::endl;
        return -1;
    }
}

/**
 * @brief Finds trailheads in a grid.
 *
 * This function scans a 2D grid and identifies all positions (trailheads) where the value is 0.
 *
 * @param grid The 2D grid to scan.
 * @return A vector of pairs representing the coordinates of the trailheads.
 */
std::vector<std::pair<int, int>> findTrailheads(const std::vector<std::vector<int>> &grid)
{
    std::vector<std::pair<int, int>> trailheads;
    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == 0)
            {
                trailheads.push_back({i, j});
            }
        }
    }
    return trailheads;
}

/**
 * @brief Calculates the score for a trailhead using BFS.
 *
 * This function performs a Breadth-First Search (BFS) starting from a given trailhead
 * to calculate the number of hilltops (positions with value 9) reachable from that trailhead.
 *
 * @param grid The 2D grid representing the terrain.
 * @param trailhead The starting position for the BFS.
 * @return The number of hilltops reachable from the trailhead.
 */
int calculateScoreForTrailhead(const std::vector<std::vector<int>> &grid, const std::pair<int, int> &trailhead)
{
    std::deque<std::pair<int, int>> Q;
    std::set<std::pair<int, int>> visited;
    int hillTops = 0;
    std::pair<int, int> currentItem;

    Q.push_back(trailhead);
    visited.insert(trailhead);

    while (!Q.empty())
    {
        currentItem = Q.back();
        Q.pop_back();

        std::set<std::pair<int, int>> neighbors = {{currentItem.first - 1, currentItem.second},
                                                   {currentItem.first + 1, currentItem.second},
                                                   {currentItem.first, currentItem.second - 1},
                                                   {currentItem.first, currentItem.second + 1}};

        for (const auto &[neighborRow, neighborColumn] : neighbors)
        {
            if (neighborRow < 0 || neighborColumn < 0 || neighborRow >= grid.size() || neighborColumn >= grid[0].size())
                continue;

            if (grid[neighborRow][neighborColumn] != (grid[currentItem.first][currentItem.second] + 1))
                continue;

            if (visited.count({neighborRow, neighborColumn}))
                continue;

            visited.insert({neighborRow, neighborColumn});

            if (grid[neighborRow][neighborColumn] == 9)
                hillTops++;
            else
                Q.push_front({neighborRow, neighborColumn});
        }
    }
    return hillTops;
}

/**
 * @brief Calculates the rating for a trailhead using BFS.
 *
 * This function performs a Breadth-First Search (BFS) starting from a given trailhead
 * to calculate the number of distinct trails to reach hilltops (positions with value 9) from that trailhead.
 *
 * @param grid The 2D grid representing the terrain.
 * @param trailhead The starting position for the BFS.
 * @return The number of distinct trails to reach hilltops from the trailhead.
 */
int calculateRatingForTrailhead(const std::vector<std::vector<int>> &grid, const std::pair<int, int> &trailhead)
{
    std::deque<std::pair<int, int>> Q;
    std::map<std::pair<int, int>, int> visited;
    int numberOfTrails = 0;
    std::pair<int, int> currentItem;

    Q.push_back(trailhead);
    visited.insert({trailhead, 1});

    while (!Q.empty())
    {
        currentItem = Q.back();
        Q.pop_back();

        if (grid[currentItem.first][currentItem.second] == 9)
        {
            numberOfTrails += visited.at({currentItem.first, currentItem.second});
        }

        std::set<std::pair<int, int>> neighbors = {{currentItem.first - 1, currentItem.second},
                                                   {currentItem.first + 1, currentItem.second},
                                                   {currentItem.first, currentItem.second - 1},
                                                   {currentItem.first, currentItem.second + 1}};

        for (const auto &[neighborRow, neighborColumn] : neighbors)
        {
            if (neighborRow < 0 || neighborColumn < 0 || neighborRow >= grid.size() || neighborColumn >= grid[0].size())
                continue;

            if (grid[neighborRow][neighborColumn] != (grid[currentItem.first][currentItem.second] + 1))
                continue;

            if (visited.count({neighborRow, neighborColumn}))
            {
                visited.at({neighborRow, neighborColumn}) += visited.at({currentItem.first, currentItem.second});
                continue;
            }

            visited[{neighborRow, neighborColumn}] = visited.at({currentItem.first, currentItem.second});

            Q.push_front({neighborRow, neighborColumn});
        }
    }
    return numberOfTrails;
}

/**
 * @brief Main function to read input data and calculate scores for trailheads.
 *
 * This function reads a grid from an input file, finds all trailheads, and calculates the sum of scores(Part One) and sum of rates(Part Two)
 * for all trailheads.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 on success, 1 on failure.
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
    std::vector<std::vector<int>> grid;
    std::string lineStr;
    std::vector<int> line;
    std::vector<std::pair<int, int>> trailheads;
    int sumOfScores = 0;
    int sumOfRatings = 0;

    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    while (std::getline(file, lineStr))
    {
        for (const auto &ch : lineStr)
        {
            line.push_back(charToInt(ch));
        }
        grid.push_back(line);
        line.clear();
    }
    file.close();

    trailheads = findTrailheads(grid);

    for (const auto &th : trailheads)
    {
        std::cout << "(" << th.first << "," << th.second << ")" << std::endl;
        sumOfScores += calculateScoreForTrailhead(grid, th);
        sumOfRatings += calculateRatingForTrailhead(grid, th);
    }
    std::cout << std::endl;

    // Score is number of 9-height positions reachable from that trailhead
    std::cout << "Sum of scores of all trailheads(Part One): " << sumOfScores << std::endl;

    // Rating is number of distinct hiking trails which begin at that trailhead
    std::cout << "Sum of ratings of all trailheads(Part Two): " << sumOfRatings << std::endl;

    return 0;
}