#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <unordered_map>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_data>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);
    std::vector<int> column1, column2, distance;
    std::string line;
    int sumOfDistances, sumOfSimilarityScores;

    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        int num1, num2;
        if (iss >> num1 >> num2)
        {
            column1.push_back(num1);
            column2.push_back(num2);
        }
    }

    file.close();

    std::sort(column1.begin(), column1.end());
    std::sort(column2.begin(), column2.end());

    for (size_t i = 0; i < column1.size(); ++i)
    {
        distance.push_back(std::abs(column1[i] - column2[i]));
    }

    sumOfDistances = std::accumulate(distance.begin(), distance.end(), 0);

    std::unordered_map<int, int> count_map;

    for (int num : column2)
    {
        count_map[num]++;
    }

    std::vector<int> similarityScores;

    for (int num : column1)
    {
        similarityScores.push_back(num * count_map[num]);
    }

    sumOfSimilarityScores = std::accumulate(similarityScores.begin(), similarityScores.end(), 0);

    std::cout << "Sum of distances: " << sumOfDistances << std::endl;
    std::cout << "Sum of similarity scores: " << sumOfSimilarityScores << std::endl;

    return 0;
}
