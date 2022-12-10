#include "pch.h"



class FileSystem
{
public:

    FileSystem() :
        current_node(new Node("/", 0, true, nullptr))
    {
    }

    FileSystem cd(str_cref dir_name)
    {
        if (dir_name == ".")
        {
            return FileSystem(current_node);
        }
        else if (dir_name == "..")
        {
            if (current_node->parent == nullptr)
                throw std::runtime_error("cannot 'cd ..' you're already at /");

            return FileSystem(current_node->parent);
        }
        else
        {
            if (current_node->children.count(dir_name) == 0)
                throw std::runtime_error(dir_name + " does not exist! cannot cd");

            return FileSystem(current_node->children[dir_name]);
        }
    }

    FileSystem& touch(str_cref file_name, u32 file_size)
    {
        std::pair<str, Node*> new_item;
        new_item.first = file_name;
        new_item.second = new Node(file_name, file_size, false, nullptr);

        auto [it, success] = current_node->children.insert(std::move(new_item));

        if (!success)
            throw std::runtime_error("failed to add file: " + file_name);

        return *this;
    }

    FileSystem& mkdir(str_cref dir_name)
    {
        std::pair<str, Node*> new_item;
        new_item.first = dir_name;
        new_item.second = new Node(dir_name, 0, true, current_node);

        auto [it, success] = current_node->children.insert(std::move(new_item));

        if (!success)
            throw std::runtime_error("failed to insert dir: " + dir_name);

        return *this;
    }

    void ls() const
    {
        for (auto& [name, child] : current_node->children)
        {
            if (child->is_dir)
            {
                cout << name << " (dir)" << endl;
            }
            else
            {
                cout << name << " (file, size=" << child->size << ")" << endl;
            }
        }
    }

    void tree() const
    {
        tree_helper(current_node, 0);
    }

    void print() const
    {
        tree_helper(current_node, 0);
    }

    bool is_root() const
    {
        return current_node->is_dir and (current_node->parent == nullptr);
    }

    u32 size() const
    {
        return current_node->size;
    }

    u32 calculate_dir_size_recursively() const
    {
        return calculate_dir_size_recursively_helper(current_node);
    }

    FileSystem operator[](str_cref dir_name)
    {
        return cd(dir_name);
    }

    u32 solve_part_1(u32 limit) const
    {
        u32 acc = 0;
        solve_part_1_helper(current_node, limit, acc);

        return acc;
    }

    u32 solve_part_2(u32 limit) const
    {
        vector<Node*> candidate_to_delete;

        solve_part_2_helper(current_node, limit, candidate_to_delete);

        std::sort(candidate_to_delete.begin(), candidate_to_delete.end(),
                  [](const Node* a, const Node* b)
        {
            return a->size < b->size;
        });

        return candidate_to_delete.front()->size;
    }

private:

    struct Node
    {
        str name;
        u32 size = 0;
        bool is_dir = false;

        Node* parent;
        std::map<str, Node*> children;

        Node(str name, u32 size, bool is_dir, Node* parent) :
            name(std::move(name)),
            size(size),
            is_dir(is_dir),
            parent(parent)
        {}

        u32 files_size() const
        {
            u32 res = size;

            for (auto& [_, child] : children)
            {
                if (not child->is_dir)
                    res += child->size;
            }

            return res;
        }
    };

    FileSystem(Node* node) :
        current_node(node)
    {}

    void tree_helper(const Node* node, u32 indent) const
    {
        auto space = [](u32 amount)
        {
            str res;

            for (u32 i = 0;
                 i < amount;
                 ++i)
                res += " ";

            return res;
        };

        cout << space(indent) << "- " << node->name << " (dir, size=" << node->size << ")" << endl;

        for (auto& [_, child] : node->children)
        {
            if (child->is_dir)
            {
                tree_helper(child, indent + 2);
            }
            else
            {
                cout << space(indent + 2) << "- " << child->name << " (file, size=" << child->size << ")" << endl;
            }
        }
    }

    u32 calculate_dir_size_recursively_helper(Node* node) const
    {
        u32 res = node->size;

        for (auto& [_, child] : node->children)
        {
            if (child->is_dir)
            {
                res += calculate_dir_size_recursively_helper(child);
            }
            else
            {
                res += child->size;
            }
        }

        if (node->is_dir) // update dir size
            node->size = res;

        return res;
    }

    void solve_part_1_helper(const Node* node, u32 limit, u32& acc) const
    {
        for (auto& [_, child] : node->children)
        {
            if (child->is_dir)
            {
                solve_part_1_helper(child, limit, acc);
            }
        }

        if (node->size <= limit)
        {
            //cout << "dir is: " << node->name << endl;
            acc += node->size;
        }
    }

    void solve_part_2_helper(Node* node, u32 limit, vector<Node*>& candidate_to_delete) const
    {
        for (auto& [_, child] : node->children)
        {
            if (child->is_dir)
            {
                solve_part_2_helper(child, limit, candidate_to_delete);
            }
        }

        if (node->size >= limit)
        {
            cout << "candidate dir is: " << node->name << endl;
            candidate_to_delete.push_back(node);
        }
    }

    Node* current_node;
};



int part_1()
{
    try
    {
        auto root = FileSystem();

        auto fs = root;

#if 1
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        string line;
        while (std::getline(ifs, line))
        {
            if (line.starts_with("$"))
            {
                // parse command
                std::istringstream iss(line);

                str prompt;
                str command;

                iss >> prompt >> command;

                if (command == "cd")
                {
                    str arg;
                    iss >> arg;

                    if (arg == "/")
                        fs = root;
                    else
                        fs = fs.cd(arg);
                }
                else if (command == "ls")
                {
                    // parse ls output command

                    while (true)
                    {
                        auto pos = ifs.tellg();

                        if (not std::getline(ifs, line))
                            break; // stream is over

                        if (line.starts_with("$"))
                        {
                            ifs.seekg(pos);
                            break;
                        }

                        iss = std::istringstream(line);

                        str a, b;
                        iss >> a >> b;

                        if (a == "dir")
                        {
                            fs.mkdir(b);
                        }
                        else
                        {
                            fs.touch(b, std::stoul(a));
                        }
                    }
                }
            }
        }

        auto root_size = root.calculate_dir_size_recursively();
        root.print();

        u32 res = root.solve_part_1(100000);
        cout << "res is: " << res << endl;

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
        auto root = FileSystem();

        auto fs = root;
#if 1
        auto file_path = "res/test.txt";
#else
        auto file_path = "res/input.txt";
#endif

        cout << "[INFO] input file: " << file_path << endl;
        auto ifs = std::ifstream(file_path);

        if (not ifs.is_open())
            throw std::runtime_error(str("[ERROR] Cannot open file ") + file_path);

        string line;
        while (std::getline(ifs, line))
        {
            if (line.starts_with("$"))
            {
                // parse command
                std::istringstream iss(line);

                str prompt;
                str command;

                iss >> prompt >> command;

                if (command == "cd")
                {
                    str arg;
                    iss >> arg;

                    if (arg == "/")
                        fs = root;
                    else
                        fs = fs.cd(arg);
                }
                else if (command == "ls")
                {
                    // parse ls output command

                    while (true)
                    {
                        auto pos = ifs.tellg();

                        if (not std::getline(ifs, line))
                            break; // stream is over

                        if (line.starts_with("$"))
                        {
                            ifs.seekg(pos);
                            break;
                        }

                        iss = std::istringstream(line);

                        str a, b;
                        iss >> a >> b;

                        if (a == "dir")
                        {
                            fs.mkdir(b);
                        }
                        else
                        {
                            fs.touch(b, std::stoul(a));
                        }
                    }
                }
            }
        }

        auto root_size = root.calculate_dir_size_recursively();
        root.print();

        u32 total_disk_space = 70000000;
        u32 space_needed_for_update = 30000000;
        u32 occupied = root.size();

        u32 free_space = total_disk_space - occupied;

        u32 need_to_free = space_needed_for_update - free_space;

        u32 res = root.solve_part_2(need_to_free);
        cout << "res is: " << res << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXC] " << e.what() << endl;
    }
}
