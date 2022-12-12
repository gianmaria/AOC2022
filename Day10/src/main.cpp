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

        u32 cycle = 1;
        u32 signal_strength = 0;
        i32 X = 1;
        string line;

        auto cycle_up = [&](u32& cycle)
        {
            if (cycle >= 20)
            {
                if (cycle == 20 or
                    ((cycle - 20) % 40 == 0))
                {
                    /*cout << "cycle: " << cycle
                        << " X: " << X
                        << " signal strength: " << cycle * X
                        << endl;*/
                    signal_strength += cycle * X;
                }
            }

            ++cycle;
        };

        while (std::getline(ifs, line))
        {
            std::istringstream iss(line);

            str instruction;
            iss >> instruction;

            if (instruction == "addx")
            {
                i32 value;
                iss >> value;

                cycle_up(cycle);
                cycle_up(cycle);

                X += value;
            }
            else
            {
                cycle_up(cycle);
            }
        }

        cout << "part 1 - signal strength: " << signal_strength << endl;

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
#if 1
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif
        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        i32 cycle = 1;
        i32 X = 1;
        string line;

        std::array<std::array<char, 40>, 6> screen;

        for (auto& row : screen)
        {
            for (auto& pixel : row)
            {
                pixel = '.';
            }
        }

        auto print_screen = [&screen]()
        {
            for (auto& row : screen)
            {
                for (auto& pixel : row)
                {
                    cout << pixel;
                }
                cout << endl;
            }
        };

        auto cycle_up = [&]()
        {
            i32 h_sprite_pos = X; // ###

            i32 pixel_currently_being_drawn = cycle;

            if (h_sprite_pos - 1 == pixel_currently_being_drawn or
                h_sprite_pos     == pixel_currently_being_drawn or
                h_sprite_pos + 1 == pixel_currently_being_drawn)
            {
                u32 row = (cycle-1) / 40;
                u32 col = (cycle-1) % 40;
                screen[row][col] = '#';
            }

            ++cycle;
        };

        while (std::getline(ifs, line))
        {
            std::istringstream iss(line);

            str instruction;
            iss >> instruction;

            if (instruction == "addx")
            {
                i32 value;
                iss >> value;

                cycle_up();
                cycle_up();

                X += value;
            }
            else
            {
                cycle_up();
            }
        }

        cout << "part 2" << endl;
        print_screen();        

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
    }

    return 1;
}

