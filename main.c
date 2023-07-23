#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <raylib.h>

void event_loop(lua_State *lua_state) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        float dt = GetFrameTime();

        // Draw
        BeginDrawing();

            ClearBackground(BLACK);

            lua_getglobal(lua_state, "love");
            lua_getfield(lua_state, -1, "update");
            lua_pushnumber(lua_state, dt);
            lua_call(lua_state, 1, 0);

            /* luaL_dostring(lua_state, "love.draw()"); */

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
}

int get_width(lua_State *l) {
    lua_pushnumber(l, 800);
    return 1;
}
int get_height(lua_State *l) {
    lua_pushnumber(l, 450);
    return 1;
}

int main(int argc, char **argv){
    lua_State *lua_state = luaL_newstate();
    luaL_openlibs(lua_state);

    luaopen_table(lua_state);

    luaL_dostring(lua_state, "love = {}");
    luaL_dostring(lua_state, "love.graphics = {}");

    lua_getglobal(lua_state, "love");
    lua_getfield(lua_state, -1, "graphics");
    lua_setfield(lua_state, -1, "getWidth");
    lua_pushcfunction(lua_state, get_width);
    /* TODO Check how to set a function to a table. it'll be used to override love behaviors */

    /* lua_getglobal(lua_state, "love"); */
    /* lua_getfield(lua_state, -1, "graphics"); */
    lua_setglobal(lua_state, "getHeight");
    lua_pushcfunction(lua_state, get_height);

    /* lua_register(lua_state, "love.graphics.getWidth", get_width); */
    /* lua_register(lua_state, "love.graphics.getHeight", get_height); */

    int game_status = luaL_dofile(lua_state, "breakout.lua");
    if(0 != game_status) {
        printf("dofile error %i\n", game_status);
        return 1;
    }

    event_loop(lua_state);

    lua_close(lua_state);
    return 0;
}
