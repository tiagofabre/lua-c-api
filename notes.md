# Annotations

- Lua download page: https://www.lua.org/download.html
  - How to build: https://www.lua.org/manual/5.4/readme.html
- Introduction to C API: https://sodocumentation.net/lua/topic/671/introduction-to-lua-c-api
- Syntax sugar: https://www.lua.org/manual/5.4/manual.html#5
- There is a fantasy console written in lua - PICO-8: https://www.lexaloffle.com/pico-8.php
- Examples of how to integrate c++ with lua: https://www.codingwiththomas.com/blog/a-lua-c-api-cheat-sheet

## How to:

### init lua:

```c
lua_State *lua_state = luaL_newstate();
luaL_openlibs(lua_state);
luaL_dofile(lua_state, "breakout.lua"); // load a lua module
```

### register C functions

```c
luaL_dostring(lua_state, "love = {}");
luaL_dostring(lua_state, "love.graphics = {}");

lua_getfield(lua_state, -1, "graphics");
int graphics = lua_gettop(lua_state);

lua_pushcfunction(lua_state, graphics_printf);
lua_setfield(lua_state, graphics, "printf");
```

### C functions

#### Reading values from lua calls
```c
int graphics_rectangle(lua_State *l) {
  const char *mode = lua_tostring(l, -5); // strings
  lua_Number x = lua_tonumber(l, -4);  // numbers
  lua_Number y = lua_tonumber(l, -3);
  lua_Number rx = lua_tonumber(l, -2);
  lua_Number ry = lua_tonumber(l, -1);

  DrawRectangle(x, y, rx, ry, currentColor);

  return 0;
}
```

#### Returning values to lua calls

C functions always returns the number of items added to the stack 

```c
int graphics_get_height(lua_State *l) {
  lua_pushnumber(l, screenHeight);
  return 1;
}
```

### Calling lua functions

```c
lua_getglobal(lua_state, "love");
lua_getfield(lua_state, love_index, "keyreleased");
lua_pushstring(lua_state, "return");
lua_call(lua_state, 1, 0); // number of params added to the stack (1)
```
