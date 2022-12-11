#include "pch.h"


struct V2
{
    V2() : x(0), y(4)
    {}

    V2(i32 x, i32 y) : x(x), y(y)
    {}

    i32 x;
    i32 y;

    u32 len()
    {
        return static_cast<u32>(std::sqrt(x * x + y * y));
    }
};

V2 operator-(const V2& a, const V2& b)
{
    return { a.x - b.x, a.y - b.y };
}

bool operator==(const V2& a, const V2& b)
{
    return a.x == b.x and a.y == b.y;
}

struct V2_Comparator
{
    bool operator()(const V2& a, const V2& b) const
    {
        if (a.x != b.x) {
            return a.x < b.x;
        }
        else {
            return a.y < b.y;
        }
    }
};

template<u32 N>
struct Rope
{
    Rope()
    {
        m_visited_by_tail.emplace(*(m_knots.end() - 1));
        m_threshold = 1;
    }

    void move(char dir)
    {
        auto& head = m_knots[0];
        auto& tail = m_knots[N - 1];

        switch (dir)
        {
        case 'U':
        {
            head.y -= 1;
        } break;

        case 'D':
        {
            head.y += 1;
        } break;

        case 'L':
        {
            head.x -= 1;
        } break;

        case 'R':
        {
            head.x += 1;
        } break;

        default:
            throw std::runtime_error("unknown direction");
            break;
        }

        for (u32 curr = 0, next = 1;
            next < N;
            ++curr, ++next)
        {
            m_knots[next] = update_tail_pos(m_knots[curr], m_knots[next]);
        }

        m_visited_by_tail.emplace(tail);
    }

    void print()
    {
        u32 width = 6;
        u32 height = 5;

        for (u32 y = 0; y < height; ++y)
        {
            for (u32 x = 0; x < width; ++x)
            {
                auto point = V2(x, y);

                if (*m_knots.begin() == point)
                    cout << "H";
                //else if (*(m_knots.end()-1) == point)
                //    cout << "t";
                else
                {
                    bool found = false;
                    u32 num = 0;
                    for (auto it = m_knots.begin() + 1;
                        it < m_knots.end();
                        ++it)
                    {
                        if (*it == point)
                        {
                            found = true;
                            num = it - m_knots.begin();
                            break;
                        }
                    }
                    if (found)
                    {
                        //cout << "o";
                        cout << num;
                    }
                    else
                        cout << ".";
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    void print_visited_by_tail()
    {
        u32 width = 26;
        u32 height = 21;

        for (u32 y = 0; y < height; ++y)
        {
            for (u32 x = 0; x < width; ++x)
            {
                auto point = V2(x, y);

                if (m_visited_by_tail.count(point) > 0)
                    cout << "#";
                else
                    cout << ".";
            }
            cout << endl;
        }
        cout << endl;
    }

    u64 visited_by_tail()
    {
        return m_visited_by_tail.size();
    }

private:

    V2 update_tail_pos(const V2& head, const V2& tail)
    {
        auto new_tail = tail;

        auto distance = (head - tail).len();

        if (distance > m_threshold)
        {
            u32 dx = std::abs(head.x - tail.x);
            u32 dy = std::abs(head.y - tail.y);

            if (dy == 0) // head moved left or right
            {
                if (head.x > tail.x) // head moved right
                {
                    new_tail.x = tail.x + 1;
                }
                else  // head moved left
                {
                    new_tail.x = tail.x - 1;
                }
            }
            else if (dx == 0) // head moved up or down
            {
                if (head.y < tail.y) // head moved up
                {
                    new_tail.y = tail.y - 1;
                }
                else // head moved down
                {
                    new_tail.y = tail.y + 1;;
                }
            }
            else // head moved diagonally
            {
                if (head.x > tail.x)
                {
                    new_tail.x = tail.x + 1;
                }
                else 
                {
                    new_tail.x = tail.x - 1;
                }

                if (head.y > tail.y) 
                {
                    new_tail.y = tail.y + 1;
                }
                else 
                {
                    new_tail.y = tail.y - 1;
                }
            }
        }

        return new_tail;
    }

    std::array<V2, N> m_knots;
    std::set<V2, V2_Comparator> m_visited_by_tail;
    u32 m_threshold;
};



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

        auto rope = Rope<2>();

#if 0
        char dir;
        std::cin >> dir;
        while (dir != 'q')
        {
            switch (dir)
            {
            case 'w': rope.move('U'); break;
            case 's': rope.move('D'); break;
            case 'a': rope.move('L'); break;
            case 'd': rope.move('R'); break;
            }

            rope.print();
            std::cin >> dir;
        }
#endif // 0


        string line;
        while (std::getline(ifs, line))
        {
            std::istringstream iss(line);
            char dir;
            i32 count;

            iss >> dir >> count;

            for (auto i = 0;
                i < count;
                ++i)
            {
                rope.move(dir);
            }
        }

        cout << "visited_by_tail part_1: " << rope.visited_by_tail() << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
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
        auto rope = Rope<10>();

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

#if 0
        char dir;
        std::cin >> dir;
        while (dir != 'q')
        {
            switch (dir)
            {
            case 'w': rope.move('U'); break;
            case 's': rope.move('D'); break;
            case 'a': rope.move('L'); break;
            case 'd': rope.move('R'); break;
            }

            rope.print();
            std::cin >> dir;
        }
#endif // 0

        string line;
        while (std::getline(ifs, line))
        {
            std::istringstream iss(line);
            char dir;
            i32 count;

            iss >> dir >> count;

            for (auto i = 0;
                i < count;
                ++i)
            {
                rope.move(dir);
            }
        }

        cout << "visited_by_tail part_2: " << rope.visited_by_tail() << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
    }
}
