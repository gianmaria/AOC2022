#include "pch.h"



int main()
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
}
