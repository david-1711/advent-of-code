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
 * @brief Struct to represent the position of the guard on the grid.
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
    dir = static_cast<Direction>((dir + 1) % 4);
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
 * @brief Checks if the guard is stuck in a loop.
 *
 * The function tracks the guard's position and direction and checks if this combination
 * has already occurred. If the same (position, direction) pair is encountered again,
 * the guard is stuck in a loop.
 *
 * @param visitedStates A set of previously seen (position, direction) pairs.
 * @param pos The current position of the guard.
 * @param dir The current direction of the guard.
 * @return true if the current (position, direction) pair has been seen before, false otherwise.
 */
bool isStuckInLoop(std::set<std::pair<Position, Direction>> &visitedStates, const Position &pos, const Direction &dir)
{
    std::pair<Position, Direction> state = {pos, dir};
    if (visitedStates.find(state) != visitedStates.end())
    {
        return true; // Loop detected
    }
    visitedStates.insert(state);
    return false;
}

Position findStartPosition(std::vector<std::string> &grid)
{
    Position pos;
    // Find the initial position of the guard
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            if (grid[i][j] == '^')
            {
                pos = {i, j};
                break;
            }
        }
    }
    return pos;
}

/**
 * @brief Navigates the grid according to the given protocol and returns the number of distinct positions visited.
 *
 * The function tracks the guard's position and direction and moves it according to the rules of movement
 * on the grid. It also checks if the guard is stuck in a loop.
 *
 * @param grid The grid representing the map.
 * @return The number of distinct positions visited by the guard.
 */
std::pair<int, bool> navigateGrid(std::vector<std::string> &&grid)
{
    Position startPosition;
    Position pos;
    Direction dir = UP;

    // Find the initial position of the guard
    startPosition = findStartPosition(grid);
    pos = startPosition;
    grid[pos.x][pos.y] = '.'; // Clear the initial position

    std::set<Position> visitedPositions;                    // Stores distinct (x, y) positions
    std::set<std::pair<Position, Direction>> visitedStates; // Stores (position, direction) pairs
    visitedPositions.insert(pos);
    visitedStates.insert({pos, dir});

    while (true)
    {
        Position nextPos = pos;
        moveForward(nextPos, dir);

        // Check if the guard is out of bounds
        if (nextPos.x < 0 || nextPos.x >= grid.size() || nextPos.y < 0 || nextPos.y >= grid[0].size())
        {
            break; // Guard has left the mapped area
        }

        // Check if the position is an obstacle
        if (isObstacle(grid, nextPos.x, nextPos.y))
        {
            turnRight(dir);
        }
        else
        {
            pos = nextPos;
            visitedPositions.insert(pos);
        }

        // Check if the guard is stuck in a loop
        if (isStuckInLoop(visitedStates, pos, dir))
        {
            // std::cout << "Guard is stuck in a loop." << std::endl;
            grid[startPosition.x][startPosition.y] = '^'; // Revert the initial position
            return {visitedPositions.size(), true};
        }
    }

    grid[startPosition.x][startPosition.y] = '^'; // Revert the initial position
    return {visitedPositions.size(), false};
}

/**
 * @brief Prints the 2D grid to the console.
 *
 * This function prints each row of the grid on a new line, providing a visual
 * representation of the current state of the grid.
 *
 * @param grid The 2D grid represented as a vector of strings.
 */
void printGrid(const std::vector<std::string> &grid)
{
    for (const std::string &row : grid)
    {
        std::cout << row << std::endl;
    }
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
    std::pair<int, bool> distPositionsAndIsItLoop;

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

    // distPositionsAndIsItLoop = navigateGrid(std::move(grid));
    // std::cout << "Distinct positions visited: " << distPositionsAndIsItLoop.first << std::endl;

    Position startPosition = findStartPosition(grid);
    std::cout << "Start position (" << startPosition.x << "," << startPosition.y << ")" << std::endl;
    char gridCurrentItem;
    int numberOfValidObstructionLocations = 0;
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            if ((startPosition.x == i && startPosition.y == j) || grid[i][j] == '#')
            {
                continue;
            }
            else
            {
                std::cout << "Current position (" << i << "," << j << ")" << std::endl;
                gridCurrentItem = grid[i][j];
                grid[i][j] = '#';
                // printGrid(grid);
                distPositionsAndIsItLoop = navigateGrid(std::move(grid));
                grid[i][j] = gridCurrentItem; // Revert grid as it was
                if (distPositionsAndIsItLoop.second)
                {
                    std::cout << "Guard is stuck in a loop." << std::endl;
                    // std::cout << "Loop created by adding obstriction at (" << i << "," << j << ")" << std::endl;
                    numberOfValidObstructionLocations++;
                }
            }
        }
    }
    std::cout << "Number of valid positions to create obstruction: " << numberOfValidObstructionLocations << std::endl;

    return 0;
}
