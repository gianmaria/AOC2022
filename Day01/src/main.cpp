#include "pch.h"

void part_1()
{
    auto file_path = "res/input.txt";
    cout << "input file: " << file_path << endl;

    auto ifs = std::ifstream(file_path);

    if (not ifs.is_open())
        cout << "Cannot open file " << file_path << endl;

    str line;
    uint64_t acc = 0;
    uint64_t max = 0;

    while (std::getline(ifs, line))
    {
        if (line != "")
        {
            auto res = std::stoul(line);
            acc += res;
        }
        else
        {
            if (acc > max)
            {
                max = acc;
            }

            acc = 0;
        }
    }

    // one last round
    if (acc > max)
    {
        max = acc;
    }

    acc = 0;

    cout << "res: " << max << endl;
}

int main()
{
    //auto file_path = "res/test.txt";
    auto file_path = "res/input.txt";

    cout << "input file: " << file_path << endl;
    auto ifs = std::ifstream(file_path);

    if (not ifs.is_open())
        cout << "Cannot open file " << file_path << endl;

    str line;
    uint64_t acc = 0;
    uint64_t max = 0;
    auto cals = std::multiset<uint64_t, std::greater<uint64_t>>();

    while (std::getline(ifs, line))
    {
        if (line != "")
        {
            auto res = std::stoul(line);
            acc += res;
        }
        else
        {
            cals.insert(acc);

            if (acc > max)
            {
                max = acc;
            }

            acc = 0;
        }
    }

    // one last round
    cals.insert(acc);

    if (acc > max)
    {
        max = acc;
    }

    acc = 0;

    auto res = std::accumulate(cals.begin(), std::next(cals.begin(), 3), 0);

    cout << "res: " << res << endl;

    return 0;
}
