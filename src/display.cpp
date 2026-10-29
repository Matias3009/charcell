#include "display.hpp"
#include "font_data.h"

SDL_Color n2c(Uint32 colorInt) {
    SDL_Color c;
    c.r = (colorInt >> 24) & 0xFF;
    c.g = (colorInt >> 16) & 0xFF;
    c.b = (colorInt >> 8)  & 0xFF;
    c.a = colorInt         & 0xFF;
    return c;
}

display::display()
{
  w_name = "test celda de carga - v2.0";

  buttons.push_back({{0,0,10,10},
      false,
      false,
      NULL,
      "START",
      {n2c(0xCFCFCFFF),
       n2c(0xF2F2F2FF),
       n2c(0xBABABAFF),
       n2c(0x969696FF),
       n2c(0xC71E1EFF)},
      button::State::normal}); // START

  buttons.push_back({{0,0,10,10},
      true,
      false,
      NULL,
      "connect",
      {n2c(0xCFCFCFFF),
       n2c(0xF2F2F2FF),
       n2c(0xBABABAFF),
       n2c(0x969696FF),
       n2c(0xC71E1EFF)},
      button::State::normal}); // connect

  buttons.push_back({{0,0,10,10},
      false,
      false,
      NULL,
      "calibrar",
      {n2c(0xCFCFCFFF),
       n2c(0xF2F2F2FF),
       n2c(0xBABABAFF),
       n2c(0x969696FF),
       n2c(0xC71E1EFF)},
      button::State::normal}); // calibrar

 
}

bool display::initialize()
{
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0){
    std::cerr << "- No se pudo iniciar SDL!" << std::endl;
    return false;
  }

  if (TTF_Init() < 0){
    std::cerr << "- No se pudo iniciar SDL_ttf!" << std::endl;
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

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  init_sizes();
  
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
  std::uint64_t tick = SDL_GetTicks64();
  
  while (running){

    tick = SDL_GetTicks64();
    btn_collision();
    
    while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT)
	running = false;

      switch (e.type){
      case SDL_KEYDOWN:
	key_down_e(e);
	break;
      }

      btn_event(e);
    }
    
    SDL_SetRenderDrawColor(renderer,
			   255,
			   255,
			   255,
			   SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    drawing();
    draw_button(buttons);
    draw_texts();
    
    SDL_RenderPresent(renderer);

    SDL_Delay(40 - (SDL_GetTicks64() - tick));
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
  rect_plot = {w_margin, w_margin, w_width*(2/3.0f), w_height*(2/3.0f)};

  buttons.at(0).rect = {w_width - w_margin - 200,
			w_height - w_margin - 100,
			200,
			100}; // START
  
  buttons.at(1).rect = {w_margin,
			w_height - w_margin - 40,
			100,
			40}; // connect


  rw = SDL_RWFromMem(fonts_LiberationSans_Bold_ttf,
		     fonts_LiberationSans_Bold_ttf_len);

  if (!rw){
    std::cerr << "- Fallo al cargar memoria de fuentes!" << std::endl;
  }
  
  font = TTF_OpenFontRW(rw, 1, 144);

  if (font == NULL){
    std::cerr << "- Fallo al cargar las fuentes!" << std::endl;
    std::cerr << TTF_GetError() << std::endl;
  }

  for (auto& b : buttons){
    SDL_Surface* surface = TTF_RenderText_Blended(font,
						  b.text.c_str(),
						  {20,
						   20,
						   20,
						   SDL_ALPHA_OPAQUE});

    if (surface == NULL){
      std::cerr << "- Fallo al generar textura!" << std::endl;
      std::cerr << SDL_GetError() << std::endl;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    if (tex == NULL){
      std::cerr << "- No se pudo crear la textura del texto: "
		<< SDL_GetError() << std::endl;
    }

    b.texture_text = tex; 
    
    SDL_FreeSurface(surface);
  }

  texts.push_back({NULL, {w_margin,
	w_height - w_margin - 85,
	100,
	20}});
  set_texts(0, "port_c");
  
  
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

bool display::is_m_collide(SDL_FRect* rect)
{
  int mx, my;
  SDL_GetMouseState(&mx, &my);

  if (mx >= rect->x && mx <= rect->x + rect->w)
    if (my >= rect->y && my <= rect->y + rect->h)
      return true;
  
  return false;
}

void display::draw_button(std::vector<button> btns)
{
  for (auto& btn : btns){
    int btn_state = static_cast<int>(btn.state);
    
    SDL_SetRenderDrawColor(renderer,
			   btn.colors[btn_state].r,
			   btn.colors[btn_state].g,
			   btn.colors[btn_state].b,
			   btn.colors[btn_state].a);
    SDL_RenderFillRectF(renderer,
			&btn.rect);

    if (SDL_RenderCopyF(renderer, btn.texture_text, NULL,&btn.rect) < 0){
      std::cerr << "- No se pudo dibujar la fuente: " << SDL_GetError()
		<< std::endl;
    }
  }
}

void display::btn_collision()
{
  for (auto& btn : buttons){
    if (btn.state == button::State::pressed)
      continue;
    if (!btn.active){
      btn.state = button::State::inactive;
      continue;
    }
    else if (btn.down){
      btn.state = button::State::active;
      continue;
    }
    
    if (is_m_collide(&btn.rect)){
      btn.state = button::State::hover;
      continue;
    }
    else{
      btn.state = button::State::normal;
    }
  }
}

void display::btn_event(SDL_Event e)
{
  for (auto& btn : buttons){
    if (e.type == SDL_MOUSEBUTTONDOWN){
      if (btn.state == button::State::hover && btn.active){
	btn.state = button::State::pressed;
	btn.call(btn.active);
	continue;
      }
    }
    if (e.type == SDL_MOUSEBUTTONUP){
      if (btn.state == button::State::pressed && btn.active){
	btn.state = button::State::hover;
	continue;
      }
    }
  }
}

void display::set_ports(std::vector<std::string> p, int n)
{
  ports_name = p;
  port_now = n;
  if (static_cast<std::size_t>(n) < ports_name.size()){
    port_c = ports_name.at(n);
    set_texts(0, port_c);
  }
}

void display::next_port(int n)
{
  if (n > 0){
    if (static_cast<std::size_t>(port_now + 1) < ports_name.size()){
      port_now += 1;
    }     
  }
  else if (n < 0){
    if (port_now - 1 >= 0){
      port_now -= 1;
    }
  }

  port_c = ports_name.at(port_now);
  set_texts(0, port_c);
}

void display::key_down_e(SDL_Event e){
  if (e.type != SDL_KEYDOWN)
    return;

  switch (e.key.keysym.sym){
  case SDLK_LEFT:
    next_port(-1);
    break;
    
  case SDLK_RIGHT:
    next_port(1);
    break;
  }
}

void display::set_texts(int n, std::string s)
{
  SDL_Surface* surface = TTF_RenderText_Blended(font,
					        s.c_str(),
						{20,
						 20,
						 20,
						 SDL_ALPHA_OPAQUE});

  if (surface == NULL){
    std::cerr << "- Fallo al generar textura!" << std::endl;
    std::cerr << SDL_GetError() << std::endl;
  }

  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
  if (tex == NULL){
    std::cerr << "- No se pudo crear la textura del texto: "
	      << SDL_GetError() << std::endl;
  }

  texts.at(n).texture_text = tex; 
}

void display::draw_texts()
{
  for (auto& t : texts){

    if (SDL_RenderCopyF(renderer, t.texture_text, NULL,&t.rect) < 0){
      std::cerr << "- No se pudo dibujar la fuente (texto): "
		<< SDL_GetError()
		<< std::endl;
    }
  }
}

void display::start_plotting()
{
  buttons.at(0).active = true;
  buttons.at(1).active = false;
}

void display::stop_plotting()
{
  
}
