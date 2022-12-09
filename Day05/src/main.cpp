#include "pch.h"

int part_1()
{
    try
    {
#if 0
        auto file_path = "res/test.txt";
        constexpr auto rows = 3;
        constexpr auto cols = 3;
#else
        auto file_path = "res/input.txt";
        constexpr auto rows = 8;
        constexpr auto cols = 9;
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        using std::array;
        using std::stack;

        array<array<char, cols>, rows> matrix;

        for (u32 row = 0;
             row < rows;
             ++row)
        {
            string line;
            std::getline(ifs, line);

            std::istringstream iss(line);

            for (u32 col = 0;
                 col < cols;
                 ++col)
            {
                str elem;
                iss >> elem;

                char c = elem[1];
                matrix[row][col] = c;

                int stop = 0;
            }
        }

        constexpr auto num_of_stacks = cols;
        array<stack<char>, num_of_stacks> stacks;

        for (u32 col = 0;
             col < cols;
             ++col)
        {
            for (u32 row_ = 0;
                 row_ < rows;
                 ++row_)
            {
                u32 row = rows - 1 - row_;

                char c = matrix[row][col];

                if (c == '_')
                    break;

                stacks[col].push(c);

                int stop = 0;
            }
        }

        string line;
        std::getline(ifs, line); // discard line
        std::getline(ifs, line); // discard line
        while (std::getline(ifs, line))
        {
            if (line.starts_with("//") or
                line == "")
                continue;

            std::istringstream iss(line);

            str i_s;
            u32 count;
            u32 from_stack;
            u32 to_stack;

            // move 1 from 2 to 1
            iss >> i_s >> count >> i_s >> from_stack >> i_s >> to_stack;

            for (u32 i = 0;
                 i < count;
                 ++i)
            {
                auto elem = stacks[from_stack - 1].top();
                stacks[from_stack - 1].pop();

                stacks[to_stack - 1].push(elem);
            }

            int stop = 0;
        }

        string res;

        for (const auto& stack : stacks)
        {
            res.push_back(stack.top());
        }

        cout << "res: " << res << endl;


        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
    }
}

int main()
{
    try
    {
        using std::array;
        using std::stack;
#if 0
        auto file_path = "res/test.txt";
        constexpr auto rows = 3;
        constexpr auto cols = 3;
#else
        auto file_path = "res/input.txt";
        constexpr auto rows = 8;
        constexpr auto cols = 9;
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);



        array<array<char, cols>, rows> matrix;

        for (u32 row = 0;
             row < rows;
             ++row)
        {
            string line;
            std::getline(ifs, line);

            std::istringstream iss(line);

            for (u32 col = 0;
                 col < cols;
                 ++col)
            {
                str elem;
                iss >> elem;

                char c = elem[1];
                matrix[row][col] = c;

                int stop = 0;
            }
        }

        constexpr auto num_of_stacks = cols;
        array<stack<char>, num_of_stacks> stacks;

        for (u32 col = 0;
             col < cols;
             ++col)
        {
            for (u32 row_ = 0;
                 row_ < rows;
                 ++row_)
            {
                u32 row = rows - 1 - row_;

                char c = matrix[row][col];

                if (c == '_')
                    break;

                stacks[col].push(c);

                int stop = 0;
            }
        }

        string line;
        std::getline(ifs, line); // discard line
        std::getline(ifs, line); // discard line
        while (std::getline(ifs, line))
        {
            if (line == "")
                continue;

            std::istringstream iss(line);

            str i_s;
            u32 count;
            u32 from_stack;
            u32 to_stack;

            // move 3 from 1 to 3
            iss >> i_s >> count >> i_s >> from_stack >> i_s >> to_stack;

            std::stack<char> tmp;
            
            for (u32 i = 0;
                 i < count;
                 ++i)
            {
                auto elem = stacks[from_stack - 1].top();
                stacks[from_stack - 1].pop();

                tmp.push(elem);
            }

            for (u32 i = 0;
                 i < count;
                 ++i)
            {
                auto elem = tmp.top();
                tmp.pop();

                stacks[to_stack - 1].push(elem);
            }

            int stop = 0;
        }

        string res;

        for (const auto& stack : stacks)
        {
            res.push_back(stack.top());
        }

        cout << "res: " << res << endl;


        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
    }
}
