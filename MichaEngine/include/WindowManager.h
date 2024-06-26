//
// Created by Mikhail Chalakov on 2/18/24.
//

#pragma once
#include <SDL2/SDL.h>

#include <memory>

#include "Types.h"
#include "imgui.h"

struct SDLDeleter {
  void operator()(SDL_Window *w) const { SDL_DestroyWindow(w); }
  void operator()(SDL_Renderer *w) const { SDL_DestroyRenderer(w); }
  void operator()(SDL_Texture *w) const { SDL_DestroyTexture(w); }
};

class WindowManager {
  std::unique_ptr<SDL_Window, SDLDeleter> window;
  std::unique_ptr<SDL_Renderer, SDLDeleter> renderer;
  std::unique_ptr<SDL_Texture, SDLDeleter> background;
  SDL_Event event;
  type::Vector2i windowSize;
  static type::Vector2i getMonitorSize();
  ImGuiIO io;
  SDL_GLContext gl_context;
  bool imgui_open;

 public:
  int frameCount;
  void setBackground(SDL_Texture *bkg);
  int draw(SDL_Texture *txt, const SDL_Rect *src, const SDL_Rect *dst);
  int draw(type::Object *object);
  int draw(type::Sprite *sprite);

  type::Vector2i getAbsolutePosition(type::Vector2i pos);
  type::Vector2i getCenter();

  bool hasQuit();

  void update();

  WindowManager(const std::string &windowName, type::Vector2i pos, Uint32 flag);

  void debugFrame();
  ~WindowManager();
};
