#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>

/**
 * @brief Directions for searching in the grid.
 *
 * The directions are represented as pairs of integers, where each pair
 * corresponds to a direction: right, down, down-right diagonal, down-left diagonal,
 * left, up, up-left diagonal, and up-right diagonal.
 * Right (0, 1), Down (1, 0), Down-right diagonal (1, 1), Down-left diagonal (1, -1),
 * Left (0, -1), Up (-1, 0), Up-left diagonal (-1, -1), Up-right diagonal (-1, 1)
 */
std::vector<std::pair<int, int>> directions = {
    {0, 1}, {1, 0}, {1, 1}, {1, -1}, {0, -1}, {-1, 0}, {-1, -1}, {-1, 1}};

/**
 * @brief Searches for the substring starting from a given position in a specific direction.
 *
 * @param grid The 2D grid of characters.
 * @param word The substring to search for.
 * @param x The starting row index.
 * @param y The starting column index.
 * @param dx The row direction increment.
 * @param dy The column direction increment.
 * @return true if the substring is found in the specified direction, false otherwise.
 */
bool search_in_specified_direction(const std::vector<std::vector<char>> &grid, const std::string &word, int x, int y, int dx, int dy)
{
    int rows = grid.size();
    int cols = grid[0].size();
    int word_len = word.length();

    for (int i = 0; i < word_len; ++i)
    {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx < 0 || ny < 0 || nx >= rows || ny >= cols || grid[nx][ny] != word[i])
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Finds all occurrences of a substring in a 2D grid of characters.
 *
 * The function searches for the substring in all eight possible directions
 * (horizontal, vertical, diagonal, and their reverse orders).
 *
 * @param grid The 2D grid of characters.
 * @param word The substring to search for.
 * @return A vector of tuples, each containing the starting row index, starting column index,
 *         row direction increment, and column direction increment for each occurrence of the substring.
 */
std::vector<std::tuple<int, int, int, int>> find_substring(const std::vector<std::vector<char>> &grid, const std::string &word)
{
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::tuple<int, int, int, int>> occurrences;

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            for (auto [dx, dy] : directions)
            {
                if (search_in_specified_direction(grid, word, r, c, dx, dy))
                {
                    occurrences.emplace_back(r, c, dx, dy);
                }
            }
        }
    }
    return occurrences;
}

/**
 * @brief Finds the positions of the character 'A' which is the center of an "X" shaped substring "MAS" in a 2D grid.
 *
 * This function scans a 2D grid to find all positions where the character 'A' is surrounded diagonally by the characters
 * 'M' and 'S' in any of the four possible "X" shaped patterns.
 *
 * @param grid A 2D vector of characters representing the grid.
 * @return A vector of pairs, where each pair contains the row and column indices of the center 'A' of an "X" shaped substring "MAS".
 */
std::vector<std::pair<int, int>> find_x_shape_centers(const std::vector<std::vector<char>> &grid)
{
    std::vector<std::pair<int, int>> centers;
    int rows = grid.size();
    int cols = grid[0].size();

    for (int i = 1; i < rows - 1; ++i)
    {
        for (int j = 1; j < cols - 1; ++j)
        {
            if (grid[i][j] == 'A' &&
                ((grid[i - 1][j - 1] == 'M' && grid[i - 1][j + 1] == 'M' && grid[i + 1][j - 1] == 'S' && grid[i + 1][j + 1] == 'S') ||
                 (grid[i - 1][j - 1] == 'M' && grid[i - 1][j + 1] == 'S' && grid[i + 1][j - 1] == 'M' && grid[i + 1][j + 1] == 'S') ||
                 (grid[i - 1][j - 1] == 'S' && grid[i - 1][j + 1] == 'S' && grid[i + 1][j - 1] == 'M' && grid[i + 1][j + 1] == 'M') ||
                 (grid[i - 1][j - 1] == 'S' && grid[i - 1][j + 1] == 'M' && grid[i + 1][j - 1] == 'S' && grid[i + 1][j + 1] == 'M')))
            {
                centers.push_back({i, j});
            }
        }
    }

    return centers;
}
/**
 * @brief The main function.
 *
 * Initializes the grid and the substring to search for, calls the find_substring function,
 * and prints the results.
 *
 * @return int Exit status.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_data>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);
    std::vector<std::vector<char>> grid;
    std::string line;
    std::string word = "XMAS";
    int xmasCounter = 0;
    int xShapedMassCounter = 0;

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

    auto occurrences = find_substring(grid, word);
    for (const auto &[r, c, dx, dy] : occurrences)
    {
        // std::cout << "Found at (" << r << ", " << c << ") in direction (" << dx << ", " << dy << ")\n";
        xmasCounter++;
    }

    std::cout << word << " appeared " << xmasCounter << " times." << std::endl;

    auto positions = find_x_shape_centers(grid);

    for (const auto &pos : positions)
    {
        // std::cout << "Center of X shape found at: (" << pos.first << ", " << pos.second << ")" << std::endl;
        xShapedMassCounter++;
    }

    std::cout << "X shaped " << word << " appeared " << xShapedMassCounter << " times." << std::endl;

    return 0;
}