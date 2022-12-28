#include "pch.h"
#include <cassert>
#include <limits>

//struct Foo
//{
//
//    Foo(str a) : a(a)
//    {
//        cout << a << " is alive" << endl;
//    }
//    ~Foo()
//    {
//        cout << a << " is dead" << endl;
//    }
//
//    Foo() = default;
//    Foo(const Foo&) = default;
//    Foo(Foo&&) = default;
//    Foo& operator=(const Foo&) = default;
//    Foo& operator=(Foo&&) = default;
//    auto operator<=>(const Foo&) const = default;
//
//    str a;
//};


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

vector<str> split(str input, str_cref delim)
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



template<u64 N>
struct Graph
{
    static constexpr u32 V = N;
    i32 dist[V][V];
    i32 next[V][V];
    static constexpr i32 Inf = std::numeric_limits<i32>::max();
    static constexpr i32 Nul = -1;

    using Idx = u32;
    using Flow = u32;
    mutable std::map<str, Idx> name_idx_map;
    mutable std::map<str, Flow> name_flow_map;

    Graph()
    {
        for (u32 i = 0;
             i < V;
             ++i)
        {
            for (u32 j = 0;
                 j < V;
                 ++j)
            {
                dist[i][j] = Inf;
                next[i][j] = Nul;
            }
        }
    }

    void AddEdge(str_cref from, str_cref to,
                 i32 flow, i32 offset)
    {
        u32 u = NameToIndex(from);
        u32 v = NameToIndex(to);

        i32 weight = offset - flow;

        dist[u][v] = weight;
        next[u][v] = v;

        name_flow_map[from] = flow;
    }

    i32 Dist(u32 from, u32 to) const
    {
        return dist[from][to];
    }

    i32 Dist(str_cref from, str_cref to) const
    {
        u32 u = NameToIndex(from);
        u32 v = NameToIndex(to);

        return Dist(u, v);
    }

    void FloydWarshall()
    {
        for (u32 v = 0;
             v < V;
             ++v)
        {
            dist[v][v] = 0;
            next[v][v] = v;
        }

        for (u32 k = 0; k < V; ++k)
        {
            for (u32 i = 0; i < V; ++i)
            {
                for (u32 j = 0; j < V; ++j)
                {
                    if (dist[i][k] == Inf or
                        dist[k][j] == Inf)
                        continue;

                    if (dist[i][j] > dist[i][k] + dist[k][j])
                    {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }

                }
            }
        }
    }

    vector<u32> Path(u32 from, u32 to) const
    {
        if (next[from][to] == Nul)
        {
            return {};
        }

        auto path = vector<u32>{from};

        while (from != to)
        {
            from = next[from][to];
            path.push_back(from);
        }

        return path;
    }

    vector<u32> Path(str_cref from, str_cref to) const
    {
        return Path(NameToIndex(from), NameToIndex(to));
    }

    u32 NameToIndex(str_cref s) const
    {
        if (name_idx_map.count(s) == 0)
        {
            u32 idx = name_idx_map.size();
            auto res = name_idx_map.insert({s, idx});

            assert(res.second);

            return idx;
        }
        else
        {
            return name_idx_map[s];
        }
    }

    str IndexToName(u32 u) const
    {
        for (auto const& [name, idx] : name_idx_map)
        {
            if (idx == u)
                return name;
        }
        return {};
    }

};


//constexpr u32 V = 58;
Graph<58> g;


struct Node
{
    using Weight = i32;
    using ID = i32;

    ID name;
    vector<Node*> children;

    bool discovered = false;
    i32 flow = 0;
    bool valve_close = true;

    Node(ID name, i32 flow) :
        name(name), flow(flow)
    {
    }

};

struct Tree
{
    Node* root = nullptr;

    std::map<Node::ID, Node*> nodes;

    Node* AddNode(Node::ID name, i32 flow)
    {
        auto* new_node = new Node(name, flow);

        nodes.insert({name, new_node});
        return new_node;
    }

    void AddChild(Node::ID from, Node::ID to)
    {
        nodes[from]->children.push_back(nodes[to]);
    }

    vector<Node*> Adjacent(Node::ID name)
    {
        return nodes[name]->children;
    }
};

void DFSHelper(Tree& tree, Node* v,
               i32& timer, i32& acc)
{
    str name = g.IndexToName(v->name);
    cout << name;

    v->discovered = true;

    if (v->valve_close and
        v->flow != 0)
    {
        v->valve_close = false;

        if (timer > 0)
        {
            --timer;
            acc += v->flow * timer;
        }

        cout << "* -> ";
    }
    else
    {
        cout << " -> ";
    }

    auto adj = tree.Adjacent(v->name);
    std::sort(adj.begin(), adj.end(),
              [](const Node* a, const Node* b)
    {
        return a->flow > b->flow;
    });

    bool found = false;
    for (Node* w : adj)
    {
        if (not w->discovered)
        {
            if (timer > 0)
            {
                --timer;
                DFSHelper(tree, w, timer, acc);
            }
        }
    }

}

i32 DFS(Tree& tree, Node* v)
{
    i32 timer = 30;
    i32 acc = 0;
    DFSHelper(tree, v, timer, acc);

    cout << endl;
    return acc;
}

void part1()
{
#if 0
    auto file_path = "res/test.txt";

#else
    auto file_path = "res/input.txt";
#endif

    str file = read_file(file_path);
    auto lines = split(file, "\n");

    int offset = 10000;
    std::map<str, bool> open_valve;

    for (auto const& line : lines)
    {
        auto part = split(line, " ");

        str from = part[1];
        open_valve[from] = false;

        i32 flow = std::stoi(split(part[4], "=")[1]);

        u32 i = 9;
        for (;
             i < part.size();
             ++i)
        {
            str to;

            if (auto pos = part[i].find(',');
                pos != str::npos)
            {
                to = part[i].erase(pos);
            }
            else
            {
                to = part[i];
            }

            g.AddEdge(from, to, flow, offset);
        }
    }

    g.FloydWarshall();

#if 0
    auto calc = [&g, &offset](u32 from, u32 to)
    {
        auto res = g.Path(from, to);

        cout << "From " << from << " to " << to << " : ";
        for (auto const& v : res)
            cout << g.IndexToName(v) << " -> ";
        cout << endl;

        //cout << "weight: " << g.Dist(from, to) << endl;
        //cout << "weight: " << (offset * (res.size() - 1)) - g.Dist(from, to) << endl;

        };

#endif // 0

    Tree t;

    for (u32 i = 0;
         i < g.V;
         ++i)
    {
        auto flow = g.name_flow_map[g.IndexToName(i)];
        t.AddNode(i, flow);
    }

    t.root = t.nodes[g.NameToIndex("AA")];

    for (u32 i = 0;
         i < g.V;
         ++i)
    {
        i32 min = 10000000;
        for (u32 j = 0;
             j < g.V;
             ++j)
        {
            if (g.dist[i][j] != 0 and
                g.dist[i][j] < min)
                min = g.dist[i][j];
        }

        for (u32 j = 0;
             j < g.V;
             ++j)
        {
            if (g.dist[i][j] == min)
                t.AddChild(i, j);
        }
    }

    auto acc = DFS(t, t.root);

    cout << "part 1: " << acc << endl;

#if 0
    Graph<4> g;

    int offset = 100;

    g.AddEdge(0, 2, offset - (-2));
    g.AddEdge(1, 0, offset - (+4));
    g.AddEdge(1, 2, offset - (+3));
    g.AddEdge(2, 3, offset - (+2));
    g.AddEdge(3, 1, offset - (-1));

    g.FloydWarshall();

    auto res = g.Path(2 - 1, 4 - 1);

    cout << "From 2 to 4: ";
    for (auto const& v : res)
        cout << v + 1 << " -> ";
    cout << endl;

    cout << "weight: " << g.Dist(2 - 1, 4 - 1) << endl;
    cout << "weight: " << (100 * (res.size() - 1)) - g.Dist(2 - 1, 4 - 1) << endl;

#endif // 0
    }

void part2()
{
#if 1
    auto file_path = "res/test.txt";
#else
    auto file_path = "res/input.txt";
#endif

    str file = read_file(file_path);
    auto lines = split(file, "\n");

    cout << "part 2: " << 0 << endl;
}

int main()
{
    try
    {
        part1();
        part2();

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << "[EXCEPTION] " << e.what() << endl;
    }

    return 1;
}
