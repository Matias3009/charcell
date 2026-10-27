#include <SDL2/SDL.h>

#include <iostream>
#include <string>

#define WIDTH 600
#define HEIGHT 600
#define WIN_TYPE SDL_WINDOW_SHOWN

#ifdef FULLSCREEN

#define WIN_TYPE SDL_WINDOW_FULLSCREEN

#endif

class display
{
public:

  display();

  bool initialize();
  void main_loop();
  void terminate();

private:

  SDL_Window* window;
  SDL_Surface* surface;

  std::string w_name;
  
  
};
