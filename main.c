#include "raylib.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

Color currentColor;
const int screenWidth = 800;
const int screenHeight = 600;
char *window_title;

int font_sizes[5];
int font_index = 0;

void event_loop(lua_State *lua_state) {
  InitWindow(screenWidth, screenHeight, window_title);

  SetTargetFPS(60);

  /* Main game loop */
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    lua_getglobal(lua_state, "love");

    int love_index = lua_gettop(lua_state);

    /* Inputs */
    if (IsKeyReleased(KEY_ENTER)) {
      lua_getfield(lua_state, love_index, "keyreleased");
      lua_pushstring(lua_state, "return");
      lua_call(lua_state, 1, 0);
    }

    /* Update */
    lua_getfield(lua_state, love_index, "update");
    lua_pushnumber(lua_state, dt);
    lua_call(lua_state, 1, 0);

    /* Draw */
    BeginDrawing();

    ClearBackground(BLACK);

    lua_getfield(lua_state, love_index, "draw");
    lua_call(lua_state, 0, 0);

    EndDrawing();
  }

  CloseWindow();
}

int window_set_title(lua_State *l) {
  char *title = lua_tostring(l, -1);
  window_title = title;
  SetWindowTitle(title);
  return 0;
}

int graphics_get_width(lua_State *l) {
  lua_pushnumber(l, screenWidth);
  return 1;
}

int graphics_get_height(lua_State *l) {
  lua_pushnumber(l, screenHeight);
  return 1;
}

int graphics_set_new_font(lua_State *l) {
  const char *name = lua_tostring(l, -2);
  lua_Number size = lua_tonumber(l, -1);
  font_sizes[font_index++] = size;
  return 0;
}

int graphics_set_color(lua_State *l) {
  lua_Number r = lua_tonumber(l, -3);
  lua_Number g = lua_tonumber(l, -2);
  lua_Number b = lua_tonumber(l, -1);

  currentColor = CLITERAL(Color){r * 255, g * 255, b * 255, 255};
  return 0;
}

int graphics_rectangle(lua_State *l) {
  const char *mode = lua_tostring(l, -5);
  lua_Number x = lua_tonumber(l, -4);
  lua_Number y = lua_tonumber(l, -3);
  lua_Number rx = lua_tonumber(l, -2);
  lua_Number ry = lua_tonumber(l, -1);

  DrawRectangle(x, y, rx, ry, currentColor);

  return 0;
}

int graphics_printf(lua_State *l) {
  const char *text = lua_tostring(l, -6);
  lua_Number font = lua_tonumber(l, -5);
  lua_Number x = lua_tonumber(l, -4);
  lua_Number y = lua_tonumber(l, -3);
  lua_Number w = lua_tonumber(l, -2);
  const char *align = lua_tostring(l, -6);

  DrawText(text, x, y, font_sizes[0] * 0.5, currentColor);

  return 0;
}

int graphics_print(lua_State *l) {
  const char *text = lua_tostring(l, -4);
  lua_Number font = lua_tonumber(l, -3);
  lua_Number x = lua_tonumber(l, -2);
  lua_Number y = lua_tonumber(l, -1);

  DrawText(text, x, y, font_sizes[1], currentColor);

  return 0;
}

int keyboard_is_down(lua_State *l) {
  const char *key = lua_tostring(l, -1);
  int comp_key;

  /* TODO Missing a HashMap to simplify this logic */
  if (strcmp(key, "right") == 0) {
    comp_key = KEY_RIGHT;
  } else if (strcmp(key, "d") == 0) {
    comp_key = KEY_D;
  } else if (strcmp(key, "left") == 0) {
    comp_key = KEY_LEFT;
  } else if (strcmp(key, "a") == 0) {
    comp_key = KEY_A;
  }

  lua_pushboolean(l, IsKeyDown(comp_key));

  return 1;
}

void register_love(lua_State *lua_state) {
  luaL_dostring(lua_state, "love = {}");
  luaL_dostring(lua_state, "love.graphics = {}");
  luaL_dostring(lua_state, "love.window = {}");
  luaL_dostring(lua_state, "love.keyboard = {}");
  luaL_dostring(lua_state, "love.keyboard.isDown = {}");

  /* Graphics  */
  int love_index = lua_getglobal(lua_state, "love");
  lua_getfield(lua_state, -1, "graphics");

  int graphics = lua_gettop(lua_state);

  lua_pushcfunction(lua_state, graphics_get_width);
  lua_setfield(lua_state, graphics, "getWidth");

  lua_pushcfunction(lua_state, graphics_get_height);
  lua_setfield(lua_state, graphics, "getHeight");

  lua_pushcfunction(lua_state, graphics_set_new_font);
  lua_setfield(lua_state, graphics, "setNewFont");

  lua_pushcfunction(lua_state, graphics_set_color);
  lua_setfield(lua_state, graphics, "setColor");

  lua_pushcfunction(lua_state, graphics_rectangle);
  lua_setfield(lua_state, graphics, "rectangle");

  lua_pushcfunction(lua_state, graphics_printf);
  lua_setfield(lua_state, graphics, "printf");

  lua_pushcfunction(lua_state, graphics_print);
  lua_setfield(lua_state, graphics, "print");

  /* Window */
  lua_getglobal(lua_state, "love");
  lua_getfield(lua_state, -1, "window");

  int window = lua_gettop(lua_state);

  lua_pushcfunction(lua_state, window_set_title);
  lua_setfield(lua_state, window, "setTitle");

  lua_getglobal(lua_state, "love");
  lua_getfield(lua_state, -1, "keyboard");
  int keyboard = lua_gettop(lua_state);

  lua_pushcfunction(lua_state, keyboard_is_down);
  lua_setfield(lua_state, keyboard, "isDown");
}

void init_game(lua_State *lua_state) {
  int game_status = luaL_dofile(lua_state, "breakout.lua");
  if (0 != game_status) {
    printf("dofile error %i\n", game_status);
    return;
  }
}

int main(int argc, char **argv) {
  lua_State *lua_state = luaL_newstate();
  luaL_openlibs(lua_state);
  luaopen_table(lua_state);

  register_love(lua_state);

  init_game(lua_state);
  event_loop(lua_state);

  lua_close(lua_state);

  return 0;
}
