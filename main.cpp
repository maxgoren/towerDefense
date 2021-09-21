
#include "headers.h" 
  
using namespace std; 

// main function - 

int main(int argc, char *argv[])
{
  int x=map_width, y=map_height;
  terminal_open();
  terminal_set("window: size=80x40");
  engine e(x,y);
  e.init();
  e.gameLoop();
  terminal_close();
  return EXIT_SUCCESS;
}