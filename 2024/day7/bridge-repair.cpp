#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

/**
 * @brief Removes the last element from the vector and returns the modified vector.
 *
 * This function takes a vector, removes the last element (if the vector is not empty),
 * and returns the modified vector.
 *
 * @param vec A vector of integers from which the last element will be removed.
 * @return A vector with the last element removed.
 */
std::vector<int> removeLastElement(std::vector<int> vec)
{
    if (!vec.empty())
        vec.pop_back();
    return vec;
}

/**
 * @brief Checks if the equation can be made true by using elements from the list of numbers.
 *
 * This function recursively checks if the `testValue` can be obtained by applying division
 * or subtraction by using the elements in `listOfNumbers`. The function
 * keeps calling itself, removing the last element from the list, until it either finds a
 * solution or exhausts the list.
 *
 * @param testValue The value to check if it can be obtained.
 * @param listOfNumbers A vector of numbers to use in operations to obtain the test value.
 *
 * @return `true` if the `testValue` can be obtained, otherwise `false`.
 */
bool checkIfEquationCanBeTrue(const long long testValue, const std::vector<int> listOfNumbers)
{
    // Base case: if the list contains only one element, check if it equals the test value
    if (listOfNumbers.size() == 1)
    {
        return (testValue == listOfNumbers[0]);
    }

    // Recursive case: try dividing by the last number in the list
    if ((testValue % listOfNumbers[listOfNumbers.size() - 1] == 0) &&
        checkIfEquationCanBeTrue((testValue / listOfNumbers[listOfNumbers.size() - 1]), removeLastElement(listOfNumbers)))
    {
        return true;
    }

    // Recursive case: try subtracting the last number from the test value
    if ((testValue > listOfNumbers[listOfNumbers.size() - 1]) &&
        checkIfEquationCanBeTrue((testValue - listOfNumbers[listOfNumbers.size() - 1]), removeLastElement(listOfNumbers)))
    {
        return true;
    }

    return false;
}

/**
 * @brief Main function that processes input data and calculates the sum of equations that can
 *        possibly be true.
 *
 * This function reads input from a file, parses it, and checks if the equation can be made true
 * by using elements from the list of numbers. It calculates the total sum of the values for which
 * the condition is true.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments, where the first argument is the input file name.
 * @return 0 if the program executes successfully, 1 if there is an error in file handling or input.
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
    std::map<long long, std::vector<int>> data;
    std::string line;
    long long sumOfTrueEquations = 0;

    if (!file.is_open())
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
        {
            std::cerr << "Invalid line format: " << line << std::endl;
            continue;
        }

        std::string keyPart = line.substr(0, colonPos);
        long long key = std::stoll(keyPart);

        std::string valuesPart = line.substr(colonPos + 1);
        valuesPart.erase(0, valuesPart.find_first_not_of(" "));

        std::stringstream ss(valuesPart);
        std::vector<int> values;
        int value;
        while (ss >> value)
        {
            values.push_back(value);
        }

        data[key] = values;
    }

    file.close();

    std::cout << "Parsed data:" << std::endl;
    for (const auto &pair : data)
    {
        std::cout << pair.first << ": ";
        for (int value : pair.second)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;

        if (checkIfEquationCanBeTrue(pair.first, pair.second))
        {
            std::cout << "OK" << std::endl;
            sumOfTrueEquations += pair.first;
        }
    }

    std::cout << "Total calibration result: " << sumOfTrueEquations << std::endl;

    return 0;
}
