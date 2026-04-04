#include "engine.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <SDL.h>

Engine::Engine(int w, int h, uint8_t delay) {
  this->init(w, h);
  this->running = true;
  this->delay = delay;
}

void Engine::init(int w, int h) {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error(
      std::string("SDL initialization failed: ") + SDL_GetError()
    );
  }
  std::cout << "SDL successfully started" << std::endl;

  this->window = std::unique_ptr<SDL_Window, SDL_Window_Deleter>(
    SDL_CreateWindow(
      "Game Engine",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      w,
      h,
      SDL_WINDOW_SHOWN
    )
  );

  if(!window) {
    std::cerr << "Window Creation Failed: " << SDL_GetError() << std::endl;
    this->shutdown();
  }
  
  std::cout << "Window successfully created" << std::endl;

  this->renderer = std::unique_ptr<SDL_Renderer, SDL_Renderer_Deleter>(
    SDL_CreateRenderer(
      this->window.get(), -1, 0
    )
  );

  if(!renderer) {
    std::cerr << "Renderer Creation Failed: " << SDL_GetError() << std::endl;
    this->shutdown();
  }

  std::cout << "Renderer successfully created" << std::endl;
}

bool Engine::run_event_loop() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if(event.type == SDL_QUIT) {
      return false;
    }
  }
  return true;
}

void Engine::run() {
  while(this->running) {
    std::cout << "in loop" << std::endl;

    this->running = this->run_event_loop();

    SDL_SetRenderDrawColor(
      this->renderer.get(),
      0xFF,
      0xFF,
      0xFF,
      0xFF
    );

    SDL_RenderClear(this->renderer.get());

    SDL_RenderPresent(this->renderer.get());
    SDL_Delay(this->delay);
  }

  this->shutdown();
}

void Engine::shutdown() {
  SDL_Quit();
}

Engine::~Engine() {
  this->shutdown();
}
