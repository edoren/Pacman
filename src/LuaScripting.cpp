#include "LuaScripting.hpp"
#include <iostream>

namespace sfLua {
    template <typename T>
    void lua_pushVector2(lua_State *L, sf::Vector2<T> vector) {
        lua_createtable(L, 0, 2);
        lua_pushnumber(L, vector.x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, vector.y);
        lua_setfield(L, -2, "y");
        lua_getglobal(L, "VectorMT"); // Gets Vector Metatable
        lua_setmetatable(L, -2);
    }

    template void lua_pushVector2<float>(lua_State*, sf::Vector2<float>);
    template void lua_pushVector2<unsigned int>(lua_State*, sf::Vector2<unsigned int>);
    template void lua_pushVector2<int>(lua_State*, sf::Vector2<int>);

    sf::Vector2f lua_toVector2f(lua_State *L, int index) {
        sf::Vector2f vector;
        assert(lua_istable(L, index));
        lua_getfield(L, index, "x");
        assert(!lua_isnil(L, -1));
        vector.x = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, index, "y");
        assert(!lua_isnil(L, -1));
        vector.y = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return vector;
    }

    sf::Vector2i lua_toVector2i(lua_State *L, int index) {
        sf::Vector2i vector;
        assert(lua_istable(L, index));
        lua_getfield(L, index, "x");
        assert(!lua_isnil(L, -1));
        vector.x = lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, index, "y");
        assert(!lua_isnil(L, -1));
        vector.y = lua_tointeger(L, -1);
        lua_pop(L, 1);
        return vector;
    }

    sf::Vector2u lua_toVector2u(lua_State *L, int index) {
        sf::Vector2u vector;
        assert(lua_istable(L, index));
        lua_getfield(L, index, "x");
        assert(!lua_isnil(L, -1));
        vector.x = lua_tounsigned(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, index, "y");
        assert(!lua_isnil(L, -1));
        vector.y = lua_tounsigned(L, -1);
        lua_pop(L, 1);
        return vector;
    }
}
