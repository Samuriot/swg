// renderer.h
#ifndef RENDERER_H 
#define RENDERER_H

#include <memory>
#include <vector>
#include <SDL.h>

#include "object.h"

class SDL_Window_Deleter {
  public:
    void operator()(SDL_Window* window) const {
      SDL_DestroyWindow(window);
    }
};

class SDL_Renderer_Deleter {
  public:
    void operator()(SDL_Renderer* renderer) const {
      SDL_DestroyRenderer(renderer);
    }
};

class Renderer {
  private:
    std::unique_ptr<SDL_Window, SDL_Window_Deleter> window;
    std::unique_ptr<SDL_Renderer, SDL_Renderer_Deleter> renderer;
    std::vector<std::unique_ptr<Object>> entities;
    bool running;
    uint8_t delay;

    bool run_event_loop();
  public:
    Engine(int w, int h, uint8_t delay);
    ~Engine();
    void init(int w, int h);
    void run();
    void shutdown();
};

#endif // RENDERER_H
