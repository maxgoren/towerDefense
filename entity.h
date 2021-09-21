class Entity {
    public:
    char label;
    Point loc;
    string name;
    char face;
    std::unordered_map<char, Point> facing;
    void move(int dx, int dy, board* m);
    Entity(char l, Point pos, string n);
    std::vector<tile> pathfind(Point t, board* m);
    void draw();
};

Entity::Entity(char l, Point pos, string n)
{
    facing['N'] = {0,-1};
    facing['S'] = {0, 1};
    facing['E'] = {1,0};
    facing['W'] = {-1,0}; 
    label = l;
    loc = pos;
    name = n;
}

void Entity::move(int dx, int dy, board* map)
{
    int px = loc.x;
    int py = loc.y;
    int nx = px + dx;
    int ny = py + dy;
    if (inbounds(nx, ny) && !map->world[nx][ny].blocking) {
    map->world[loc.x][loc.y].blocking = false;
    map->world[loc.x][loc.y].populated = false;
    loc.x += dx;
    loc.y += dy;
    map->world[loc.x][loc.y].blocking = true;
    map->world[loc.x][loc.y].populated = true;
    }
    for (auto f : facing)
    {
        if (Point{dx,dy} == f.second)
          face = f.first;
    }
}


void Entity::draw()
{
    terminal_color(0xFF00FF00);
    terminal_put(loc.x, loc.y, label);
}

namespace std {
template <> struct hash<tile> {
  std::size_t operator()(const tile& id) const noexcept {
    return std::hash<int>()(id.loc.x ^ (id.loc.y << 4));
  }
};
}
std::vector<tile> Entity::pathfind(Point t, board* m)
{
    bool found;
    tile curr;
    tile adj;
    Dirs d;
    int nx, ny;
    std::vector<tile> path;
    unordered_map<tile, tile> camefrom;
    queue<tile> q;
    curr.loc = loc;
    q.push(curr);
    while (!q.empty())
    {
        curr = q.front();
        q.pop();
        if (curr.loc == t)
        {
            found = true;
            break;
        }
        for (auto i : d.dir)
        {
            nx = curr.loc.x + i.x;
            ny = curr.loc.y + i.y;
            if (inbounds(nx, ny)) {
            
            if (m->world[nx][ny].blocking == false)
            {
            if(camefrom.find(m->world[nx][ny]) == camefrom.end())
            {
                q.push( m->world[nx][ny]);
                camefrom[m->world[nx][ny]] = curr;
            }
            }
            }
        }
    }
    if (found)
    {
        path.clear();
        tile crawl;
        crawl.loc = t;
        while (crawl.loc != loc)
        {
            path.push_back(crawl);
            crawl = camefrom[crawl];
        }
    }
    return path;
}