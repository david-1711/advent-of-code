#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype> //isdigit
#include <map>

/**
 * @brief Converts a character to its corresponding integer value.
 * 
 * @param c The character to convert.
 * @return The integer value of the character if it is a digit, otherwise -1.
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
 * @brief Expands a disk map into a detailed view of files and free blocks.
 * 
 * @param diskMap A vector representing the disk map with file and free block sizes.
 * @return A pair containing a map of file indices to their positions and lengths, and a vector of free blocks.
 */
std::pair<std::map<int, std::pair<int, int>>, std::vector<std::pair<int, int>>> expandMapView(const std::vector<long long> &diskMap)
{
    std::map<int, std::pair<int, int>> expandedDiskMap;
    std::vector<std::pair<int, int>> freeBlocks;
    size_t fileIndex = 0;
    size_t filePosition = 0;
    size_t fileLenght;
    for (size_t i = 0; i < diskMap.size(); ++i)
    {
        fileLenght = diskMap[i];
        if (i == 0 || i % 2 == 0)
        {
            if (fileLenght == 0)
            {
                std::cerr << "ERROR: File of size 0 found." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            expandedDiskMap[fileIndex] = {filePosition, fileLenght};
            fileIndex++;
        }
        else
        {
            if (fileLenght != 0)
            {
                freeBlocks.push_back({filePosition, fileLenght});
            }
        }
        filePosition += fileLenght;
    }
    return {expandedDiskMap, freeBlocks};
}

/**
 * @brief Rearranges files in memory to minimize fragmentation and calculates a checksum.
 * 
 * This function takes an expanded map view of files and free blocks, rearranges the files to minimize fragmentation,
 * and calculates a checksum based on the new positions of the files. The function ensures that files are moved to
 * the earliest available free blocks that can accommodate them, and updates the free blocks accordingly.
 * 
 * @param expandedMapView A reference to a pair containing:
 *  - A map where the key is the file index and the value is a pair representing the file's starting position and length.
 *  - A vector of pairs representing the starting position and length of free blocks.
 * 
 * @return The checksum calculated from the file indices and their new positions.
 * 
 * The function works as follows:
 * 1. Initializes the checksum to 0.
 * 2. Iterates over the files in reverse order (from the last file to the first).
 * 3. For each file, it finds the earliest free block that can accommodate the file.
 * 4. Moves the file to the new position and updates the free block list.
 * 5. If a free block is completely used, it is removed from the list; otherwise, it is updated to reflect the remaining free space.
 * 6. After all files are moved, the function calculates the checksum by iterating over the files and summing the product of the file index and each position within the file's range.
 */
long long fragmentMemory(std::pair<std::map<int, std::pair<int, int>>, std::vector<std::pair<int, int>>> &expandedMapView)
{
    long long checksum = 0;

    int fileIndex = expandedMapView.first.size();
    size_t filePosition = 0;
    size_t fileLenght = 0;

    while (fileIndex > 0)
    {
        fileIndex--;
        filePosition = expandedMapView.first.at(fileIndex).first;
        fileLenght = expandedMapView.first.at(fileIndex).second;

        size_t freeBlStart;
        size_t freeBlLenght;
        for (size_t i = 0; i < expandedMapView.second.size(); ++i)
        {
            freeBlStart = expandedMapView.second.at(i).first;
            freeBlLenght = expandedMapView.second.at(i).second;
            if (freeBlStart >= filePosition)
            {
                do
                {
                    expandedMapView.second.pop_back();
                } while ((expandedMapView.second.size()) > i);
                break;
            }
            if (fileLenght <= freeBlLenght)
            {
                expandedMapView.first.at(fileIndex).first = freeBlStart;
                expandedMapView.first.at(fileIndex).second = fileLenght;
                if (fileLenght == freeBlLenght)
                {
                    expandedMapView.second.erase(expandedMapView.second.begin() + i);
                }
                else
                {
                    expandedMapView.second.at(i) = {freeBlStart + fileLenght, freeBlLenght - fileLenght};
                }
                break;
            }
        }
    }

    for (const auto &[fileIndex, posAndLenght] : expandedMapView.first)
    {
        for (size_t i = posAndLenght.first; i < (posAndLenght.first + posAndLenght.second); ++i)
        {
            checksum += fileIndex * i;
        }
    }
    std::cout << std::endl;
    return checksum;
}

/**
 * @brief Main function to process input data and calculate the filesystem checksum.
 * 
 * This function reads input data from a file, processes it to expand the disk map,
 * and calculates the filesystem checksum.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments. The first argument should be the input file name.
 * @return Returns 0 on success, or 1 on error.
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
    std::pair<std::map<int, std::pair<int, int>>, std::vector<std::pair<int, int>>> expandedDiskMap;

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

    // for (const auto &[fileIndex, posAndLenght] : expandedDiskMap.first)
    // {
    //     std::cout << fileIndex << ": (" << posAndLenght.first << "," << posAndLenght.second << ") ";
    // }

    // std::cout << std::endl;

    // for (const auto &[freeBlockIndex, lenght] : expandedDiskMap.second)
    // {
    //     std::cout << "(" << freeBlockIndex << "," << lenght << ") ";
    // }
    // std::cout << std::endl;

    checksum = fragmentMemory(expandedDiskMap);

    // for (const auto &num : expandedDiskMap)
    // {
    //     std::cout << num << " ";
    // }

    // std::cout << std::endl;

    std::cout << "Filesystem checksum: " << checksum << std::endl;

    return 0;
}