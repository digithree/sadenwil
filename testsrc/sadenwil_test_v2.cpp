/* sadenwil - Simple Adventure Engine With Lua
 * v 0.2
 *
 * by Simon Kenny
 *
 * compile with: gcc sadenwil_test_tableget.c -o sadenwil_test_tableget -I/usr/include/lua5.1 -llua5.1 -Wall
 *
 * /home/digithree/Documents/sadenwil/testgame/
 *
 *
 * Changelog:
 * v0.2
 *		- adding game config file
 * v0.1
 *		- basic room recognition
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static std::vector<std::string> commandList;

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
	
	// load file	
	bool loadFile( char *file ) {
		if( !fatalError ) {
			int status = luaL_loadfile(luaPtr, file);
			if( status == 0 )
				status = lua_pcall(luaPtr, 0, 0, 0);
			return hasError( status );
		}
		return false;
	}
	
	bool loadFile( std::string file ) {
		return loadFile( file.c_str() );
	}
	
	bool loadFile( const char *file ) {
		char str[128];
		strcpy( str, file );
		return loadFile( str );
	}
	
	// global var access
	std::string getGlobal_string( std::string name ) {
		return getGlobal_string( name.c_str() );
	}
	
	std::string getGlobal_string( const char *name ) {
		lua_getglobal(luaPtr,name);
		if( !lua_isstring(luaPtr, -1) ) {
			std::cerr << "-- couldn't get field from table" << std::endl;
			lua_pop(luaPtr, 1);
			return std::string();
		}
		std::string result = std::string((char*)lua_tostring(luaPtr, -1));
		lua_pop(luaPtr, 1);
		return result;
	}
	
	bool runFuncFromGlobal( const char *func ) {
		if( !fatalError ) {
			lua_getglobal(luaPtr, func);
			if( !lua_isfunction(luaPtr, -1) ) {
				std::cerr << "-- couldn't get function from global!" << std::endl;
				//lua_pop(luaPtr,1);
				return true;
			}
			if( lua_pcall(luaPtr,0,0,0) ) {
				std::cerr << "-- couldn't run function" << std::endl;
				return true;
			}
			//lua_pop(luaPtr,1);
			return false;
		}
		return true;
	}
	
	// table access
	bool loadTable( std::string table ) {
		return loadTable( table.c_str() );
	}
	
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
			if( !lua_istable(luaPtr, -1) ) {
				std::cerr << "-- trying to pop table but no table on top of stack" << std::endl;
				return true;
			}
			lua_pop(luaPtr, 1);
			tableIsLoaded = false;
			return false;
		}
		return true;
	}
	
	int getTableSize() {
		if( !fatalError && tableIsLoaded ) {
			if( !lua_istable(luaPtr, -1) ) {
				std::cerr << "-- couldn't get size of table, no table on top of stack!" << std::endl;
				return 0;
			}
			//luaL_getn(luaPtr, -1);
			lua_getglobal(luaPtr, "getNumCommands");
			if( lua_pcall(luaPtr,0,1,0) ) {
				std::cerr << "-- couldn't get size of table, count function failed" << std::endl;
				return 0;
			}
			if( !lua_isnumber(luaPtr, -1) ) {
				std::cerr << "-- couldn't get size of table" << std::endl;
				return 0;
			}
			int len = lua_tointeger(luaPtr,-1);
			lua_pop(luaPtr, 1);
			return len;
		}
		return 0;
	}
	
	std::string getFieldFromTable_string( std::string key ) {
		return getFieldFromTable_string( key.c_str() );
	}
	
	std::string getFieldFromTable_string( const char *key ) {
		if( !fatalError && tableIsLoaded ) {
			lua_pushstring(luaPtr, key);
			lua_gettable(luaPtr, -2);  /* get background[key] */
			if (!lua_isstring(luaPtr, -1)) {
				std::cerr << "-- couldn't get field from table (via key)" << std::endl;
				lua_pop(luaPtr, 1);
				return std::string();
			}
			std::string result = std::string((char*)lua_tostring(luaPtr, -1));
			lua_pop(luaPtr, 1);
			return result;
		}
		return std::string();
	}
	
	std::string getFieldFromTable_string( int idx ) {
		if( !fatalError && tableIsLoaded ) {
			lua_rawgeti(luaPtr, -1, idx);
			if (!lua_isstring(luaPtr, -1)) {
				std::cerr << "-- couldn't get field from table (via idx)" << std::endl;
				lua_pop(luaPtr, 1);
				return std::string();
			}
			std::string result = std::string((char*)lua_tostring(luaPtr, -1));
			lua_pop(luaPtr, 1);
			return result;
		}
		return std::string();
	}

	bool runFuncFromTable( std::string func ) {
		return runFuncFromTable( func.c_str() );
	}
	
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
			//lua_pop(luaPtr,1);
			return false;
		}
		return true;
	}
	
	// run command
	bool runCommand( std::string command, std::string item ) {
		if( !fatalError ) {
			lua_getglobal(luaPtr, "executeCommand");
			if( !lua_isfunction(luaPtr, -1) ) {
				std::cerr << "-- couldn't get function from global!" << std::endl;
				return true;
			}
			lua_pushstring(luaPtr, command.c_str());
			lua_pushstring(luaPtr, item.c_str());
			if( lua_pcall(luaPtr,2,0,0) ) {
				std::cerr << "-- couldn't run function" << std::endl;
				return true;
			}
			//lua_pop(luaPtr,1);
			return false;
		}
		return true;
	}
};




bool processInput( Lua *luaPtr ) {
	// get input
	std::cout << "> ";
	std::string line;
	std::getline(std::cin, line);
	
	std::string parsedInput[4];
	int numInputs = 0;
	// parse using dashes
	for( int i = 0 ; i < 4 ; i++ ) {
		size_t result = line.find_first_of('-');
		if( result != std::string::npos ) {
			parsedInput[i] = line.substr(0, (int)result);
			line = line.substr((int)result + 1, line.length() - (int)result -1);
		} else {
			parsedInput[i] = line;
			numInputs = i+1;
			break;
		}
	}
	//for( int i = 0 ; i < numInputs ; i++ )
	//	std::cout << i << " :" << parsedInput[i] << std::endl;
	
	// check minimum number of commands
	if( numInputs < 2 ) {
		std::cout << "Not enough commands, type \"help\" for help." << std::endl;
		return false;
	}
	
	// check command
	for( std::vector<std::string>::const_iterator it = commandList.begin() ; it != commandList.end() ; ++it ) {
		std::cout << parsedInput[1] << " == " << (*it) << " ???" << std::endl;
		if( parsedInput[0].find(*it) != std::string::npos ) {
			(*luaPtr).runCommand( (*it), parsedInput[1] );
		}
	}
	return false;
}



int main( int argc, char *argv[] ) {
	Lua lua;
	std::string gamePath;
	
	if( argc < 2 ) {
		std::cerr << "-- no game directory specified!" << std::endl;
		exit(-1);
	}
	gamePath = std::string(argv[1]);
	if( argv[1][strlen(argv[1])-2] != '/' )
		gamePath += '/';
	
	// load files for game
	// load commands
	if( lua.loadFile(gamePath + "commands.lua") )
		exit(-1);
	if( lua.loadTable("commands") )
		exit(-1);
	else {
		int len = lua.getTableSize();
		for( int i = 0 ; i < len ; i++ ) { //lua uses subscripts starting at 1
			std::string str = lua.getFieldFromTable_string(i+1);
			commandList.push_back( str );
		}
		if( lua.unloadTable() )
			std::cout << "Couldn't unload commands table" << std::endl;
	}
	// start initial stuff
	if( lua.loadFile(gamePath + "initial.lua") )
		exit(-1);
	if( lua.loadFile(gamePath + "interface.lua") )
		exit(-1);
	if( lua.loadFile(gamePath + "room1.lua") )
		exit(-1);
		
	bool doQuit = false;
	while( !doQuit ) {
		doQuit = processInput(&lua);
	}
	
	//std::string firstRoom = lua.getGlobal_string("firstroom");
	//std::cout << "first room is: " << firstRoom << std::endl;
		
	/*	
	// access room table
	if( lua.loadTable("room") )
		std::cerr << "-- couldn't get room table from lua engine" << std::endl;
	else {
		std::cout << "Room name: " << lua.getFieldFromTable_string("name") << std::endl;
		std::cout << "Description: " << lua.getFieldFromTable_string("description") << std::endl;
		lua.runFuncFromTable("onEnter");
		lua.unloadTable();
	}
	*/
	
	exit(0);
}
