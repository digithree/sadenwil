-----
-- Interface - to outside stuff, like "graphics", etc.
-----

function message( from, msg )
	if from == nil then
		screenMessage( 4, "", msg );
	else
		if from == "ego" or from == "you" then
			screenMessage( 2, from, msg );
		elseif from == "narrator" then
			screenMessage( 6, from, msg );
		else
			screenMessage( 3, from, msg );
		end
	end
end


function executeCommand( com, itemName )
	if itemName ~= nil and com ~= nil then
		if itemName == "room" or itemName == "surroundings" then
			local room = rooms[curRoomIdx];
			message( "narrator", room.description );
			showItems(room.items);
			return;
		end
		local itemLists = { inventory, globalItems, rooms[curRoomIdx].items, rooms[curRoomIdx].hiddenItems };
		for j, itemList in ipairs(itemLists) do
			for i, item in ipairs(itemList) do
				if itemName == item.name and item.name ~= "invalid" then
					item:command( com );

					return;
				end
			end
		end
		-- if program gets to here then no item was found
		message( "ego", "I can't see that." );
	end
end

function executeDoubleCommand( com, itemName, otherItemName )
	if itemName ~= nil and com ~= nil and otherItemName ~= nil then
		-- see if com1 is in commandsDouble list
		local match = 0;
		for i, command in ipairs(doubleCommands) do
			if command == com then
				match = -1; --true
				break;
			end
		end
		if match == 0 then --no match
			message( "ego", "That doesn't work." );
			return;
		end
		-- otherwise, continue
		local itemLists = { inventory, globalItems, rooms[curRoomIdx].items, rooms[curRoomIdx].hiddenItems };
		-- find first item by name
		local item1 = nil;
		for j, itemList in ipairs(itemLists) do
			for i, item in ipairs(itemList) do
				if itemName == item.name and item.name ~= "invalid" then
					item1 = item;
					break;
				end
			end
		end
		-- if not found, exit
		if item1 == nil then
			message( "ego", "I can't see " .. itemName );
			return;
		end
		-- find second item by name
		local item2 = nil;
		for j, itemList in ipairs(itemLists) do
			for i, item in ipairs(itemList) do
				if otherItemName == item.name and item.name ~= "invalid" then
					item2 = item;
					break;
				end
			end
		end
		-- if not found, exit
		if item2 == nil then
			message( "ego", "I can't see " .. otherItemName );
			return;
		end
		-- found
		item1:commandDouble( com, item2 );
		return;
	end
end

function doTurn()
	rooms[curRoomIdx].onTurn();
end
