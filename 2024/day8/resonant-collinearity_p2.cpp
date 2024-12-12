#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

/**
 * @brief Represents a position on a 2D grid with (x, y) coordinates.
 *
 * The Position struct holds the x (row) and y (column) coordinates of a position on the grid.
 * It allows positions to be compared using the '<' operator, enabling sorting and use in ordered containers
 * like std::set or std::map.
 */
struct Position
{
    int x; ///< The row index of the position
    int y; ///< The column index of the position

    /**
     * @brief Compares two Position objects for ordering.
     *
     * Positions are compared first by the x-coordinate (row) and, if they are equal, by the y-coordinate (column).
     * This allows Position objects to be sorted lexicographically, like words in a dictionary.
     *
     * @param other The other Position to compare with.
     * @return true if this position is "less than" the other position.
     */
    bool operator<(const Position &other) const
    {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

/**
 * @brief Main function that reads a grid from a file and processes antenna locations.
 *
 * The main function reads a grid from a specified file, identifies antenna locations,
 * calculates antinode positions, and counts the number of unique antinode locations within the grid.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns 0 on success, or 1 if an error occurs.
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
    std::vector<std::string> grid;
    std::map<char, std::set<Position>> antennaLocations;
    std::set<Position> antinodes;
    int numberOfAntinodesInsideGrid = 0;

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            grid.push_back(line);
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            std::cout << grid[i][j] << " ";
            if (grid[i][j] != '.')
            {
                antennaLocations[grid[i][j]].insert({i, j});
            }
        }
        std::cout << std::endl;
    }

    for (const auto &antenna : antennaLocations)
    {
        std::cout << "Antenna: " << antenna.first << std::endl;
        std::cout << "Locations: ";
        for (const auto &loc : antenna.second)
        {
            std::cout << "(" << loc.x << "," << loc.y << ") ";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < antenna.second.size(); ++i)
        {
            for (size_t j = 0; j < antenna.second.size(); ++j)
            {
                if (i == j) continue;
                std::set<Position>::iterator itFirst = antenna.second.begin();
                std::set<Position>::iterator itSecond = antenna.second.begin();
                std::advance(itFirst, i);
                std::advance(itSecond, j);
                Position pos1, pos2, antinode;
                pos1 = *itFirst;
                pos2 = *itSecond;

                //move only in one direction since other direction will be covered when i and j are flipped during for loop
                int directionX = pos2.x - pos1.x;
                int directionY = pos2.y - pos1.y;
                antinode.x = pos1.x;
                antinode.y = pos1.y;

                while (antinode.x >= 0 && antinode.x < grid.size() && antinode.y >= 0 && antinode.y < grid[0].size())
                {
                    antinodes.insert(antinode); //since antennas also count as antinodes
                    antinode.x += directionX;
                    antinode.y += directionY;
                }
            }
        }
    }

    for (const auto &antinode : antinodes)
    {
        std::cout << "Antinode: (" << antinode.x << "," << antinode.y << ") " << std::endl;
        numberOfAntinodesInsideGrid++;
    }

    std::cout << "Number of unique antinode locations within the bounds of the map: " << numberOfAntinodesInsideGrid << std::endl;

    return 0;
}