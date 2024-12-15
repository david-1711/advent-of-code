#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <list> // doubly-linked list - O(1) insertion to n-th position if there
                // is iterator to the position, otherwise O(i)
#include <cmath>

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
 * @brief Processes the list of stones for a given number of "blinks."
 *
 * Each "blink" changes the values of the stones in the list according to specific rules:
 * - If the stone is 0, it is replaced with 1.
 * - If the number of digits in the stone is even, it is split into two parts
 *   and the right part is inserted into the list after the current position.
 * - If the number of digits in the stone is odd, the stone is multiplied by 2024.
 *
 * The process continues for the given number of blinks.
 *
 * @param stones A list of stones to be processed.
 * @param numberOfBlinks The number of times the transformation process is applied.
 */
void processStonesAfterBlinks(std::list<long long> &stones, uint8_t numberOfBlinks)
{
    std::pair<int, int> splittedNum;

    while (numberOfBlinks > 0)
    {
        for (auto it = stones.begin(); it != stones.end(); ++it)
        {
            if (*it == 0)
            {
                *it = 1;
            }
            else if (static_cast<long long>(std::log10(*it) + 1) % 2 == 0)
            {
                splittedNum = splitNumber(*it);
                *it = splittedNum.first;
                stones.insert(std::next(it), splittedNum.second);
                it++;
            }
            else
            {
                *it *= 2024;
            }
        }
        numberOfBlinks--;
        // for (const auto & stone : stones) {
        //     std::cout << stone << " ";
        // }
        // std::cout << std::endl;
    }
}

/**
 * @brief Main function to read the input file, process the stones, and print the results.
 *
 * This function reads stone values from an input file, processes them using the
 * processStonesAfterBlinks function, and outputs the final number of stones.
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

    int numberOfBlinks = 25;

    processStonesAfterBlinks(stones, numberOfBlinks);

    std::cout << "Number of stones after blinking " << numberOfBlinks << " times: " << stones.size() << std::endl;

    return 0;
}