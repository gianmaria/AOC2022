#include "pch.h"

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

            input = input.substr(pos + 1, input.length());
        }
        else
        {
            res.push_back(std::move(input));
            break;
        }
    }

    return res;
}


void part_1()
{
    struct Monkey
    {
        u32 index = 0;
        std::queue<u64> items;
        std::array<str, 3> operation;
        u64 divisible_by = 0;
        u32 dest_monkey_if_true = 0;
        u32 dest_monkey_if_false = 0;

        u32 num_items_inspected = 0;

        u64 eval_operation(u64 old)
        {
            u64 res = 0;

            const auto& a = operation[0];
            const auto& op = operation[1];
            const auto& b = operation[2];

            if (a == "old")
                res = old;
            else
                res = std::stoll(a);

            if (op == "+")
            {
                if (b == "old")
                    res += old;
                else
                    res += std::stoll(b);
            }
            else if (op == "*")
            {
                if (b == "old")
                    res *= old;
                else
                    res *= std::stoll(b);
            }
            else
            {
                throw std::runtime_error("Unsupported operation");
            }

            return res;
        }


        bool operator<(const Monkey& rhs)
        {
            return num_items_inspected > rhs.num_items_inspected;
        }

        void parse_monkey(str_cref line)
        {
            auto res = split_by(line, " ");

            auto idx_str = res[1].substr(0, 1);

            index = std::stoi(idx_str);
        }

        void parse_starting_items(str_cref line)
        {
            auto res = split_by(line, " ");

            for (u32 i = 4;
                 i < res.size();
                 ++i)
            {
                auto item_str = res[i];

                if (item_str.find(",") != str::npos)
                {
                    auto pos = item_str.find(",");
                    res[i] = res[i].substr(0, pos);
                }

                items.push(std::stoi(res[i]));
            }
        }

        void parse_operation(str_cref line)
        {
            auto res = split_by(line, " ");

            operation[0] = res[5];
            operation[1] = res[6];
            operation[2] = res[7];
        }

        void parse_test(str_cref line)
        {
            auto res = split_by(line, " ");

            divisible_by = std::stoi(res[5]);
        }

        void parse_if_true(str_cref line)
        {
            auto res = split_by(line, " ");

            dest_monkey_if_true = std::stoi(res[9]);
        }

        void parse_if_false(str_cref line)
        {
            auto res = split_by(line, " ");

            dest_monkey_if_false = std::stoi(res[9]);
        }

    };

#if 0
    auto file_path = "res/test.txt";
#else
    auto file_path = "res/input.txt";
#endif
    cout << "[INFO] input file: " << file_path << endl;
    auto ifs = std::ifstream(file_path);

    if (not ifs.is_open())
        throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

    std::vector<Monkey> monkeys;

    // parse file
    while (true)
    {
        std::vector<str> lines;

        str line;
        for (u32 i = 0;
             i < 6;
             ++i)
        {
            std::getline(ifs, line);
            lines.push_back(line);
        }

        Monkey m;

        m.parse_monkey(lines[0]);
        m.parse_starting_items(lines[1]);
        m.parse_operation(lines[2]);
        m.parse_test(lines[3]);
        m.parse_if_true(lines[4]);
        m.parse_if_false(lines[5]);

        monkeys.push_back(std::move(m));

        if (not std::getline(ifs, line))
            break;
    }

    for (u32 round = 1;
         round <= 20;
         ++round)
    {
        for (u32 i = 0;
             i < monkeys.size();
             ++i)
        {
            auto& monkey = monkeys[i];

            while (monkey.items.size() > 0)
            {
                auto worry_level = monkey.items.front();
                monkey.items.pop();

                worry_level = monkey.eval_operation(worry_level);

                worry_level /= 3;

                if (worry_level % monkey.divisible_by == 0)
                {
                    auto idx = monkey.dest_monkey_if_true;
                    monkeys[idx].items.push(worry_level);
                }
                else
                {
                    auto idx = monkey.dest_monkey_if_false;
                    monkeys[idx].items.push(worry_level);
                }

                monkey.num_items_inspected++;
            }
        }
    }


    auto get_monkey_business = [](std::vector<Monkey> monkeys) -> u64
    {
        std::sort(monkeys.begin(), monkeys.end());

        auto res = monkeys[0].num_items_inspected * monkeys[1].num_items_inspected;

        return res;
    };

    auto res = get_monkey_business(monkeys);

    cout << "[INFO] res part 1: " << res << endl;
}

void part_2()
{

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

