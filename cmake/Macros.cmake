# macro that compile all the lua scripts
macro(compile_lua_scripts)
    file(MAKE_DIRECTORY data)
    foreach(filepath ${ARGN})
        get_filename_component(filename ${filepath} NAME_WE)
        # execute the luac compiler
        execute_process(COMMAND ${LUAC_EXECUTABLE} -o "${LUA_OBJ_DIR}/${filename}.luac" ${filepath})
    endforeach()
endmacro()
