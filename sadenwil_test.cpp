/* sadenwil - Simple Adventure Engine With Lua
 * v 0.1
 *
 * by Simon Kenny
 *
 * compile with: g++ sadenwil_test.cpp -o sadenwil_test -I/usr/include/lua5.2 -llua
 */

#include <stdio.h>
#include <stdlib.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


int main( int argc, char *argv[] ) {
	for( int i = 1 ; i < argc ; ++i ) {
		lua_State *lua;

		lua = luaL_newstate();
		luaL_openlibs(lua);

		printf( "-- Loading file: %s\n", argv[i] );

		int status = luaL_loadfile(lua, argv[i]);
		if( status == 0 )
			status = lua_pcall(lua, 0, 0, 0);
		// report errors
		if( status != 0 ) {
			printf( "-- %s\n", lua_tostring(lua, -1) );
			lua_pop(lua, 1); // remove error message
		} else
			printf( "Compiled and run without errors.\n" );
	
		lua_close(lua);
	}
	exit(0);
}
