class Room {
   public:
   Rect loc;
   Point cent;
   int id;
   bool collides(const Room& other);
    Room(int, int, int, int);
 };
 Room::Room(int x, int y, int w, int h)
 {
  loc.uL.x = x;
  loc.uL.y = y;
  loc.lR.x = x+w;
  loc.lR.y = y+h;
 }
bool Room::collides(const Room& other)
{
 if ((loc.uL.x < other.loc.lR.x) && (loc.lR.x > other.loc.uL.x) &&
     (loc.uL.y < other.loc.lR.y) && (loc.lR.y > other.loc.uL.y)) {
     return true; 
  } else {
  return false;
 }
}