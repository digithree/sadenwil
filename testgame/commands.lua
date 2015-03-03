-----
-- Game config file
-----

-- Commands
commands = { "look at", "talk to", "open", "close", "walk to", "pick up", "drop", "give", "use", "push", "pull" };
doubleCommands = { "give", "use" };

function getNumCommands()
	local c = 0;
	for i, com in ipairs(commands) do
		c = c + 1;
	end
	return c;
end

-- Processing
function getCommand()
	io.write("> ");
	local line = io.read();
	line = string.lower(line);
	if line == nil then return end;
	-- see if commands are in line
	local commandFound = -1;
	for i, com in ipairs(commands) do
		local result = string.find(line, com);
		if result ~= nil then
			io.write("Got command match at ",i,": ",result,"\n");
			if result == 1 then 
				commandFound = i;
				io.write("Command is at start of string\n");
				break;
			end
		end
	end
	-- check if we found a command
	if commandFound >= 0 then
		io.write("Found command \"", commands[commandFound],"\"\n");
	else
		io.write("No command found\n");
	end
end;
