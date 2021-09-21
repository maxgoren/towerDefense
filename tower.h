#include <map>
#include <vector>
using namespace std;
class tower {
    public:
    Point loc;
    vector<Point> direc;
    vector<bullet*> fired;
    bullet *b;
    tower(int x, int y);
    std::pair<bool, Point> targetScan(board* m);
    void shoot(board* m);
};

tower::tower(int x, int y)
{
    loc.x = x;
    loc.y = y;
    direc.push_back({0,-1});
    direc.push_back({0, 1});
    direc.push_back({-1, 0});
    direc.push_back({1, 0});
    direc.push_back({-1, -1});
    direc.push_back({1,1});
    direc.push_back({-1,1});
    direc.push_back({1, -1});
}

void tower::shoot(board* m)
{
    auto [targeting, target] = targetScan(m);
    if (targeting)
    {
        b = new bullet(loc, target);
        fired.push_back(b);
    }
}

std::pair<bool, Point> tower::targetScan(board* m) 
{
    struct target {
        Point loc;
        int distance;
        target(Point l, int d) : loc(l), distance(d) { } 
    }; 
    int x, y; 
    map<Point, bool> seen;
    Point next;
    queue<target*> radar;
    target* curr = new target(loc, 0);
    seen[curr->loc] = true;
    radar.push(curr);
    while (!radar.empty())
    {
        curr = radar.front(); radar.pop();
        x = curr->loc.x;
        y = curr->loc.y;
        if (m->world[x][y].populated)
        {
            return make_pair(true, curr->loc);
        }
        for (auto d : direc)
        {
            next = {curr->loc.x + d.x, curr->loc.y + d.y};
            if (inbounds(next.x, next.y) && seen[next] == false)
            {
                if (curr->distance + 1 < 10)
                {
                   radar.push(new target(next, curr->distance + 1));
                }
            }
        }
    }
    next = {0,0};
    return make_pair(false, next);
}