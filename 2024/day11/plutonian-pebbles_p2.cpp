#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <list> // doubly-linked list - O(1) insertion to n-th position if there
                // is iterator to the position, otherwise O(i)
#include <cmath>
#include <unordered_map>

/**
 * @brief Splits a number into two parts by dividing its digits in half.
 *
 * This function divides the number into two parts from the middle. If the number
 * of digits is odd, the left part will have one more digit than the right part.
 *
 * @param num The number to be split.
 * @return std::pair<int, int> A pair containing the left and right parts of the number.
 */
std::pair<int, int> splitNumber(long long num)
{
    if (num == 0)
        return {0, 0};

    int numDigits = std::log10(num) + 1; // count number of digits -> how many "places" of "powers of 10" fit inside num, +1

    int splitPoint = numDigits / 2; // split in half - if odd, left side will have more numbers
    int divisor = std::pow(10, splitPoint);

    int rightPart = num % divisor;
    int leftPart = num / divisor;

    return {leftPart, rightPart};
}

/**
 * @typedef Pair
 * @brief Defines a type alias for a pair of a stone and a number of blinks.
 *
 * The type is used as a key in the memoization cache.
 */
using Pair = std::pair<long long, int>;

/**
 * @brief Custom hash function for a std::pair to be used as a key in std::unordered_map.
 *
 * This struct defines a custom hash function for pairs since std::pair is not hashable
 * by default. It uses the hash of the first and second elements combined using XOR.
 */
struct pairHash
{
    /**
     * @brief Computes a hash value for a given pair.
     *
     * Combines the hash of the first and second elements of the pair.
     *
     * @tparam T1 The type of the first element of the pair.
     * @tparam T2 The type of the second element of the pair.
     * @param pair The pair to hash.
     * @return std::size_t The computed hash value.
     */
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

/**
 * @brief Memoization cache to store precomputed results.
 *
 * The key is a Pair consisting of (stone, numberOfBlinks), and the value is the
 * number of stones produced after processing the stone for the given number of blinks.
 */
std::unordered_map<Pair, long long, pairHash> cache;

/**
 * @brief Processes a single stone after a number of blinks using memoization.
 *
 * This function processes a stone according to the following rules:
 * - If `numberOfBlinks` is 0, it returns 1.
 * - If the stone is 0, it transforms into 1.
 * - If the number of digits in the stone is even, the stone is split into two parts
 *   and both parts are processed recursively.
 * - If the number of digits in the stone is odd, the stone is multiplied by 2024.
 *
 * The results are stored in a cache to avoid recomputation for identical (stone, numberOfBlinks) pairs.
 *
 * @param stone The stone to be processed.
 * @param numberOfBlinks The number of blinks to process the stone.
 * @return long long The number of stones produced after processing.
 */
long long processStoneAfterBlinks(long long stone, int numberOfBlinks)
{
    Pair key = {stone, numberOfBlinks};
    std::pair<int, int> splittedNum;

    if (cache.find(key) != cache.end())
    {
        return cache[key];
    }

    if (numberOfBlinks == 0)
    {
        return 1;
    }

    if (stone == 0)
    {
        return cache[key] = processStoneAfterBlinks(1, numberOfBlinks - 1);
    }

    if (static_cast<long long>(std::log10(stone) + 1) % 2 == 0)
    {
        splittedNum = splitNumber(stone);
        cache[key] = processStoneAfterBlinks(splittedNum.first, numberOfBlinks - 1) + processStoneAfterBlinks(splittedNum.second, numberOfBlinks - 1);
        return cache[key];
    }

    cache[key] = processStoneAfterBlinks(stone * 2024, numberOfBlinks - 1);
    return cache[key];
}

/**
 * @brief Main function to read input, process stones, and output the final result.
 *
 * The program reads a list of stones from an input file specified as a command-line argument.
 * It then processes each stone according to the rules in `processStoneAfterBlinks`,
 * counting how many total stones exist after a set number of blinks.
 * The final result is printed to the console.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return int Returns 0 if the program executes successfully, otherwise returns 1.
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
    std::list<long long> stones;
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    std::getline(file, line);
    std::istringstream iss(line);
    long long number;
    while (iss >> number)
    {
        stones.push_back(number);
    }

    file.close();

    for (const auto &stone : stones)
    {
        std::cout << stone << " ";
    }
    std::cout << std::endl;

    int numberOfBlinks = 75;

    long long totalNumberOfStones = 0;
    for (const auto &stone : stones)
    {
        totalNumberOfStones += processStoneAfterBlinks(stone, numberOfBlinks);
    }

    std::cout << "Number of stones after blinking " << numberOfBlinks << " times: " << totalNumberOfStones << std::endl;

    return 0;
}