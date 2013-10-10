#ifndef LUA_SCRIPTING_HPP
#define LUA_SCRIPTING_HPP

#include <lua.hpp>
#include <type_traits>
#include <cassert>
#include <SFML/System.hpp>

namespace sfLua{
    // Push a Lua table with the sf::Vector2<T> x and y fields inside
    // Only works with arithmetic types
    template <typename T>
    void lua_pushVector2(lua_State *L, sf::Vector2<T> vector);

    // Converts the Lua table at the given index in a sf::Vector2f
    // Table must have x and y fields inside
    sf::Vector2f lua_toVector2f(lua_State *L, int index);

    // Converts the Lua table at the given index in a sf::Vector2i
    // Table must have x and y fields inside
    sf::Vector2i lua_toVector2i(lua_State *L, int index);

    // Converts the Lua table at the given index in a sf::Vector2u
    // Table must have x and y fields inside
    sf::Vector2u lua_toVector2u(lua_State *L, int index);
}

#endif // LUA_SCRIPTING_HPP
