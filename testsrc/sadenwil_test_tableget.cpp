/* sadenwil - Simple Adventure Engine With Lua
 * v 0.1
 *
 * by Simon Kenny
 *
 * compile with: gcc sadenwil_test_tableget.c -o sadenwil_test_tableget -I/usr/include/lua5.1 -llua5.1 -Wall
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


class Lua {
private:
	lua_State *luaPtr;
	bool tableIsLoaded;
	bool fatalError;
	
	bool hasError( int status ) {
		if( status != 0 ) {
			std::cerr <<  "-- " << lua_tostring(luaPtr, -1) << std::endl;
			lua_pop(luaPtr, 1); // remove error message
			fatalError = true;
			return true;
		}
		return false;
	}
	
public:
	Lua() {
		luaPtr = luaL_newstate();
		luaL_openlibs(luaPtr);
		tableIsLoaded = false;
		fatalError = false;
	}
	
	~Lua() {
		lua_close(luaPtr);
	}
	
	bool loadFile( char *file ) {
		if( !fatalError ) {
			int status = luaL_loadfile(luaPtr, file);
			if( status == 0 )
				status = lua_pcall(luaPtr, 0, 0, 0);
			return hasError( status );
		}
		return false;
	}
	
	bool loadFile( const char *file ) {
		char str[128];
		strcpy( str, file );
		return loadFile( str );
	}
	
	// table access
	bool loadTable( const char *table ) {
		if( !fatalError && !tableIsLoaded ) {
			lua_getglobal(luaPtr,table);
			if( !lua_istable(luaPtr, -1) ) {
				std::cerr << "-- couldn't get table" << std::endl;
				lua_pop(luaPtr, 1);
				return true;
			}
			tableIsLoaded = true;
			return false;
		}
		return true;
		
	}
	
	bool unloadTable() {
		if( !fatalError && tableIsLoaded ) {
			lua_pop(luaPtr, 1);
			tableIsLoaded = false;
			return false;
		}
		return true;
	}
	
	/* assume that table is on the stack top */
	char* getFieldFromTable_string( const char *key ) {
		if( !fatalError && tableIsLoaded ) {
			char *result;
			lua_pushstring(luaPtr, key);
			lua_gettable(luaPtr, -2);  /* get background[key] */
			if (!lua_isstring(luaPtr, -1)) {
				std::cerr << "-- couldn't get field from table" << std::endl;
				return "\0";
			}
			result = (char*)lua_tostring(luaPtr, -1);
			lua_pop(luaPtr, 1);
			return result;
		}
		return "\0";
	}

	/* assume that table is on the stack top */
	bool runFuncFromTable( const char *func ) {
		if( !fatalError && tableIsLoaded ) {
			lua_pushstring(luaPtr, func);
			lua_gettable(luaPtr, -2);
			if( !lua_isfunction(luaPtr, -1) ) {
				std::cerr << "-- couldn't get function from table!" << std::endl;
				return true;
			}
			if( lua_pcall(luaPtr,0,0,0) ) {
				std::cerr << "-- couldn't run function" << std::endl;
				return true;
			}
			lua_pop(luaPtr,1);
			return false;
		}
		return true;
	}
};



int main( int argc, char *argv[] ) {
	int i;
	for( i = 1 ; i < argc ; ++i ) {
		Lua lua;

		if( lua.loadFile(argv[i]) )
			exit(-1);
			
		// access room table
		if( lua.loadTable("room") )
			std::cerr << "couldn't get room table from lua engine" << std::endl;
		else {
			std::cout << "Room name: " << lua.getFieldFromTable_string("name") << std::endl;
			std::cout << "Description: " << lua.getFieldFromTable_string("description") << std::endl;
			lua.runFuncFromTable("onEnter");
			lua.unloadTable();
		}
	}
	exit(0);
}
