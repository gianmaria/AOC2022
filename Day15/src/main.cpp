#include "pch.h"

// trim from start (in place)
void ltrim(str& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(str& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(str& s) {
    rtrim(s);
    ltrim(s);
}

vector<str> split_by(str input, str_cref delim)
{
    vector<str> res;

    while (true)
    {
        auto pos = input.find(delim);

        if (pos != str::npos)
        {
            auto tmp = input.substr(0, pos);
            res.push_back(std::move(tmp));

            input = input.substr(pos + delim.size(), input.length());
        }
        else
        {
            res.push_back(std::move(input));
            break;
        }
    }

    return res;
}

str read_file(const char* file_path)
{
    auto ifs = std::ifstream(file_path);

    if (not ifs.is_open())
        throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

    //std::stringstream buffer;
    //buffer << ifs.rdbuf();
    //return buffer.str();

    return str(std::istreambuf_iterator<char>{ifs}, {});
}


struct V2
{
    int x;
    int y;

    auto operator<=>(const V2&) const = default;
};

inline int m_dist(const V2& a, const V2& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct SB
{
    V2 S;
    V2 B;
    int dist = 0;

    SB() = default;

    SB(int x1, int y1, int x2, int y2) :
        S{x1, y1}, B{x2, y2}
    {
        dist = m_dist(S, B);
    }
};



void part1()
{
#if 0
    auto file_path = "res/test.txt";
    const int target_row = 10;

#else
    auto file_path = "res/input.txt";
    const int target_row = 2000000;

#endif

    str file = read_file(file_path);
    auto lines = split_by(file, "\n");

    vector<SB> items;

    int min_x = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();

    int max_dist = 0;

    for (const auto& line : lines)
    {
        int x1, y1, x2, y2;
        sscanf_s(line.c_str(),
                 "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
                 &x1, &y1, &x2, &y2);

        items.emplace_back(x1, y1, x2, y2);

        if (x1 < min_x)
            min_x = x1;
        if (x1 > max_x)
            max_x = x1;

        if (max_dist < items.back().dist)
            max_dist = items.back().dist;
    }

    std::set<V2> res;

    for (int x = min_x - max_dist;
         x <= max_x + max_dist;
         ++x)
    {
        V2 p{x, target_row};

        for (const auto& sb : items)
        {
            int dist = m_dist(p, sb.S);
            if (dist <= sb.dist and
                p != sb.B)
            {
                res.insert(p);
            }
        }
    }

    cout << "part 1: " << res.size() << endl;
}

void part2()
{
#if 0
    auto file_path = "res/test.txt";
    const int target_row = 10;

#else
    auto file_path = "res/input.txt";
    const int target_row = 2000000;

#endif

    str file = read_file(file_path);
    auto lines = split_by(file, "\n");

    cout << "part 2: " << 0 << endl;

}

int main()
{
    try
    {
        part1();
        part2();

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXCEPTION] " << e.what() << endl;
    }

    return 1;
}
