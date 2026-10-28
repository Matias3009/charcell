#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#include <algorithm>
#include <functional>

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

  struct button
  {
    SDL_FRect rect;
    bool active = true;
    bool down = false;
    std::function<void(bool)> call;
    std::string text;

    SDL_Color colors[5];
    enum class State{normal=0,
	       inactive=1,
	       hover=2,
	       pressed=3,
	       active=4};
    State state;
  };

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

  std::vector<button> buttons;

private:

  bool is_m_collide(SDL_FRect* rect);
  void draw_button(std::vector<button> btns);
  void btn_collision();
  void btn_event(SDL_Event e);
  
  SDL_Window* window;
  SDL_Renderer* renderer;

  int w_width = 0;
  int w_height = 0;
  float w_margin = 20.0f;

  SDL_FRect rect_plot;
  
  std::string w_name;

  std::array<SDL_FPoint, 1000> buffer;
  std::vector<data_c> c_buffer;
  std::uint16_t max_value;
  std::uint16_t min_value;
  
  std::atomic_flag can_copy{};
  
};
