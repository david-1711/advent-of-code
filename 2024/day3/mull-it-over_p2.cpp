#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>

void process_matches(const std::string &substring, int &total_sum)
{
    std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
    std::smatch match;
    auto begin = std::sregex_iterator(substring.begin(), substring.end(), pattern);
    auto end = std::sregex_iterator();
    for (std::sregex_iterator i = begin; i != end; ++i)
    {
        match = *i;
        int num1 = std::stoi(match[1].str());
        int num2 = std::stoi(match[2].str());
        total_sum += num1 * num2;
    }
}

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

    int total_sum = 0;
    size_t pos = 0;
    bool process = true;

    while (pos < data.size())
    {
        size_t dont_pos = data.find("don't()", pos);
        size_t do_pos = data.find("do()", pos);

        if (dont_pos != std::string::npos && (do_pos == std::string::npos || dont_pos < do_pos))
        {
            if (process)
            { // process the data before the first "don't()" substring
                process_matches(data.substr(pos, dont_pos - pos), total_sum);
            }
            process = false;
            pos = dont_pos + 7; // move past "don't()"
        }
        else if (do_pos != std::string::npos)
        {
            if (!process)
            {
                process = true;
                pos = do_pos + 4; // move past "do()"
            }
            else
            {
                process_matches(data.substr(pos, do_pos - pos), total_sum);
                pos = do_pos + 4; // move past "do()"
            }
        }
        else
        {
            if (process)
            {
                process_matches(data.substr(pos), total_sum);
            }
            break;
        }
    }
    std::cout << "The total sum of all multiplications is: " << total_sum << std::endl;

    return 0;
}