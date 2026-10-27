#include "display.hpp"

display::display()
{
  w_name = "test celda de carga - v2.0";
}

bool display::initialize()
{
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0){
    std::cerr << "- No se pudo iniciar SDL!" << std::endl;
    return false;
  }

  std::cout << "- Iniciando SDL" << std::endl;

  window = SDL_CreateWindow(w_name.c_str(),
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    WIDTH,
			    HEIGHT,
			    WIN_TYPE);
  
  if (window == nullptr){
    std::cerr << "- No se pudo crear la ventana!" << std::endl;
    return false;
  }

  std::cout << "- Ventana creada!" << std::endl;

  surface = SDL_GetWindowSurface(window);

  if (surface == nullptr){
    std::cerr <<
      "- No se pudo usar la superficie de la ventana!"
	      << std::endl;
    return false;
  }

  return true;
}

void display::main_loop()
{
  SDL_Event e;
  bool running = true;

  while (running){
    while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT)
	running = false;
    }
    
  }

  terminate();
  return;
}

void display::terminate()
{
  SDL_DestroyWindow(window);
  SDL_Quit();
  return;
}
