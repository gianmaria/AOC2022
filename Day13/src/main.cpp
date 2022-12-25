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






enum class Token_Type : u8
{
    number,
    open_parent,
    close_parent
};

struct Token
{
    Token_Type type;
    str val;

    operator Token_Type() const
    {
        return type;
    }
};

bool operator==(Token const& lhs, Token const& rhs)
{
    return lhs.type == rhs.type and lhs.val == rhs.val;
}

using Tokens = vector<Token>;

str to_str(const Tokens& tokens)
{
    str res;

    for (auto& tok : tokens)
    {
        res += tok.val + " ";
    }
    return res;
}

bool is_digit(str_cref s)
{
    for (char c : s)
    {
        bool is_number = (c >= '0') and (c <= '9');
        if (not is_number)
        {
            return false;
        }
    }
    return true;
}

Tokens tokenize(str_cref input)
{
    Tokens res;
    str tmp;

    for (char c : input)
    {
        if (c == '[')
        {
            res.emplace_back(Token_Type::open_parent, "[");
        }
        else if (c == ']')
        {
            if (tmp.size() > 0)
            {
                res.emplace_back(Token_Type::number, tmp);
                tmp.clear();
            }

            res.emplace_back(Token_Type::close_parent, "]");
        }
        else if (c == ',')
        {
            if (tmp.size() > 0)
            {
                res.emplace_back(Token_Type::number, tmp);
                tmp.clear();
            }
        }
        else
        {
            tmp.push_back(c);
        }
    }

    return res;
}

vector<Tokens> parse(Tokens tokens)
{
    vector<Tokens> ans;

    u32 depth = 0;
    Tokens tmp;

    for (auto& token : tokens)
    {
        if (token == Token_Type::open_parent)
        {
            ++depth;

            if (depth > 1)
            {
                tmp.push_back(token);
            }
        }
        else if (token == Token_Type::number)
        {
            if (depth == 1)
            {
                tmp.push_back(token);
                ans.push_back(tmp);

                tmp.clear();
            }
            else
            {
                tmp.push_back(token);
            }
        }
        else if (token == Token_Type::close_parent)
        {
            --depth;

            if (depth == 0)
            {
                if (tmp.size() > 0)
                {
                    ans.push_back(tmp);

                    tmp.clear();
                }
            }
            else if (depth == 1)
            {
                if (tmp.size() > 0)
                {
                    tmp.push_back(token);
                    ans.push_back(tmp);

                    tmp.clear();
                }
            }
            else
            {
                tmp.push_back(token);
            }
        }
    }

    return ans;
}

i32 compare(vector<Tokens> left_items, vector<Tokens> right_items)
{
    for (u64 i = 0;
         i < std::min(left_items.size(), right_items.size());
         ++i)
    {
        Tokens& left_item = left_items[i];
        Tokens& right_item = right_items[i];

        Token& left = left_item[0];
        Token& right = right_item[0];

        if (left == Token_Type::open_parent and
            right == Token_Type::open_parent)
        {
            auto res = compare(parse(left_item), parse(right_item));
            if (res != -1)
                return res;
        }
        else if (left == Token_Type::number and
                 right == Token_Type::number)
        {
            auto num_left = std::stoi(left.val);
            auto num_right = std::stoi(right.val);

            if (num_left < num_right)
                return 1;
            else if (num_left > num_right)
                return 0;
        }
        else if (left == Token_Type::number and
                 right != Token_Type::number)
        {
            Tokens tmp_left;

            tmp_left.push_back({Token_Type::open_parent,"["});
            tmp_left.push_back(left);
            tmp_left.push_back({Token_Type::close_parent,"]"});

            auto res = compare(parse(tmp_left), parse(right_item));
            if (res != -1)
                return res;
        }
        else if (left != Token_Type::number and
                 right == Token_Type::number)
        {
            Tokens tmp_right;

            tmp_right.push_back({Token_Type::open_parent,"["});
            tmp_right.push_back(right);
            tmp_right.push_back({Token_Type::close_parent,"]"});

            auto res = compare(parse(left_item), parse(tmp_right));
            if (res != -1)
                return res;
        }
    }

    if (left_items.size() < right_items.size())
        return 1;
    else if (left_items.size() > right_items.size())
        return 0;
    else
        return -1;
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

    auto pairs = split_by(file, "\n\n");

    u32 index = 1;
    u32 res = 0;
    for (auto& pair : pairs)
    {
        auto tmp = split_by(pair, "\n");

        auto left = tokenize(tmp[0]);
        auto right = tokenize(tmp[1]);

        auto in_order = compare(parse(left), parse(right));

        if (in_order == 1)
        {
            res += index;
        }
        
        ++index;
    }

    cout << "[INFO] res part 1: " << res << endl;
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

    auto pairs = split_by(file, "\n\n");
    pairs.push_back("[[2]]\n[[6]]");

    vector<Tokens> all_packets;

    for (auto& pair : pairs)
    {
        auto tmp = split_by(pair, "\n");

        auto left = tokenize(tmp[0]);
        auto right = tokenize(tmp[1]);

        all_packets.push_back(std::move(left));
        all_packets.push_back(std::move(right));
    }

    std::sort(all_packets.begin(), all_packets.end(),
              [](Tokens const& left, Tokens const& right)
    {
        return compare(parse(left), parse(right)) > 0;
    });

    auto first_packet = std::find(all_packets.begin(), all_packets.end(), tokenize("[[2]]")) - all_packets.begin();
    auto second_packet = std::find(all_packets.begin(), all_packets.end(), tokenize("[[6]]"))- all_packets.begin();

    cout << "[INFO] res part 2: " << (first_packet+1) * (second_packet+1) << endl;
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
