enum GameState {
  PLAYER_TURN,
  BUILD_TOWER,
  AI_TURN
};

class engine {
  public:
  board *m;
  Entity *player;
  goblinAi *robot;
  towerAi *outposts;
  GameState gs;
  int numTowers;
  int scrx, scry;
  int px,py;
  engine(int x, int y)
  {
    scrx = x;
    scry = y;
    numTowers = 0;
  }
  void init();
  void gameLoop();
  void handleinput(char cm);
  void render();
  void drawStatusBox();
};

void engine::init()
{
  m = new board(scrx,scry);
  m->makeObstacles(30);
  px = map_width/2;
  py = map_height/2;
  player = new Entity('@', {px, py}, "il Doctore");
  robot = new goblinAi(20);
  robot->init(m);
  outposts = new towerAi;
}

void engine::handleinput(char cm)
{
  int mx, my;
  switch (cm)
  {
    case TK_UP:
       player->move(0, -1, m); break;
    case TK_DOWN:
       player->move(0, 1, m); break;
    case TK_LEFT:
       player->move(-1, 0, m); break;
    case TK_RIGHT:
       player->move(1, 0, m); break;
    case TK_SPACE:
       mx = terminal_state(TK_MOUSE_X);
       my = terminal_state(TK_MOUSE_Y);
       outposts->placeTower(mx, my, m); 
       numTowers++;
       break;
    case TK_Q:
      terminal_close();
      exit(0);
    default:
    break;
  }
}
void engine::gameLoop()
{
  bool inplay = true;
  Point goal;
  char cm;
  while (inplay)
  {
    if (terminal_has_input())
    {
     cm = terminal_read();
     if (gs == PLAYER_TURN)
     {
       handleinput(cm);
       outposts->defend(m);
       gs = AI_TURN;
     }
    }
    if (gs == AI_TURN)
    {
      robot->dijkstra_map(player->loc, m);
      goal = player->loc;
      cout<<"target: "<<goal.x<<"/"<<goal.y<<endl;
      robot->take_turn(m, goal);
      gs = PLAYER_TURN;
    }
    render();
  }
}

void engine::drawStatusBox()
{
  for (auto x = 0; x < map_width; x++)
  {
    for (auto y = 0; y < 4; y++)
    {
      terminal_layer(0);
      terminal_bkcolor(color_from_name("darker blue"));
      terminal_print(x, y, " ");
    }
  }
  terminal_color(color_from_name("white"));
  terminal_printf(5, 2, "number of towers: %d", numTowers);
}

void engine::render()
{
  terminal_clear();
  drawStatusBox();
  m->draw();
  player->draw();
  robot->render();
  outposts->render(m);
  terminal_refresh();
}
