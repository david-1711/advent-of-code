#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Structure to hold a pair of integers.
 */
struct Pair {
    int x;
    int y;
};

/**
 * @brief Finds the index of a number in a vector.
 * 
 * @param vector The vector to search in.
 * @param number The number to find.
 * @return The index of the number if found, otherwise -1.
 */
int find_number_in_vector(const std::vector<int>& vector, int number) {
    auto it = std::find(vector.begin(), vector.end(), number);
    if (it != vector.end()){
        return std::distance(vector.begin(), it);
    }
    else {
        return -1;
    }
}

/**
 * @brief Validates an list of integers based on rules defined as pair of two integers.
 *        The first integer of a pair must always come after second pair in list provided as
 *        the first argument.
 * 
 * @param pairs A vector of Pair structures representing the rules.
 * @param update A vector of integers representing the list of integers to validate.
 * @return 0 if the update is invalid based on the rules, otherwise the middle element of the update.
 */
int validate_update_based_on_pairs(const std::vector<Pair> &pairs, const std::vector<int> &update) {
    for (const auto & pair : pairs) {
        int indexOfX = find_number_in_vector(update, pair.x);
        int indexOfY = find_number_in_vector(update, pair.y);
        if ((indexOfX > indexOfY) && indexOfY != -1) {
            return 0;
        }
    }

    return update[update.size() / 2];
}

/**
 * @brief Parses rules into a map for easier access.
 * 
 * @param rules A vector of Pair structures representing the rules.
 * @param ruleMap An unordered map where each key is an element(rule) and the value is a set of elements that must come after the key.
 */
void parse_rules(const std::vector<Pair>& rules, std::unordered_map<int, std::unordered_set<int>>& ruleMap) {
    for (const auto& pair : rules) {
        ruleMap[pair.x].insert(pair.y);
    }
}

/**
 * @brief Applies rules to a list using sorting.
 * 
 * @param ruleMap An unordered map where each key is an element and the value is a set of elements that must come after the key.
 * @param update A vector of integers to be sorted based on the rules.
 */
void apply_rules(const std::unordered_map<int, std::unordered_set<int>>& ruleMap, std::vector<int>& update) {
    auto comparator = [&] (int x, int y) {
        if (ruleMap.count(x) && ruleMap.at(x).count(y)) {
            return true; // x must come before y
        }
        if (ruleMap.count(y) && ruleMap.at(y).count(x)) {
            return false; // y must come before x
        }
        return x < y; // default comparison
    };

    std::sort(update.begin(), update.end(), comparator);
}

/**
 * @brief Applies rules to a list using an alternative method.
 * 
 * @param ruleMap An unordered map where each key is an element and the value is a set of elements that must come after the key.
 * @param update A vector of integers to be sorted based on the rules.
 */
void apply_rules_alternative(const std::unordered_map<int, std::unordered_set<int>>& ruleMap, std::vector<int>& update) {
    auto satisfiesRules = [&] (int x, int y) {
        return ruleMap.count(x) == 0 || ruleMap.at(x).count(y) == 0; //there is no rule for x, or for rule x there is not y which comes after it
    };

    for (size_t i = 0; i < update.size(); ++i) {
        for (size_t j = i + 1; j < update.size(); ++j) { //every next number in the update vector
            if (!satisfiesRules(update[i], update[j])) {
                std::swap(update[i], update[j]);
            }
        }
    }
}

/**
 * @brief Main function to read input data, parse rules, validate updates, and apply rules.
 * 
 * This function reads pairs and updates from an input file, parses the rules into a map,
 * validates the updates based on the rules, and applies the rules to fix invalid updates.
 * It then calculates and prints the sums of valid and invalid updates.
 * 
 * @return 0 on successful execution, 1 if the input file cannot be opened.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_data>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open the file";
        return 1;
    }

    std::vector<Pair> rules;
    std::vector<std::vector<int>> updates;
    std::string line;
    int sumOfValidUpdates = 0;
    int sumOfInValidUpdates = 0;
    int sumOfMiddleElementsOfValidUpdates = 0;
    int sumOfMiddleElementsOfFixedUpdates = 0;
    std::unordered_map<int, std::unordered_set<int>> ruleMap;

    while(getline(file, line) && ! line.empty()) {
        std::istringstream iss(line);
        Pair p;
        char delimiter;
        if(iss >> p.x >> delimiter >> p.y) {
            rules.push_back(p);
        }
    }

    while(std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> numbers;
        std::string num;
        while(std::getline(iss, num, ',')) {
            numbers.push_back(std::stoi(num));
        }
        updates.push_back(numbers);
    }

    file.close();

    // std::cout << "Pairs: " << std::endl;
    // for (const auto& pair : rules) {
    //     std::cout << pair.x << "|" << pair.y << std::endl;
    // }

    parse_rules(rules, ruleMap);

    // std::cout << "Updates: " << std::endl;
    for (auto & update : updates) { //update var not set to const so apply_rules can modify it
        // for(int num : update) {
        //     std::cout << num << " ";
        // }
        // std::cout << std::endl;
        int ret = validate_update_based_on_pairs(rules, update);
        if (ret != 0) {
            //std::cout << "OK" << std::endl;
            sumOfValidUpdates++;
            sumOfMiddleElementsOfValidUpdates += validate_update_based_on_pairs(rules, update);
        }
        else{
            // for(int num : update) {
            //     std::cout << num << " ";
            // }
            // std::cout << "NOK" << std::endl;
            sumOfInValidUpdates++;
            //sumOfMiddleElementsOfFixedUpdates += fix_invalid_updates(rules, update);
            apply_rules(ruleMap, update);
            sumOfMiddleElementsOfFixedUpdates += update[update.size() / 2];
        }
    }

    std::cout << "Sum of valid updates: " << sumOfValidUpdates << std::endl;
    std::cout << "Sum of middle elements fo valid updates: " << sumOfMiddleElementsOfValidUpdates << std::endl;

    std::cout << "Sum of invalid updates: " << sumOfInValidUpdates << std::endl;
    std::cout << "Sum of middle elements fo fixed updates: " << sumOfMiddleElementsOfFixedUpdates << std::endl;
}