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

        cout << "input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("Cannot open file ") + file_path);


        auto split_string = [](const string& input) -> std::pair<str, str>
        {
            if (input.length() != 3)
                throw std::runtime_error("Can only split strings whose length is 3");

            auto pos = input.find(' ');

            if (pos == str::npos)
                throw std::runtime_error("Cannot find space in string");

            str opponent = input.substr(0, pos);
            str me = input.substr(pos + 1, input.length() - 1);

            return {opponent, me};
        };

        uint64_t score = 0;

        const uint64_t score_for_R = 1;
        const uint64_t score_for_P = 2;
        const uint64_t score_for_S = 3;
        
        const uint64_t score_if_lose = 0;
        const uint64_t score_if_draw = 3;
        const uint64_t score_if_win = 6;

        string line;
        while (std::getline(ifs, line))
        {
            auto [opponent, me] = split_string(line);

            if (opponent == "R" and me == "R")
            {
                score += score_for_R;
                score += score_if_draw;
            }
            else if (opponent == "R" and me == "P")
            {
                score += score_for_P;
                score += score_if_win;
            }
            else if (opponent == "R" and me == "S")
            {
                score += score_for_S;
                score += score_if_lose;
            }

            else if (opponent == "P" and me == "R")
            {
                score += score_for_R;
                score += score_if_lose;
            }
            else if (opponent == "P" and me == "P")
            {
                score += score_for_P;
                score += score_if_draw;
            }
            else if (opponent == "P" and me == "S")
            {
                score += score_for_S;
                score += score_if_win;
            }

            else if (opponent == "S" and me == "R")
            {
                score += score_for_R;
                score += score_if_win;
            }
            else if (opponent == "S" and me == "P")
            {
                score += score_for_P;
                score += score_if_lose;
            }
            else if (opponent == "S" and me == "S")
            {
                score += score_for_S;
                score += score_if_draw;
            }

            else
                throw std::runtime_error("Unknown charater");
        }

        cout << "Total score: " << score << endl;

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
        auto file_path = "res/test_pt2.txt";
#else
        auto file_path = "res/input_pt2.txt";
#endif

        cout << "input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("Cannot open file ") + file_path);

        auto split_string = [](const string& input) -> std::pair<str, str>
        {
            if (input.length() != 3)
                throw std::runtime_error("Can only split strings whose length is 3");

            auto pos = input.find(' ');

            if (pos == str::npos)
                throw std::runtime_error("Cannot find space in string");

            str opponent = input.substr(0, pos);
            str outcome = input.substr(pos + 1, input.length() - 1);

            return {opponent, outcome};
        };

        uint64_t score = 0;

        const uint64_t score_for_R = 1;
        const uint64_t score_for_P = 2;
        const uint64_t score_for_S = 3;
        
        const uint64_t score_if_lose = 0;
        const uint64_t score_if_draw = 3;
        const uint64_t score_if_win = 6;

        string line;
        while (std::getline(ifs, line))
        {
            auto [opponent, outcome] = split_string(line);

            if (opponent == "R" and outcome == "L")
            {
                // me = S
                score += score_for_S;
                score += score_if_lose;
            }
            else if (opponent == "R" and outcome == "D")
            {
                // me = R
                score += score_for_R;
                score += score_if_draw;
            }
            else if (opponent == "R" and outcome == "W")
            {
                // me = P
                score += score_for_P;
                score += score_if_win;
            }

            else if (opponent == "P" and outcome == "L")
            {
                // me = R
                score += score_for_R;
                score += score_if_lose;
            }
            else if (opponent == "P" and outcome == "D")
            {
                // me = P
                score += score_for_P;
                score += score_if_draw;
            }
            else if (opponent == "P" and outcome == "W")
            {
                // me = S
                score += score_for_S;
                score += score_if_win;
            }

            else if (opponent == "S" and outcome == "L")
            {
                // me = P
                score += score_for_P;
                score += score_if_lose;
            }
            else if (opponent == "S" and outcome == "D")
            {
                // me = S
                score += score_for_S;
                score += score_if_draw;
            }
            else if (opponent == "S" and outcome == "W")
            {
                // me = R
                score += score_for_R;
                score += score_if_win;
            }

            else
                throw std::runtime_error("Unknown charater");
        }

        cout << "Total score: " << score << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}

