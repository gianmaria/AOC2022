#include "pch.h"
#include <cassert>


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
    int x = 0;
    int y = 0;
};

bool operator<(const V2& a, const V2& b)
{
    if (a.y == b.y)
    {
        return a.x < b.x;
    }
    else
    {
        return a.y < b.y;
    }
}

bool operator!=(const V2& a, const V2& b)
{
    return (a.x != b.x) and (a.y != b.y);
}

inline int m_dist(const V2& a, const V2& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct Signal
{
    V2 pos;
    V2 bacon;
    int dist_S_B = 0;

    Signal(int x1, int y1, int x2, int y2) :
        pos{x1, y1}, bacon{x2, y2}
    {
        dist_S_B = m_dist(pos, bacon);
    }


    Signal() = default;

    Signal(const Signal& other) = default;
    Signal& operator=(const Signal& other) = default;

    Signal(Signal&& other) = default;
    Signal& operator=(Signal&& other) = default;

    ~Signal() = default;
};

bool operator<(const Signal& a, const Signal& b)
{
    return a.pos < b.pos;
}




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

    vector<Signal> signals;

    int min_x = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();

    int max_dist = 0;

    for (const auto& line : lines)
    {
        int x1, y1, x2, y2;
        sscanf_s(line.c_str(),
                 "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
                 &x1, &y1, &x2, &y2);

        signals.emplace_back(x1, y1, x2, y2);

        if (x1 < min_x)
            min_x = x1;
        if (x1 > max_x)
            max_x = x1;

        if (max_dist < signals.back().dist_S_B)
            max_dist = signals.back().dist_S_B;
    }

    std::set<V2> res;

    for (int x = min_x - max_dist;
         x <= max_x + max_dist;
         ++x)
    {
        V2 p{x, target_row};

        for (const auto& sb : signals)
        {
            int dist = m_dist(p, sb.pos);
            if (dist <= sb.dist_S_B and
                p != sb.bacon)
            {
                res.insert(p);
            }
        }
    }

    cout << "part 1: " << res.size() << endl;
}

auto constexpr ROWS = 50;
auto constexpr COLS = 50;
char map[ROWS][COLS];

void init_map()
{
    memset(map, ' ', ROWS * COLS);
}

void print_map()
{
    std::ofstream ofs("output_map.txt", std::ofstream::trunc);

    ofs << " ";
    for (int c = 0;
         c < COLS;
         ++c)
    {
        ofs << std::format("{}", std::abs(c - 10) % 10);
    }
    ofs << endl;

    for (int r = 0;
         r < ROWS;
         ++r)
    {
        ofs << std::format("{}", std::abs(r - 10) % 10);
        for (int c = 0;
             c < COLS;
             ++c)
        {
            ofs << map[r][c];
        }
        ofs << endl;
    }
}

void put_pixel(int x, int y, int offset, char ch)
{
    map[y + offset][x + offset] = ch;
}

void part2()
{
#if 0
    auto file_path = "res/test.txt";
    int limit = 20;
#define DEBUG_DRAW 1

#else
    auto file_path = "res/input.txt";
    int limit = 4'000'000;
#define DEBUG_DRAW 0

#endif

    str file = read_file(file_path);
    auto lines = split_by(file, "\n");

    int min_x = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int min_y = std::numeric_limits<int>::max();
    int max_y = std::numeric_limits<int>::min();

    int max_dist = 0;
    vector<Signal> signals;

    for (const auto& line : lines)
    {
        int x1, y1, x2, y2;
        sscanf_s(line.c_str(),
                 "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
                 &x1, &y1, &x2, &y2);

        auto sb = Signal{x1, y1, x2, y2};

        if (x1 < min_x)
            min_x = x1;
        if (x1 > max_x)
            max_x = x1;

        if (y1 < min_y)
            min_y = y1;
        if (y1 > max_y)
            max_y = y1;

        if (max_dist < sb.dist_S_B)
            max_dist = sb.dist_S_B;

        signals.push_back(std::move(sb));
    }

#if DEBUG_DRAW
    init_map();
#endif // DEBUG_DRAW


    int offset = max_dist;

    //signals.clear();
    //signals.emplace_back(-1,-2, -3,-4);
    //signals.emplace_back(17,0, 17,-8);
    //signals.emplace_back(8,8, 8,25);

    // draw area of search
#if 0
    for (u64 i = 0;
         i < signals.size();
         ++i)
    {
        const auto& signal = items[i];

        for (int r = 0;
             r < ROWS;
             ++r)
        {
            for (int c = 0;
                 c < COLS;
                 ++c)
            {
                V2 p{c, r};

                V2 S = signal.S;
                S.x += offset;
                S.y += offset;

                if (m_dist(p, S) <= sb.dist_S_B)
                {
                    map[r][c] = '#';
                }
            }
        }
    }
#endif // 0

    vector<V2> candidates;
    candidates.reserve(signals.size() * max_dist * 4);
    // draw perimeter searching area
    for (const auto& signal : signals)
    {
        V2 S = signal.pos;
        int dist_S_B = signal.dist_S_B;

        V2 lu, ru, ld, rd;
        for (int i = 0;
             i <= dist_S_B;
             ++i)
        {
            lu = {S.x - dist_S_B + i, S.y - i};
            ru = {S.x + dist_S_B - i, S.y - i};
            ld = {S.x - dist_S_B + i, S.y + i};
            rd = {S.x + dist_S_B - i, S.y + i};

            candidates.emplace_back(lu.x - 1, lu.y);
            candidates.emplace_back(ru.x + 1, ru.y);
            candidates.emplace_back(ld.x - 1, ld.y);
            candidates.emplace_back(rd.x + 1, rd.y);

#if DEBUG_DRAW
            char ch = '.';
            put_pixel(lu.x, lu.y, offset, ch);
            put_pixel(ru.x, ru.y, offset, ch);
            put_pixel(ld.x, ld.y, offset, ch);
            put_pixel(rd.x, rd.y, offset, ch);

            char per = '|';
            put_pixel(candidates[candidates.size() - 4].x, candidates[candidates.size() - 4].y, offset, per);
            put_pixel(candidates[candidates.size() - 3].x, candidates[candidates.size() - 3].y, offset, per);
            put_pixel(candidates[candidates.size() - 2].x, candidates[candidates.size() - 2].y, offset, per);
            put_pixel(candidates[candidates.size() - 1].x, candidates[candidates.size() - 1].y, offset, per);
#endif // DEBUG_DRAW

        }
        // break;
    }

    V2 target;
    for (const auto& cand_point : candidates)
    {
        int match = 0;

        bool valid_point =
            cand_point.x >= 0 and cand_point.x <= limit
            and
            cand_point.y >= 0 and cand_point.y <= limit;

        if (not valid_point)
            continue;

        for (const auto& signal : signals)
        {
            if (m_dist(signal.pos, cand_point) > signal.dist_S_B)
            {
                ++match;
            }
        }

        if (match == signals.size())
        {
            target = {cand_point.x, cand_point.y};
            //cout << "FOUND " << "(" << target.x << "," << target.y << ")" << endl;
#if DEBUG_DRAW
            put_pixel(target.x, target.y, offset, '!');
#endif // DEBUG_DRAW
            break;
        }
    }

    // draw S and B
#if DEBUG_DRAW
    for (const auto& signal : signals)
    {
        put_pixel(signal.pos.x, signal.pos.y, offset, 'S');
        put_pixel(signal.bacon.x, signal.bacon.y, offset, 'B');
    }
#endif // 0

#if DEBUG_DRAW
    print_map();
#endif // DEBUG_DRAW


    u64 sol = 
        static_cast<u64>(target.x) 
        * 4000000ULL 
        + static_cast<u64>(target.y);

    cout << "part 2: " << sol << endl;
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
