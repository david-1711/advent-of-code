#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype> //isdigit

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
 * @brief Expands a disk map view.
 *
 * This function expands a compressed disk map view into a full representation.
 * Even indices in the input vector represent the number of blocks with a specific ID,
 * while odd indices represent the number of empty blocks (-1).
 *
 * @param diskMap The compressed disk map view.
 * @return The expanded disk map view.
 */
std::vector<long long> expandMapView(const std::vector<long long> &diskMap)
{
    std::vector<long long> expandedDiskMap;
    size_t idNumber = 0;
    for (size_t i = 0; i < diskMap.size(); ++i)
    {
        if (i == 0 || i % 2 == 0)
        {
            for (size_t j = 0; j < diskMap[i]; ++j)
            {
                expandedDiskMap.push_back(idNumber);
            }
            idNumber++;
        }
        else
        {
            for (size_t j = 0; j < diskMap[i]; ++j)
            {
                expandedDiskMap.push_back(-1);
            }
        }
    }
    return expandedDiskMap;
}

/**
 * @brief Fragments memory and calculates a checksum.
 *
 * This function simulates memory fragmentation by moving non-empty blocks to fill gaps.
 * It also calculates a checksum based on the positions and values of the blocks.
 *
 * @param expandedMapView The expanded disk map view.
 * @return The calculated checksum.
 */
long long fragmentMemory(std::vector<long long> &expandedMapView)
{
    long long checksum = 0;
    for (size_t i = 0; i < expandedMapView.size(); ++i)
    {
        if (expandedMapView[i] == -1)
        {
            int lastIndexBeforePop = i - 1;
            while (expandedMapView[i] == -1)
            {
                expandedMapView[i] = expandedMapView.back();
                if ((lastIndexBeforePop + 1) == expandedMapView.size())
                    break;
                expandedMapView.pop_back();
            }
        }
    }

    for (size_t i = 0; i < expandedMapView.size(); ++i)
    {
        std::cout << expandedMapView[i] << " ";
        checksum += i * expandedMapView[i];
    }
    std::cout << std::endl;
    return checksum;
}

/**
 * @brief Main function that processes a disk map file.
 *
 * The main function reads a disk map from a file, expands it, fragments the memory,
 * and calculates a checksum. It prints the expanded map and the checksum.
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
    std::vector<long long> data;
    char numberChar;
    long long number;
    long long checksum;
    std::vector<long long> expandedDiskMap;

    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    while (file.get(numberChar))
    {
        number = charToInt(numberChar);
        data.push_back(number);
    }

    file.close();

    expandedDiskMap = expandMapView(data);

    for (const auto &num : expandedDiskMap)
    {
        std::cout << num << " ";
    }

    std::cout << std::endl;

    checksum = fragmentMemory(expandedDiskMap);

    for (const auto &num : expandedDiskMap)
    {
        std::cout << num << " ";
    }

    std::cout << std::endl;

    std::cout << "Filesystem checksum: " << checksum << std::endl;

    return 0;
}