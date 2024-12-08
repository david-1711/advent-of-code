#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <fstream>

/**
 * @brief Enum to represent the direction the guard is facing.
 */
enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

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
 * @brief Turns the guard 90 degrees to the right.
 *
 * @param dir The current direction of the guard.
 */
void turnRight(Direction &dir)
{
    dir = static_cast<Direction>((dir + 1) % 4); // ensure after LEFT it is rotated back to UP
}

/**
 * @brief Checks if the given position is an obstacle.
 *
 * @param grid The grid representing the map.
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 * @return true if the position is an obstacle, false otherwise.
 */
bool isObstacle(const std::vector<std::string> &grid, int x, int y)
{
    return grid[x][y] == '#';
}

/**
 * @brief Moves the guard one step forward in the current direction.
 *
 * @param pos The current position of the guard.
 * @param dir The current direction of the guard.
 */
void moveForward(Position &pos, Direction dir)
{
    switch (dir)
    {
    case UP:
        pos.x--;
        break;
    case RIGHT:
        pos.y++;
        break;
    case DOWN:
        pos.x++;
        break;
    case LEFT:
        pos.y--;
        break;
    }
}

/**
 * @brief Navigates the grid according to the given protocol and returns the number of distinct positions visited.
 *
 * @param grid The grid representing the map.
 * @return The number of distinct positions visited by the guard.
 */
int navigateGrid(std::vector<std::string> &grid)
{
    Position pos;
    Direction dir = UP;

    // Find the initial position of the guard
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == '^')
            {
                pos = {i, j};
                grid[i][j] = '.'; // Clear the initial position
                break;
            }
        }
    }

    std::set<Position> visited;
    visited.insert(pos);

    while (true)
    {
        Position nextPos = pos;
        moveForward(nextPos, dir);

        if (nextPos.x < 0 || nextPos.x >= grid.size() || nextPos.y < 0 || nextPos.y >= grid[0].size())
        {
            break; // Guard has left the mapped area
        }

        if (isObstacle(grid, nextPos.x, nextPos.y))
        {
            turnRight(dir);
        }
        else
        {
            pos = nextPos;
            visited.insert(pos);
        }
    }

    return visited.size();
}

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

    int distinctPositions = navigateGrid(grid);
    std::cout << "Distinct positions visited: " << distinctPositions << std::endl;

    return 0;
}
