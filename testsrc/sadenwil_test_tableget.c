/* sadenwil - Simple Adventure Engine With Lua
 * v 0.1
 *
 * by Simon Kenny
 *
 * compile with: gcc sadenwil_test_tableget.c -o sadenwil_test_tableget -I/usr/include/lua5.1 -llua5.1 -Wall
 */

#include <stdio.h>
#include <stdlib.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/* assume that table is on the stack top */
char* getfield_string( lua_State *L, const char *key ) {
	char *result;
	lua_pushstring(L, key);
	lua_gettable(L, -2);  /* get background[key] */
	if (!lua_isstring(L, -1)) {
		printf("[ERROR] couldn't get field!\n");
		return "\0";
	}
	result = (char*)lua_tostring(L, -1);
	lua_pop(L, 1);  /* remove from top */
	return result;
}

/* assume that table is on the stack top */
void runfunc_fromtable( lua_State *L, const char *func ) {
	lua_pushstring(L, func);
	lua_gettable(L, -2);
	if( !lua_isfunction(L, -1) ) {
		printf("[ERROR] couldn't get function from table!\n");
		return;
	}
	if( lua_pcall(L,0,0,0) )
		printf("[ERROR] couldn't run function\n");
	lua_pop(L,1);
}

int main( int argc, char *argv[] ) {
	int i;
	for( i = 1 ; i < argc ; ++i ) {
		lua_State *lua;

		lua = luaL_newstate();
		luaL_openlibs(lua);

		printf( "-- Loading file: %s\n", argv[i] );
		
		/* PRIMING RUN. FORGET THIS AND YOU'RE TOAST */
		/*
		if (lua_pcall(lua, 0, 0, 0)) {
			printf("lua_pcall() primer failed\n");
			exit(-1);
		}
		*/

		int status = luaL_loadfile(lua, argv[i]);
		if( status == 0 )
			status = lua_pcall(lua, 0, 0, 0);
		// report errors
		if( status != 0 ) {
			printf( "-- %s\n", lua_tostring(lua, -1) );
			lua_pop(lua, 1); // remove error message
		} else
			printf( "Compiled and run without errors.\n" );

		// get table
		lua_getglobal(lua,"room");
		if( !lua_istable(lua,-1) )
			printf("couldn't get room table from lua engine\n");
		else {
			printf( "Room name: %s\n", getfield_string(lua, "name") );
			printf( "Description: %s\n", getfield_string(lua, "description") );
		}
		
		// run onEnter function
		runfunc_fromtable(lua, "onEnter");
	
		lua_close(lua);
	}
	exit(0);
}
