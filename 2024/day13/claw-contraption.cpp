#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <climits> //LLONG_MAX

/**
 * @brief Calculates the minimum number of tokens required to win all possible prizes.
 *
 * This function iterates through all possible combinations of button presses (i and j) to find the minimum number of tokens
 * required to satisfy the linear equations ax * i + bx * j == px and ay * i + by * j == py for each group of numbers.
 *
 * @param numberGroups A vector of vectors containing the coefficients and constants for the equations.
 * Each inner vector should contain six elements: {ax, ay, bx, by, px, py}.
 * @return The minimum number of tokens required to satisfy the conditions for all groups.
 */
long long calculateMinNumberOfTokens(const std::vector<std::vector<long long>> &numberGroups)
{
    int ax, ay, bx, by;
    long long px, py;
    long long minTokensSum = 0;
    for (const auto &numList : numberGroups)
    {
        long long minTokens = LLONG_MAX;
        ax = numList[0];
        ay = numList[1];
        bx = numList[2];
        by = numList[3];
        px = numList[4];
        py = numList[5];
        for (int i = 0; i < 101; ++i)
        {
            for (int j = 0; j < 101; j++)
            {
                if (ax * i + bx * j == px && ay * i + by * j == py)
                {
                    minTokens = std::min(minTokens, static_cast<long long>(i * 3 + j));
                }
            }
        }
        if (minTokens != LLONG_MAX)
        {
            minTokensSum += minTokens;
        }
    }
    return minTokensSum;
}

/**
 * @brief Calculates the minimum number of tokens required to win all possible prizes.
 *
 * @param numberGroups A vector of vectors containing the coefficients and constants for the equations.
 * Each inner vector should contain six elements: {ax, ay, bx, by, px, py}.
 * @return The minimum number of tokens required to satisfy the conditions for all groups.
 */
long long calculateMinNumberOfTokensOptimized(const std::vector<std::vector<long long>> &numberGroups)
{
    int ax, ay, bx, by;
    long long px, py;
    long long minTokensSum = 0;
    long long prizeIncrease = 10000000000000;
    // long long prizeIncrease = 0; //uncomment to produce solution of part 1
    for (const auto &numList : numberGroups)
    {
        ax = numList[0];
        ay = numList[1];
        bx = numList[2];
        by = numList[3];
        px = numList[4] + prizeIncrease;
        py = numList[5] + prizeIncrease;

        long long determinant = ax * by - ay * bx;

        if (determinant != 0)
        {
            // after resolving 2 linear equations with 2 unknowns: ax * i + bx * j = px , ay * i + by * j = py
            // we can get count for number of presses of button A and button B
            long long countOfAPresses = (px * by - py * bx) / determinant;
            long long countOfBPresses = (px - ax * countOfAPresses) / bx;

            if ((px * by - py * bx) % determinant == 0 && (px - ax * countOfAPresses) % bx == 0)
            {
                std::cout << "P " << countOfAPresses << " " << countOfBPresses << std::endl;
                minTokensSum += countOfAPresses * 3 + countOfBPresses;
            }
        }
    }
    return minTokensSum;
}

/**
 * @brief Main function to read input data and calculate minimum number of tokens.
 *
 * This function reads input data from a file specified by the command line argument,
 * processes the data to extract groups of numbers, and calculates the minimum number
 * of tokens required to win all possible prizes.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return int Returns 0 on successful execution, 1 on error.
 *
 **/
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_data>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);
    std::string line;
    std::regex numbersRegex(R"(\d+)");
    std::vector<std::vector<long long>> numberGroups;
    std::vector<long long> numbers;
    long long minNumberOfTokens = 0;
    long long minNumberOfTokensP2 = 0;

    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            if (!numbers.empty())
            {
                numberGroups.push_back(numbers);
                numbers.clear();
            }
        }
        else
        {
            auto begin = std::sregex_iterator(line.begin(), line.end(), numbersRegex);
            auto end = std::sregex_iterator();

            for (std::regex_iterator i = begin; i != end; ++i)
            {
                numbers.push_back(std::stoll((*i).str()));
            }
        }
    }
    if (!numbers.empty())
    {
        numberGroups.push_back(numbers);
    }
    file.close();

    for (const auto &numList : numberGroups)
    {
        for (const auto &num : numList)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    minNumberOfTokens = calculateMinNumberOfTokens(numberGroups);

    std::cout << "The fewest tokens you would have to spend to will all posible prizes is(Part 1): " << minNumberOfTokens << std::endl;

    minNumberOfTokensP2 = calculateMinNumberOfTokensOptimized(numberGroups);

    std::cout << "The fewest tokens you would have to spend to will all posible prizes is(Part 2): " << minNumberOfTokensP2 << std::endl;

    return 0;
}
