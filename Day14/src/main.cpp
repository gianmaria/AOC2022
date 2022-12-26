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




constexpr auto HEIGHT = 1000;
constexpr auto WIDTH = 1000;

char map[HEIGHT][WIDTH];
constexpr char ROCK = '#';
constexpr char SAND = 'o';
constexpr char AIR = ' ';

constexpr auto init_map()
{
    for (auto y = 0;
         y < HEIGHT;
         ++y)
    {
        for (auto x = 0;
             x < WIDTH;
             ++x)
        {
            map[y][x] = ' ';
        }
    }

    map[0][500] = '+';
}

void print_map()
{
    std::ofstream ofs("output_map.txt", std::ofstream::trunc);

    for (int y = 0;
         y < HEIGHT;
         ++y)
    {
        for (int x = 0;
             x < WIDTH;
             ++x)
        {
            ofs << map[y][x];
        }
        ofs << endl;
    }
}

void draw_wall(int x1, int y1, int x2, int y2)
{
    if (x1 > x2)
        std::swap(x1, x2);
    if (y1 > y2)
        std::swap(y1, y2);

    if (x1 == x2)
    {
        for (int y = y1;
             y <= y2;
             ++y)
        {
            map[y][x1] = ROCK;
        }
    }
    else if (y1 == y2)
    {
        for (int x = x1;
             x <= x2;
             ++x)
        {
            map[y1][x] = ROCK;
        }
    }
    else
    {
        assert(false);
    }
}

bool pour_sand()
{
    int xs = 500;
    int ys = 0;

    bool sand_to_rest = false;
    while (not sand_to_rest)
    {
        // try down
        if (ys + 1 < HEIGHT and
            map[ys + 1][xs] != ROCK and
            map[ys + 1][xs] != SAND)
        {
            ys += 1;
        }

        // try down left
        else if (ys + 1 < HEIGHT and
                 xs - 1 >= 0 and
                 map[ys + 1][xs - 1] != ROCK and
                 map[ys + 1][xs - 1] != SAND)
        {
            ys += 1;
            xs -= 1;
        }

        // try down right
        else if (ys + 1 < HEIGHT and
                 xs + 1 < WIDTH and
                 map[ys + 1][xs + 1] != ROCK and
                 map[ys + 1][xs + 1] != SAND)
        {
            ys += 1;
            xs += 1;
        }

        else
        {
            map[ys][xs] = SAND;
            sand_to_rest = true;
        }
    }

    if (ys + 1 != HEIGHT)
    {
        return true;
    }
    else
    {
        map[ys][xs] = AIR;
        return false; // we reached the bottom
    }
}

void part_1()
{
#if 0
    auto file_path = "res/test.txt";
#else
    auto file_path = "res/input.txt";
#endif
    cout << "[INFO] input file: " << file_path << endl;

    str file = read_file(file_path);
    auto lines = split_by(file, "\n");

    init_map();

    for (const auto& line : lines)
    {
        auto coords = split_by(line, " -> ");

        for (auto it = coords.begin();
             it < coords.end() - 1;
             ++it)
        {
            auto& coord = (*it);
            auto& coord_next = *(it + 1);

            auto xy = split_by(coord, ",");
            auto xy_next = split_by(coord_next, ",");

            int x1, y1, x2, y2;
            x1 = std::stoi(xy[0]);
            y1 = std::stoi(xy[1]);
            x2 = std::stoi(xy_next[0]);
            y2 = std::stoi(xy_next[1]);

            draw_wall(x1, y1, x2, y2);
        }
    }

    int count = 0;
    while (pour_sand())
    {
        ++count;
    }
    
    print_map();

    cout << "[INFO] res part 1: " << count << endl;
}

void part_2()
{
#if 0
    auto file_path = "res/test.txt";
#else
    auto file_path = "res/input.txt";
#endif
    cout << "[INFO] input file: " << file_path << endl;

    str file = read_file(file_path);

    cout << "[INFO] res part 2: " << endl;
}

int main()
{
    try
    {
        part_1();
        part_2();

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
    }

    return 1;
}
