class bullet {
  public:
    Point loc;
    Point target; 
    Point impact;
    bool hit_target;
    bool go;
    bullet(Point start, Point finish);
    void updatePos(board* m);
    void render(board* m);
    Point getNext(board* m, Point p1, Point p2);
    ~bullet();
};

bullet::bullet(Point start, Point finish) 
{
    loc = start;
    target = finish;
    hit_target = false;
    go = true;
}

bullet::~bullet()
{

}

Point bullet::getNext(board* m, Point p1, Point p2)
{
    int dx, dy, sx, sy, err, e2;
                        
    dx = abs(p2.x - p1.x);
    sx = (p1.x < p2.x) ? 1:-1;
    dy = -abs(p2.y - p1.y);
    sy = (p1.y < p2.y) ? 1:-1;
    err = dx+dy;
                        
    if (p1 == p2)
        return p1;
                              
    if (inbounds(p1.x, p1.y)) 
    {
        e2 = 2*err;
        if (e2 >= dy)
        {
            err += dy;
            p1.x += sx;
        }
        if (e2 <= dx)
        {
              err += dx;
              p1.y += sy;
        }
            return p1;
    }
}

void bullet::updatePos(board* m)
{
    Point Next;
    Next = getNext(m, loc, target);
    loc = Next;
    if (inbounds(Next.x, Next.y)) {
      
        if (m->world[Next.x][Next.y].populated)
        {
            hit_target = true;
            impact = Next;
            go = false;
        }
    } else if (!inbounds(Next.x,Next.y)) {
          go = false; 
    } else {
        go = true;
    }
}


void bullet::render(board* m)
{
    if (go == true)
    {
          updatePos(m);
          cout<<"Round at: "<<loc.x<<"/"<<loc.y<<endl;
          terminal_color("cyan");
          terminal_put(loc.x,loc.y, '*');
    }
}