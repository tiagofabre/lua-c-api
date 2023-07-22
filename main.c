#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int main(int argc, char **argv){
    lua_State *lua = luaL_newstate();
    luaL_openlibs(lua);

    luaL_dostring(lua, "print 'Hello, World!'");

    for(int i=0; i < 3; i++) {
        luaL_dofile(lua, "hello.lua");

        lua_getglobal(lua, "hello_world");
        lua_pushstring(lua, "Tiago");
        lua_call(lua, 1, 1);
    }

    return 0;
}
