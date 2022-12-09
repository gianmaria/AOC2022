#include "pch.h"

int part_01()
{
    try
    {
#if 0
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif

        cout << "input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("Cannot open file ") + file_path);

        str res;

        //uint32_t counter = 1;
        string line;
        while (std::getline(ifs, line))
        {
            //cout << std::format("line {:02}: '{}'", counter++, line) << endl;
            if (line.length() % 2 != 0)
                throw std::runtime_error("String len is not even");

            str compartment_a = line.substr(0, line.length() / 2);
            str compartment_b = line.substr(line.length() / 2, line.length() - 1);

            std::multiset<char> set1;
            for (char c : compartment_a)
            {
                set1.insert(c);
            }

            std::multiset<char> set2;
            for (char c : compartment_b)
            {
                set2.insert(c);
            }

            std::set<char> intersection;
            std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),
                                  std::inserter(intersection, intersection.begin()));

            if (intersection.size() != 1)
                throw std::runtime_error("Intersection of two sets hold more than one item");

            res.push_back(*intersection.begin());
        }

        uint32_t score = 0;
        for (char c : res)
        {
            if (c >= 'a' and c <= 'z')
                score += c - 'a' + 1;
            else if (c >= 'A' and c <= 'Z')
                score += c - 'A' + 26 + 1;
            else
                throw std::runtime_error("Unknown charater");
        }

        cout << "score is: " << score << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}

char intersection(const std::multiset<char>& set1,
                  const std::multiset<char>& set2,
                  const std::multiset<char>& set3)
{
    std::set<char> intersection1;
    std::set_intersection(set1.begin(), set1.end(),
                          set2.begin(), set2.end(),
                          std::inserter(intersection1, intersection1.end()));

    std::set<char> intersection2;
    std::set_intersection(intersection1.begin(), intersection1.end(),
                           set3.begin(), set3.end(),
                           std::inserter(intersection2, intersection2.end()));

    if (intersection2.size() != 1)
        throw std::runtime_error("Intersection of two sets hold more than one item");

    return *intersection2.begin();
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

        cout << "input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("Cannot open file ") + file_path);

        str res;

        uint32_t counter = 0;
        std::array<std::multiset<char>, 3> sets;

        string line;
        while (std::getline(ifs, line))
        {
            sets[counter].insert(line.begin(), line.end());
            ++counter;

            if (counter % 3 == 0)
            {
                char c = intersection(sets[0], sets[1], sets[2]);
                res.push_back(c);

                sets[0].clear();
                sets[1].clear();
                sets[2].clear();

                counter = 0;
            }
        }

        uint32_t score = 0;
        for (char c : res)
        {
            if (c >= 'a' and c <= 'z')
                score += c - 'a' + 1;
            else if (c >= 'A' and c <= 'Z')
                score += c - 'A' + 26 + 1;
            else
                throw std::runtime_error("Unknown charater");
        }

        cout << "score is: " << score << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}

