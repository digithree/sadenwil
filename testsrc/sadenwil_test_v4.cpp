/* sadenwil - Simple Adventure Engine With Lua
 * v 0.4
 *
 * by Simon Kenny
 *
 * compile with: g++ sadenwil_test_v4.cpp -o sadenwil_test_v4 -I/usr/include/lua5.1 -llua5.1 -Wall
 *
 * ./sadenwil_test_v4 /home/digithree/Documents/sadenwil/testgame/
 *
 *
 * Changelog:
 * v0.4
 * v0.3
 *		- adding room navigation
 *		- developed sqawning of items in scripts
 * v0.2
 *		- added game config file
 *		- added command parsing and invocation
 * v0.1
 *		- basic room recognition
 *
 * \033[1;31m   text   \033[0m
 *      ^ is bold, for normal use 2
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <time.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#define FUNC_MAIN	1
#define FUNC_DIALOG	2

//#define DELAY_AFTER_MESSAGE 	// comment out to remove delay (for faster testing)
#define DELAY_PER_LINE			6.f    //in half seconds

static std::vector<std::string> commandList;

void sleepms(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void delayWithAnimation( int delay_t ) {
	int delay = delay_t;
	while( delay > 0 ) {
		for( int i = 0 ; i < delay ; i++ )
			std::cout << ".";
		flush(std::cout);
		sleepms(500000);
		delay--;
		std::cout << "\x1B[2K\x1B[0E";
	}
}

static int screenMessage(lua_State *L) {
	int col = lua_tointeger(L,1);
	std::string strFrom = std::string((char*)lua_tostring(L, 2));
	std::string strMsg = std::string((char*)lua_tostring(L, 3));
	std::cout << "\033[1;35m" << strFrom << ": ";
	if( col == 0 )
		std::cout << "\033[1;30m";
	else if( col == 1 )
		std::cout << "\033[1;31m";
	else if( col == 2 )
		std::cout << "\033[1;32m";
	else if( col == 3 )
		std::cout << "\033[1;33m";
	else if( col == 4 )
		std::cout << "\033[1;34m";
	else if( col == 5 )
		std::cout << "\033[1;35m";
	else if( col == 6 )
		std::cout << "\033[1;36m";
	else if( col == 7 )
		std::cout << "\033[1;37m";
	std::cout << strMsg << "\033[0m" << std::endl;
	// delay each message
	#ifdef DELAY_AFTER_MESSAGE
		delayWithAnimation( 2 + (int)((((float)strMsg.length())/80.f) * DELAY_PER_LINE) );
	#endif
	return 0;  // number of results
}

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
	
	void registerFunctions() {
		lua_pushcfunction(luaPtr, screenMessage);
	    lua_setglobal(luaPtr, "screenMessage");
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
			std::cerr << "-- couldn't get string field from global" << std::endl;
			lua_pop(luaPtr, 1);
			return std::string();
		}
		std::string result = std::string((char*)lua_tostring(luaPtr, -1));
		lua_pop(luaPtr, 1);
		return result;
	}
	
	float getGlobal_number( std::string name ) {
		return getGlobal_number( name.c_str() );
	}
	
	float getGlobal_number( const char *name ) {
		lua_getglobal(luaPtr,name);
		if( !lua_isnumber(luaPtr, -1) ) {
			std::cerr << "-- couldn't get number field from global" << std::endl;
			lua_pop(luaPtr, 1);
			return 0;
		}
		float result = (float)lua_tonumber(luaPtr, -1);
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
			lua_getglobal(luaPtr, "processFunction");
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
	
	bool runDoubleCommand( std::string command1, std::string item, std::string command2 ) {
		if( !fatalError ) {
			lua_getglobal(luaPtr, "processFunctionDouble");
			if( !lua_isfunction(luaPtr, -1) ) {
				std::cerr << "-- couldn't get function from global!" << std::endl;
				return true;
			}
			lua_pushstring(luaPtr, command1.c_str());
			lua_pushstring(luaPtr, item.c_str());
			lua_pushstring(luaPtr, command2.c_str());
			if( lua_pcall(luaPtr,3,0,0) ) {
				std::cerr << "-- couldn't run function" << std::endl;
				return true;
			}
			//lua_pop(luaPtr,1);
			return false;
		}
		return true;
	}
	
	int cutScene() {
		if( !fatalError ) {
			lua_getglobal(luaPtr, "processFunction");
			if( !lua_isfunction(luaPtr, -1) ) {
				std::cerr << "-- couldn't get function from global!" << std::endl;
				return -1;
			}
			if( lua_pcall(luaPtr,0,1,0) ) {
				std::cerr << "-- couldn't run function" << std::endl;
				return -1;
			}
			if( !lua_isnumber(luaPtr, -1) ) {
				std::cerr << "-- return from cut scene process not number!" << std::endl;
				return -1;
			}
			int num = (int)lua_tonumber(luaPtr,-1);
			return num;
		}
		return -1;
	}
	
	// game specific calls
	
	void startGame() {
		if( !fatalError ) {
			lua_getglobal(luaPtr, "startGame");
			if( !lua_isfunction(luaPtr, -1) ) {
				std::cerr << "-- couldn't get startGame function from global!" << std::endl;
				return;
			}
			if( lua_pcall(luaPtr,0,0,0) ) {
				std::cerr << "-- couldn't run startGame function" << std::endl;
				return;
			}
		}
	}
	
	void doTurn() {
		if( !fatalError ) {
			lua_getglobal(luaPtr, "doTurn");
			if( !lua_isfunction(luaPtr, -1) ) {
				std::cerr << "-- couldn't get doTurn function from global!" << std::endl;
				return;
			}
			if( lua_pcall(luaPtr,0,0,0) ) {
				std::cerr << "-- couldn't run doTurn function" << std::endl;
				return;
			}
		}
	}
};


std::string removeWhitespace( std::string str ) {
	std::string retStr = std::string(str);
	//from start
	int pos = 0;
	for( int i = 0 ; i < (int)retStr.length() ; i++ ) {
		if( (int)retStr[i] == ' ' )
			pos++;
		else
			break;
	}
	retStr = retStr.substr(pos,retStr.length() - pos);
	//from end
	pos = retStr.length();
	for( int i = (retStr.length()-1) ; i >= 0 ; i-- ) {
		if( (int)retStr[i] == ' ' )
			pos--;
		else
			break;
	}
	retStr = retStr.substr(0,pos);
	return retStr;
}

bool processTurn( Lua *luaPtr ) {
	int mode = (int)(*luaPtr).getGlobal_number( "activeFunction" );
	
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
	for( int i = 0 ; i < 4 ; i++ )
		parsedInput[i] = removeWhitespace(parsedInput[i]);
	
	// TODO get rid of leading and trailing white space
	// TODO to lowercase
	
	if( numInputs >= 1 ) {
		if( parsedInput[0].compare("quit") == 0 ) {
			std::cout << "\033[1;35mnarrator: \033[1;36mGoodbye\033[0m" << std::endl;
			return true;
		} else if( parsedInput[0].compare("help") == 0 ) {
			if( mode == FUNC_MAIN ) {
				std::cout << "\033[1;35mCommand structure: command-noun[-with-othernoun]" << std::endl;
				std::cout << "Commands: ";
				for( std::vector<std::string>::const_iterator it = commandList.begin() ; it != commandList.end() ; ++it ) {
					if( it != commandList.begin() )
						std::cout << ", ";
					std::cout << (*it);
				}
				std::cout << ", help, quit\033[0m" << std::endl;
			} else {
				std::cout << "\033[1;35mType in the number of the line you wish to say.\033[0m" << std::endl;
			}
			return false;
		}
	}
	
	if( mode == FUNC_DIALOG ) {
		(*luaPtr).runCommand( parsedInput[0], std::string() );
		return false;
	}
	
	// otherwise mode must be main mode, so process commands
	
	// check minimum number of commands
	if( numInputs < 2 ) {
		std::cout << "Not enough commands, type \"help\" for help." << std::endl;
		return false;
	}
	
	// check command
	bool gotCommand = false;
	for( std::vector<std::string>::const_iterator it = commandList.begin() ; it != commandList.end() ; ++it ) {
		//std::cout << parsedInput[1] << " == " << (*it) << " ???" << std::endl;
		if( parsedInput[0].find(*it) != std::string::npos ) {
			if( numInputs == 2 )
				(*luaPtr).runCommand( (*it), parsedInput[1] );
			else if( numInputs == 4 )
				(*luaPtr).runDoubleCommand( (*it), parsedInput[1], parsedInput[3] );;
			gotCommand = true;
		}
	}
	if( !gotCommand )
		std::cout << "Didn't recognise command, type \"help\" for help." << std::endl;
	
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
	if( lua.loadFile(gamePath + "items.lua") )
		exit(-1);
	if( lua.loadFile(gamePath + "dialog.lua") )
		exit(-1);
	if( lua.loadFile(gamePath + "room1.lua") )
		exit(-1);
	if( lua.loadFile(gamePath + "room2.lua") )
		exit(-1);
	if( lua.loadFile(gamePath + "room3.lua") )
		exit(-1);
	if( lua.loadFile(gamePath + "postinit.lua") )
		exit(-1);
	
	// register lua -> C functions
	lua.registerFunctions();
		
	lua.startGame();

	bool doQuit = false;
	while( !doQuit ) {
		// get commands from user
		doQuit = processTurn(&lua);
		// process in room turn events
		lua.doTurn();
	}
	
	exit(0);
}
