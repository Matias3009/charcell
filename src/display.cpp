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

  init_sizes();
  
  std::cout << "- Ventana creada!" << std::endl;

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == nullptr){
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
    
    SDL_SetRenderDrawColor(renderer,
			   230,
			   230,
			   230,
			   SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    drawing();
    
    SDL_RenderPresent(renderer);
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

void display::drawing()
{
  draw_plot();
}

void display::draw_plot()
{
  SDL_SetRenderDrawColor(renderer,
			 0,
			 0,
			 0,
			 SDL_ALPHA_OPAQUE);
  
  SDL_RenderFillRectF(renderer,
		     &rect_plot);

  if (can_copy.test()){
    std::uint32_t first, last, diff;
    first = c_buffer.front().timestamp;
    last = c_buffer.back().timestamp;
    diff = last - first;

    std::uint16_t vdiff;
    vdiff = max_value-min_value;
    
    for (size_t i = 0;
	 i < std::min(buffer.max_size(), c_buffer.size());
	 i++){
      SDL_FPoint p;
      data_c d = c_buffer.at(i); // asegurar existencia de i en buffer
      
      p.x = (rect_plot.w / (float) diff) *
	(float) (d.timestamp-first);
      p.y = rect_plot.h -
	(rect_plot.h / (float) vdiff) *
	(float) (d.value-min_value);

      p.x += rect_plot.x;
      p.y += rect_plot.y;
      
      buffer.at(i) = p; // asegurar existencia de i en buffer
    }
    can_copy.clear();
    // Puede haber un error de escritura, asegurar luego!
  }

  // Aqui dibuja todo el grafico, se podria
  // separar en funciones pero meh
  if (c_buffer.size() < 2){
    return;
  }

  SDL_SetRenderDrawColor(renderer,
			 0,
			 255,
			 0,
			 SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLinesF(renderer,
		       buffer.data(),
		       std::min(buffer.max_size(), c_buffer.size()));

  
}

void display::init_sizes()
{
  SDL_GetWindowSize(window, &w_width, &w_height);
  rect_plot = {20, 20, w_width*(2/3.0f), w_height*(2/3.0f)};
}

bool display::set_plot(std::vector<data_c>* list,
		       std::uint16_t max,
		       std::uint16_t min)
{
  if (can_copy.test()){
    return false;
  }
  can_copy.clear();

  max_value = max;
  min_value = min;
  
  std::copy(list->begin(),
	    list->end(),
	    std::back_inserter(c_buffer));

  can_copy.test_and_set();
  return true;
}
