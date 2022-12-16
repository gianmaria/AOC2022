#include "pch.h"

int part_1()
{
    try
    {
#if 0
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        uint32_t count = 0;

        string line;
        while (std::getline(ifs, line))
        {
            std::istringstream iss(line);

            char ignore;

            uint32_t min_a;
            uint32_t max_a;
            uint32_t min_b;
            uint32_t max_b;

            iss >> min_a >> ignore >> max_a >> ignore >> min_b >> ignore >> max_b;

            std::set<uint32_t> set1;
            std::set<uint32_t> set2;

            for (uint32_t i = min_a;
                 i <= max_a;
                 ++i)
            {
                set1.insert(i);
            }

            for (uint32_t i = min_b;
                 i <= max_b;
                 ++i)
            {
                set2.insert(i);
            }

            std::set<uint32_t> intersection;

            std::set_intersection(set1.begin(), set1.end(),
                                  set2.begin(), set2.end(),
                                  std::inserter(intersection, intersection.begin()));

            if (intersection.size() == set1.size() or
                intersection.size() == set2.size())
            {
                ++count;
            }

            int stop = 0;
        }

        cout << "res is: " << count << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}


int main()
{
    try
    {
#if 0
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        uint32_t count = 0;

        string line;
        while (std::getline(ifs, line))
        {
            std::istringstream iss(line);

            char ignore;

            uint32_t min_a;
            uint32_t max_a;
            uint32_t min_b;
            uint32_t max_b;

            iss >> min_a >> ignore >> max_a >> ignore >> min_b >> ignore >> max_b;

            std::set<uint32_t> set1;
            std::set<uint32_t> set2;

            for (uint32_t i = min_a;
                 i <= max_a;
                 ++i)
            {
                set1.insert(i);
            }

            for (uint32_t i = min_b;
                 i <= max_b;
                 ++i)
            {
                set2.insert(i);
            }

            std::set<uint32_t> intersection;

            std::set_intersection(set1.begin(), set1.end(),
                                  set2.begin(), set2.end(),
                                  std::inserter(intersection, intersection.begin()));

            if (intersection.size() > 0)
            {
                ++count;
            }

            int stop = 0;
        }

        cout << "res is: " << count << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}
