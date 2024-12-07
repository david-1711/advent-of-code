#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>

int main()
{
    std::ifstream file("input_data");

    if (!file.is_open())
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string data = buffer.str();

    file.close();

    std::cout << "Memory data:" << data << std::endl;

    std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
    std::vector<std::string> parsed_data;

    auto begin = std::sregex_iterator(data.begin(), data.end(), pattern);
    auto end = std::sregex_iterator();
    std::smatch match1, match2;

    for (std::sregex_iterator i = begin; i != end; ++i)
    {
        match1 = *i;
        parsed_data.push_back(match1.str());
    }

    int total_sum = 0;

    for (const auto &s : parsed_data)
    {
        if (std::regex_match(s, match2, pattern))
        {
            // LOG("Match1:", match2[0].str());
            int num1 = std::stoi(match2[1].str());
            int num2 = std::stoi(match2[2].str());
            total_sum += num1 * num2;
        }
    }

    std::cout << "The total sum of all multiplications is: " << total_sum << std::endl;

    return 0;
}