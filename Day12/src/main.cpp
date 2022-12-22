#include "pch.h"
#include <cfenv>

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

struct V2
{
    int x = 0;
    int y = 0;

    str to_str() const
    {
        return std::format("({},{})", x, y);
    }
};

bool operator<(const V2& a, const V2& b)
{
    if (a.x != b.x)
    {
        return a.x < b.x;
    }
    else
    {
        return a.y < b.y;
    }
}

bool operator==(const V2& a, const V2& b)
{
    return (a.x == b.x) and (a.y == b.y);
}

struct Vertex;

struct Edge
{
    Vertex* from = nullptr;
    Vertex* to = nullptr;
    u32 weight = 0;
};

const u32 Inf = 0xffffffff;

using ID = V2;

struct Vertex
{
    str name;
    ID id;
    std::set<Edge*> edges;
    vector<Vertex*> neighbours;

    u32 dist_from_source = Inf;
    Vertex* prev = nullptr;

    str to_str() const
    {
        std::ostringstream iss;

        /*iss << "\"" << name << "\"" << " " << id.to_str() << " [";

        for (const auto& n : neighbors)
        iss << n.to->id.to_str() << ", ";

        iss << "]";*/

        iss << "\"" << name << "\"";
        return iss.str();
    }
};

bool operator==(const Vertex& a, const Vertex& b)
{
    return a.id == b.id;
}

bool operator<(const Vertex& a, const Vertex& b)
{
    return a.id < b.id;
}

struct Graph
{
    std::map<ID, Vertex*> vertices;
    std::set<Edge*> edges;

    void add_vertex(Vertex&& vertex)
    {
        auto* v = new Vertex(std::move(vertex));

        auto res = vertices.insert({v->id, v});

        if (res.second == false)
        {
            throw std::runtime_error("Node being added is duplicated");
        }

    }

    void add_edge(const ID& from, const ID& to, u32 weight)
    {
        if (vertices.find(from) == vertices.end())
            throw std::runtime_error("'from' Node not found, cannot create Edge");

        if (vertices.find(to) == vertices.end())
            throw std::runtime_error("'to' Node not found, cannot create Edge");

        auto* vertex_from = vertices[from];
        auto* vertex_to = vertices[to];

        auto* edge = new Edge{vertex_from, vertex_to, weight};
        edges.insert(edge);

        vertex_from->edges.insert(edge);
        vertex_from->neighbours.push_back(vertex_to);
    }

    void dijkstra(Vertex& source, const Vertex& dest)
    {
        std::vector<Vertex*> Q;

        for (auto& pair : vertices)
        {
            auto* vertex = pair.second;

            vertex->dist_from_source = Inf;
            vertex->prev = nullptr;

            Q.push_back(vertex);
        }

        source.dist_from_source = 0;

        auto min_dist = [](std::vector<Vertex*>& Q)
        {
            auto pred = [](const Vertex* a,
                           const Vertex* b)
            {
                return a->dist_from_source < b->dist_from_source;
            };

            std::sort(Q.begin(), Q.end(), pred);
            return Q.front();
        };

        u32 alt = 0;

        while (Q.size() > 0)
        {
            Vertex* u = min_dist(Q);

            if (*u == dest)
                break;

            Q.erase(std::find(Q.begin(), Q.end(), u));

            for (Vertex* v : u->neighbours)
            {
                // check if neighbour v is still in Q, otherwise continue
                if (std::find(Q.begin(), Q.end(), v) == Q.end())
                    continue;

                auto u_v_dist = dist(u, v);

                if (u->dist_from_source == Inf or
                    u_v_dist == Inf)
                {
                    alt = Inf;
                }
                else
                {
                    alt = u->dist_from_source + u_v_dist;
                }

                if (alt < v->dist_from_source)
                {
                    v->dist_from_source = alt;
                    v->prev = u;
                }

            }

        }
    }

    vector<const Vertex*> shortest_path(const Vertex& source, const Vertex& target)
    {
        vector<const Vertex*> res;

        const Vertex* u = &target;

        if (u->prev != nullptr or
            *u == source)
        {
            while (u != nullptr)
            {
                res.push_back(u);
                u = u->prev;
            }
        }

        return res;
    }

    void reset()
    {
        for (auto& [id, vertex] : vertices)
        {
            vertex->dist_from_source = Inf;
            vertex->prev = nullptr;
        }
    }

private:

    u32 dist(const Vertex* from, const Vertex* to)
    {
        u32 res = 0;
        bool found = false;

        for (const Edge* edge : from->edges)
        {
            if (*edge->to == *to)
            {
                res = edge->weight;
                found = true;
                break;
            }
        }

        if (found)
            return res;
        else
            return Inf;
    }
};

void part_1()
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

    vector<vector<char>> map;

    V2 start, end;

    str line;
    while (std::getline(ifs, line))
    {
        map.push_back({});

        for (auto c : line)
        {
            if (c == 'S')
            {
                map.back().push_back('a');

                start.x = map.back().size() - 1;
                start.y = map.size() - 1;
            }
            else if (c == 'E')
            {
                map.back().push_back('z');

                end.x = map.back().size() - 1;
                end.y = map.size() - 1;
            }
            else
            {
                map.back().push_back(c);
            }
        }
        map.back().shrink_to_fit();
    }
    map.shrink_to_fit();

    auto HEIGHT = map.size();
    auto WIDTH = map[0].size();

    Graph g;

    for (i32 y = 0; y < HEIGHT; ++y)
    {
        for (i32 x = 0; x < WIDTH; ++x)
        {
            auto c = map[y][x];
            auto name = str(1, c);

            g.add_vertex({name, {(i32)x, (i32)y}});
        }
    }

    for (i32 y = 0; y < HEIGHT; ++y)
    {
        for (i32 x = 0; x < WIDTH; ++x)
        {
            // left
            if (x - 1 >= 0)
            {
                int delta = map[y][x] - map[y][x - 1];
                if (delta >= -1)
                {
                    g.add_edge({x,y}, {x - 1,y}, (delta == 0) ? std::abs(1) : std::abs(delta));
                }
            }

            // right
            if (x + 1 < WIDTH)
            {
                int delta = map[y][x] - map[y][x + 1];
                if (delta >= -1)
                {
                    g.add_edge({x,y}, {x + 1,y}, (delta == 0) ? std::abs(1) : std::abs(delta));
                }
            }

            // top
            if (y - 1 >= 0)
            {
                int delta = map[y][x] - map[y - 1][x];
                if (delta >= -1)
                {
                    g.add_edge({x,y}, {x,y - 1}, (delta == 0) ? std::abs(1) : std::abs(delta));
                }
            }

            // bottom
            if (y + 1 < HEIGHT)
            {
                int delta = map[y][x] - map[y + 1][x];
                if (delta >= -1)
                {
                    g.add_edge({x,y}, {x,y + 1}, (delta == 0) ? std::abs(1) : std::abs(delta));
                }
            }
        }
    }

    auto* source = g.vertices[{start.x, start.y}];
    auto* dest = g.vertices[{end.x, end.y}];

    g.dijkstra(*source, *dest);

    auto shortest_path = g.shortest_path(*source, *dest);

    if (shortest_path.size() == 0)
    {
        cout << "No shortest path found :(" << endl;
    }

    cout << "[INFO] res part 1: " << shortest_path.size() - 1 << endl;
}

void part_2()
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

    vector<vector<char>> map;

    V2 start, end;

    str line;
    while (std::getline(ifs, line))
    {
        map.push_back({});

        for (auto c : line)
        {
            if (c == 'S')
            {
                map.back().push_back('a');

                start.x = map.back().size() - 1;
                start.y = map.size() - 1;
            }
            else if (c == 'E')
            {
                map.back().push_back('z');

                end.x = map.back().size() - 1;
                end.y = map.size() - 1;
            }
            else
            {
                map.back().push_back(c);
            }
        }
        map.back().shrink_to_fit();
    }
    map.shrink_to_fit();

    auto HEIGHT = map.size();
    auto WIDTH = map[0].size();

    Graph g;

    for (i32 y = 0; y < HEIGHT; ++y)
    {
        for (i32 x = 0; x < WIDTH; ++x)
        {
            auto c = map[y][x];
            auto name = str(1, c);

            g.add_vertex({name, {(i32)x, (i32)y}});
        }
    }

    for (i32 y = 0; y < HEIGHT; ++y)
    {
        for (i32 x = 0; x < WIDTH; ++x)
        {
            // left
            if (x - 1 >= 0)
            {
                int delta = map[y][x] - map[y][x - 1];
                if (delta >= -1)
                {
                    g.add_edge({x,y}, {x - 1,y}, (delta == 0) ? std::abs(1) : std::abs(delta));
                }
            }

            // right
            if (x + 1 < WIDTH)
            {
                int delta = map[y][x] - map[y][x + 1];
                if (delta >= -1)
                {
                    g.add_edge({x,y}, {x + 1,y}, (delta == 0) ? std::abs(1) : std::abs(delta));
                }
            }

            // top
            if (y - 1 >= 0)
            {
                int delta = map[y][x] - map[y - 1][x];
                if (delta >= -1)
                {
                    g.add_edge({x,y}, {x,y - 1}, (delta == 0) ? std::abs(1) : std::abs(delta));
                }
            }

            // bottom
            if (y + 1 < HEIGHT)
            {
                int delta = map[y][x] - map[y + 1][x];
                if (delta >= -1)
                {
                    g.add_edge({x,y}, {x,y + 1}, (delta == 0) ? std::abs(1) : std::abs(delta));
                }
            }
        }
    }

    auto* dest = g.vertices[{end.x, end.y}];

    u64 min = 0xffffffffffffffff;

    for (i32 y = 0; y < HEIGHT; ++y)
    {
        for (i32 x = 0; x < WIDTH; ++x)
        {
            char c = map[y][x];
            if (c == 'a')
            {
                auto* source = g.vertices[{x, y}];

                g.reset();
                g.dijkstra(*source, *dest);

                auto shortest_path = g.shortest_path(*source, *dest);
                if (shortest_path.size() != 0)
                {
                    if (min > shortest_path.size() - 1)
                        min = shortest_path.size() - 1;
                    //cout << "[INFO] res part 2: " << shortest_path.size() - 1 << endl;
                }
                else
                {
                    //cout << "No shortest path found :(" << endl;
                }
            }
        }
    }
    
    cout << "[INFO] res part 2: " << min << endl;
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

