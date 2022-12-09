#include "pch.h"

int main()
{
    try
    {
        using std::array;
        using std::stack;
        using std::set;
#if 0
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        u32 window_width = 14;

        string line;
        while (std::getline(ifs, line))
        {
            if (line.length() < window_width)
            {
                cout << "[WARN] skipped line: " << line << endl;
                continue;
            }

            bool found = false;

            set<char> set;

            u32 offset = 0;
            for (;
                 offset < line.length() - window_width;
                 ++offset)
            {
                for (u32 j = offset;
                     offset + window_width < line.length() and
                     j < offset + window_width;
                     ++j)
                {
                    char c = line[j];
                    set.insert(c);
                }

                if (set.size() == window_width)
                {
                    cout << "first marker after character " << offset + window_width << endl;
                    found = true;
                    break;
                }

                set.clear();
            }

            if (!found)
            {
                // one last round for the last window_width chars
                for (u32 j = offset;
                     j < offset + window_width;
                     ++j)
                {
                    char c = line[j];
                    set.insert(c);
                }

                if (set.size() == window_width)
                {
                    cout << "first marker after character " << offset + window_width << endl;
                }
                else
                {
                    cout << "marker NOT found in line " << line << endl;
                }
            }

        }

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
    }
}
