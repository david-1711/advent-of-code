#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

bool isAscendingOrDescendingWithProperDifference(std::vector<int> vector, int elementToSkip = 99)
{
    int diff;
    std::string direction;
    for (size_t i = 1; i < vector.size(); ++i)
    {
        if (i == elementToSkip)
        {
            continue;
        }

        if (vector[i] == vector[i - 1])
            return false;

        if (direction.empty())
        {
            if (vector[i] > vector[i - 1]) // ascending
            {
                direction = "ascending";
            }
            else
            {
                direction = "descending";
            }
        }

        diff = std::abs(vector[i] - vector[i - 1]);

        if (direction.compare("ascending") == 0)
        {
            if (elementToSkip != i - 1)
            {
                if (vector[i] < vector[i - 1])
                    return false;
            }
            else
            { // usecase 52 53 54 56 59 61 65 63
                if (vector[i] < vector[i - 2])
                    return false;
            }
        }
        else if (direction.compare("descending") == 0)
        {
            if (elementToSkip != i - 1)
            {
                if (vector[i] > vector[i - 1])
                    return false;
            }
            else
            {
                if (vector[i] > vector[i - 2])
                    return false;
            }
        }

        if (diff < 1 || diff > 3)
            return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_data>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);
    std::vector<std::vector<int>> data;
    std::string line;
    uint32_t number_of_safe_reports = 0;
    uint32_t number_of_safe_reports_with_problem_dampener = 0;

    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::vector<int> numbers;
        int number;
        while (iss >> number)
        {
            numbers.push_back(number);
        }
        data.push_back(numbers);
    }

    file.close();

    for (const auto &line : data)
    {
        if (isAscendingOrDescendingWithProperDifference(line))
        {
            number_of_safe_reports++;
        }
        else
        {
            for (size_t i = 0; i < line.size(); ++i)
            {
                if (isAscendingOrDescendingWithProperDifference(line, i))
                {
                    number_of_safe_reports_with_problem_dampener++;
                    break;
                }
            }
        }
    }

    std::cout << "Number of reports: " << data.size() << std::endl;
    std::cout << "Number of safe reports: " << number_of_safe_reports << std::endl;
    std::cout << "Number of safe reports when using Problem Dampener: " << number_of_safe_reports + number_of_safe_reports_with_problem_dampener << std::endl;

    return 0;
}