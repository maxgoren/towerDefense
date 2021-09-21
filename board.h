enum terrain_t {
  GRASS,
  DIRT,
  ROCK,
  FORTRESS
};
struct tile {
  Point loc;
  char label;
  bool blocking;
  bool populated;
  int distance;
  terrain_t terrain;
  //tile(int x, int y) : loc.x(x), loc.y(y) { }
  tile() { distance = 0; blocking = false; label = '.'; }
  //tile(const tile&) { }
  bool operator==(const tile& o) const {
    return loc.x == o.loc.x && loc.y == o.loc.y;
  }
  tile operator=(tile& o) {
    loc.x = o.loc.x;
    loc.y = o.loc.y;
    label = o.label;
    blocking = o.blocking;
    return o;
  }
};

bool inbounds(int& x, int& y)
{
   return (x > 0 && x < map_width &&
          y > 4 && y < map_height);
}

class board {
  public:
  vector<vector<tile>> world;    
  vector<Room*> rooms;
  int numRooms;
  int mapw;
  int maph;
  int level;
  board(int x, int y)
  {
    mapw = x;
    maph = y;
    world = vector<vector<tile>>(mapw, vector<tile>(maph, tile{}));
      for (y = 4; y < maph; y++)
      {
      for (x = 0; x < mapw; x++)
      {
         world[x][y].loc.x = x;
         world[x][y].loc.y = y;
         world[x][y].blocking = false;
         if (x % 3 == 0)
         {
           world[x][y].terrain = DIRT;
           world[x][y].label = '.';
         } else {
           world[x][y].terrain = GRASS;
           world[x][y].label = ',';
         }
         
      }
     }
  }
  void makeObstacles(int level);
  void placeTower(int x, int y, int size);
  void dig(const Rect&);
  void draw();
};

void board::makeObstacles(int level)
{

  int nx, ny, nh,nw;
  int maxW = 12;
  int maxH = 11;
  int roomsMade = 0;
  int rid;
  Room *posRoom;
  for (int a = 0; a < 30; a++)
  {
    nx = getrand(2, mapw -maxW- 2);
      ny = getrand(2, maph - maxH- 2);
      nh = getrand(3, maxH);
      nw = getrand(3, maxW);
      posRoom = new Room(nx,ny,nw,nh);
      if (roomsMade < 1)
      {
        rid = level+roomsMade+1;
        posRoom->id = rid;
         posRoom->cent.x = (posRoom->loc.uL.x + posRoom->loc.lR.x) / 2;
          posRoom->cent.y = (posRoom->loc.uL.y + posRoom->loc.lR.y) / 2;
        rooms.push_back(posRoom);
        roomsMade++;
      } else {
        bool retry = false;
        for (auto r : rooms)
        {
          if (posRoom->collides(*r)) 
          {
            retry = true;  
            break;
          }
        }
        if (retry)
        {
          a--;
        } else {
          rid = level+roomsMade+1;
        posRoom->id = rid;
          posRoom->cent.x = (posRoom->loc.uL.x + posRoom->loc.lR.x) / 2;
          posRoom->cent.y = (posRoom->loc.uL.y + posRoom->loc.lR.y) / 2;
          rooms.push_back(posRoom);
          roomsMade++;
        }
      }        
  }
  for (auto r : rooms)
    dig(r->loc);

  numRooms = roomsMade;
  delete posRoom;
  placeTower(map_width/2, map_height/2, 6);
}

//DigDig!!
void board::dig(const Rect& r)
{
  cout<<endl;
  for (int x = r.uL.x; x < r.lR.x; x++)
    for (int y = r.uL.y; y < r.lR.y; y++)
    {
      world[x][y].blocking = true;
      world[x][y].label = '~';
      world[x][y].terrain = ROCK;
    }
}
void board::placeTower(int x, int y, int size)
{
  int cx, cy;
  int dx, dy, dist;
  cx = x;
  cy = y;
  for (auto a = cx - 10; a < cx+11; a++)
  {
    for (auto b = cy-6; b < cy+6; b++)
    {
      dx = abs(a - cx);
      dy = abs(b - cy);
      dist = sqrt(pow(dx,2) + pow(dy, 2));
      if (dist == size)
      {
        world[a][b].label = '#';
        world[a][b].blocking = true;
        world[a][b].terrain = FORTRESS;
      }
      if (dist <= size-1)
      {
        world[a][b].label = ' ';
        world[a][b].blocking = false;
        world[a][b].terrain = FORTRESS;
      }
    }
  }
}

void board::draw()
{
      string dist = " 123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
      terminal_bkcolor(color_from_name("black"));
      for (int y = 4; y < maph; y++)
        {
          for (int x = 0; x < mapw; x++)
          {
            switch (world[x][y].terrain)
            {
              case GRASS:
                 terminal_color(color_from_name("green"));
                 break;
              case DIRT:
                 terminal_color(color_from_name("brown"));
                 break;
              case ROCK:
                 terminal_color(color_from_name("grey"));
                 break;
              case FORTRESS:
                 terminal_color(color_from_name("dark grey"));
                 break;
              default:
                 terminal_color(color_from_name("white"));
            }
              terminal_put(x,y,world[x][y].label);
          }
      }
}