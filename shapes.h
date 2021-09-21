struct Point {
  int x;
  int y;
  
  bool operator==(const Point& o) {
    return x == o.x && y == o.y;
  }
  bool operator!=(const Point& o) {
    return !(*this == o);
  }
  Point operator=(const Point& o) {
    x = o.x;
    y = o.y;
    return *this;
  }
  bool operator<(const Point& o) const {
    return (this->x < o.x && this->y < o.y);
  }
};


class Rect {
   public:
     Point uL;
     Point lR;
     Rect(Point u, Point l)
     {
       uL = u;
       lR = l;
     }
     Rect(int x, int y, int w, int h)
     {
       uL.x = x;
       uL.y = y;
       lR.x = x+w;
       lR.y = y+h;
     }
     Rect() { }
     Rect(const Rect&) { }
 };