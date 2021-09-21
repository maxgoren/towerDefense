using namespace std;
enum cardinal {
   NORTH = 0,
   SOUTH,
   EAST,
   WEST
};
struct Dirs {
    vector<Point> dir;
    cardinal d;
    Dirs()
    {
      dir.resize(4);
      dir[NORTH] = {0,-1}; 
      dir[SOUTH] = {0, 1}; 
      dir[EAST] = {1, 0}; 
      dir[WEST] = {-1,0};
    }
};