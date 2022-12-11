#include "pch.h"


int part_1()
{
    try
    {
        using std::array;

#if 0
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        vector<vector<u32>> forest;

        string line;
        while (std::getline(ifs, line))
        {
            forest.push_back(vector<u32>{});

            for (char c : line)
            {
                forest.back().push_back(c - '0');
            }
        }

        u32 N = forest.size();

        u32 visible_trees = N + N + (N - 2) + (N - 2);

        for (i32 y = 1;
             y < N - 1;
             ++y)
        {
            for (i32 x = 1;
                 x < N - 1;
                 ++x)
            {
                u32 target_tree_height = forest[y][x];

                cout << "checking tree height " << target_tree_height
                    << " (" << y << ", " << x << ")" << endl;

                // check top
                bool visible_from_top = true;
                {
                    for (i32 yy = y - 1;
                         yy >= 0;
                         --yy)
                    {
                        u32 curr_tree = forest[yy][x];

                        if (curr_tree >= target_tree_height)
                        {
                            visible_from_top = false;
                            break;
                        }
                    }
                    if (visible_from_top)
                    {
                        cout << "  visible from top" << endl;
                    }
                }


                // check bottom
                bool visible_from_bottom = true;
                {
                    for (i32 yy = y + 1;
                         yy < N;
                         ++yy)
                    {
                        u32 curr_tree = forest[yy][x];

                        if (curr_tree >= target_tree_height)
                        {
                            visible_from_bottom = false;
                            break;
                        }
                    }
                    if (visible_from_bottom)
                    {
                        cout << "  visible from bottom" << endl;
                    }
                }


                // check left
                bool visible_from_left = true;
                {
                    for (i32 xx = x - 1;
                         xx >= 0;
                         --xx)
                    {
                        u32 curr_tree = forest[y][xx];

                        if (curr_tree >= target_tree_height)
                        {
                            visible_from_left = false;
                            break;
                        }
                    }
                    if (visible_from_left)
                    {
                        cout << "  visible from left" << endl;
                    }
                }


                // check right
                bool visible_from_right = true;
                {
                    for (i32 xx = x + 1;
                         xx < N;
                         ++xx)
                    {
                        u32 curr_tree = forest[y][xx];

                        if (curr_tree >= target_tree_height)
                        {
                            visible_from_right = false;
                            break;
                        }
                    }
                    if (visible_from_right)
                    {
                        cout << "  visible from right" << endl;
                    }
                }

                if (visible_from_top or visible_from_bottom or
                    visible_from_right or visible_from_left)
                    ++visible_trees;
            }
        }

        cout << endl << "visible trees: " << visible_trees << endl;

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
        using std::array;

#if 0
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        vector<vector<u32>> forest;

        string line;
        while (std::getline(ifs, line))
        {
            forest.push_back(vector<u32>{});

            for (char c : line)
            {
                forest.back().push_back(c - '0');
            }
        }

        u32 N = forest.size();
        u32 scenic_score = 0;

        for (i32 y = 1;
             y < N - 1;
             ++y)
        {
            for (i32 x = 1;
                 x < N - 1;
                 ++x)
            {

                u32 current_tree_height = forest[y][x];

                cout << "checking tree height " << current_tree_height
                    << " (" << y << ", " << x << ") ";

                // check top
                u32 top_count = 0;
                {
                    for (i32 yy = y - 1;
                         yy >= 0;
                         --yy)
                    {
                        u32 tree_in_front = forest[yy][x];

                        ++top_count;

                        if (tree_in_front >= current_tree_height)
                        {
                            break;
                        }
                    }
                }


                // check bottom
                u32 bottom_count = 0;
                {
                    for (i32 yy = y + 1;
                         yy < N;
                         ++yy)
                    {
                        u32 tree_to_the_back = forest[yy][x];

                        ++bottom_count;

                        if (tree_to_the_back >= current_tree_height)
                        {
                            break;
                        }
                    }
                }


                // check left
                u32 left_count = 0;
                {
                    for (i32 xx = x - 1;
                         xx >= 0;
                         --xx)
                    {
                        u32 tree_to_the_left = forest[y][xx];

                        ++left_count;

                        if (tree_to_the_left >= current_tree_height)
                        {
                            break;
                        }
                    }
                }


                // check right
                u32 right_count = 0;
                {
                    for (i32 xx = x + 1;
                         xx < N;
                         ++xx)
                    {
                        u32 tree_to_the_right = forest[y][xx];

                        ++right_count;

                        if (tree_to_the_right >= current_tree_height)
                        {
                            break;
                        }
                    }
                }


                u32 local_score =
                    top_count * bottom_count *
                    left_count * right_count;

                cout << "local_score: " << local_score << endl;

                if (local_score > scenic_score)
                {
                    scenic_score = local_score;
                }
            }
        }

        cout << endl << "scenic score: " << scenic_score << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
    }
}
