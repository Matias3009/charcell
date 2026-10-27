#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <algorithm>

#include "structs.hpp"

#ifndef WIDTH
#define WIDTH 600
#endif

#ifndef HEIGHT
#define HEIGHT 600
#endif

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
  void drawing();
  void draw_plot();
  void init_sizes();
  bool set_plot(std::vector<data_c>* list,
		std::uint16_t max,
		std::uint16_t min);

private:

  SDL_Window* window;
  SDL_Renderer* renderer;

  int w_width = 0;
  int w_height = 0;

  SDL_FRect rect_plot;
  
  std::string w_name;

  std::array<SDL_FPoint, 1000> buffer;
  std::vector<data_c> c_buffer;
  std::uint16_t max_value;
  std::uint16_t min_value;
  
  std::atomic_flag can_copy{};
  
};
