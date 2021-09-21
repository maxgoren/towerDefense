class towerAi {
    public:
    std::vector<tower*> towers;
    tower* fort;
    towerAi();
    void placeTower(int mx, int my, board* m);
    void defend(board* m);
    void render(board* m);
};

towerAi::towerAi()
{

}

void towerAi::placeTower(int mx, int my, board* m)
{
  m->placeTower(mx, my, 1);
  fort = new tower(mx, my);
  towers.push_back(fort);
}

void towerAi::defend(board* m)
{
    for (auto t : towers)
    {
        if (t->fired.empty())
        {
          t->shoot(m);
        }
    }
}

void towerAi::render(board* m)
{
    for (auto t : towers) 
    {
      int i = 0;
      for (auto bul : t->fired)
      {
       if (bul->go)
       {
        bul->render(m);
       } else {
        bullet* temp = bul;
        t->fired.erase(t->fired.begin()+i);
        delete temp;
       }
       i++;
      }
    }
}

class goblinAi {
    public:
    int numBG;
    std::vector<Entity*> theHorde;
    std::vector<tile> walkable;
    Entity* bad_guy;
    goblinAi(int numBadguys);
    void init(board* m);
    bool canWalk(tile t) { return t.blocking; }
    void getWalkable(board* m);
    void render();
    void take_turn(board* m, Point player_location);
    void dijkstra_map(Point origin, board* m);
    void moveGoblin(Entity* goblin, board* m);
    void moveGoblin(Entity* goblin, board* m, Point player_location);
};

goblinAi::goblinAi(int numBadguys)
{
    numBG = numBadguys;
}
void goblinAi::init(board* m)
{
   getWalkable(m);
   int bg_x, bg_y, random_spot;
   for (auto i = 0; i < numBG; i++)
   {
       random_spot = rand() % (walkable.size() - 3) - 1;
       bg_x = walkable.at(random_spot).loc.x;
       bg_y = walkable.at(random_spot).loc.y;
       bad_guy = new Entity('G', {bg_x,bg_y}, "Horde Goblin " + to_string(i));
       theHorde.push_back(bad_guy);
   }
}

void goblinAi::render()
{
  for (auto goblin : theHorde)
  {
      goblin->draw();
  }
}
void goblinAi::take_turn(board* m, Point player_location)
{
    int i = 0;
    for (auto goblin : theHorde)
    {
        moveGoblin(goblin, m);
    }
}
void goblinAi::getWalkable(board* m)
{
   walkable.clear();
   int x, y;
   for (x = map_width-1; x > map_width-20; x--)
   {
       for (y = map_height-1; y > map_height-10; y--)
       {
           if (m->world[x][y].blocking == false)
           {
               walkable.push_back(m->world[x][y]);
           }
       }
   }
   for (x = 2; x < map_width/2; x++)
   {
       for (y = 6; y < (map_height/2)-10; y++)
       {
           if (inbounds(x, y) && m->world[x][y].blocking == false)
           {
               walkable.push_back(m->world[x][y]);
           }
       }
   }
}

//version to use dijkstra map
void goblinAi::moveGoblin(Entity* g, board* m)
{
    Dirs compass;
    Point curr = g->loc, next;
    int best = m->world[g->loc.x][g->loc.y].distance + 2;
    int nx, ny;
    for (auto d : compass.dir)
    {
        nx = curr.x+d.x;
        ny = curr.y+d.y;
        if (inbounds(nx, ny)) {
         if (m->world[nx][ny].distance < best && !m->world[nx][ny].blocking)
         {
          best = m->world[nx][ny].distance;
          next = d;
         }
        }
    }
    g->move(next.x,next.y,m);
}


void goblinAi::dijkstra_map(Point origin, board* m)
{
    bool found;
    tile curr;
    Dirs d;
    int nx, ny;
    curr.loc = {origin.x, origin.y};
    unordered_map<tile, tile> camefrom;
    queue<tile> q;
    camefrom[curr] = curr;
    m->world[origin.x][origin.y].distance = 0;
    q.push(curr);
    while (!q.empty())
    {
        curr = q.front();
        q.pop();
        for (auto i : d.dir)
        {
            nx = curr.loc.x + i.x;
            ny = curr.loc.y + i.y;
            if (inbounds(nx, ny)) 
            {
               if (m->world[nx][ny].blocking == false)
               {
                if(camefrom.find(m->world[nx][ny]) == camefrom.end())
                {
                  q.push( m->world[nx][ny]);
                  camefrom[m->world[nx][ny]] = curr;
                  m->world[nx][ny].distance = m->world[curr.loc.x][curr.loc.y].distance + 1;
                }
               }
            }
        }
    }
}