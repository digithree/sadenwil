-----
-- Test Room
-----

item_ph_key = { name = "key",
				description = "A simple iron key. It is heavy",
				pickUpable = "yes",
				found = "no",
				parent = nil
				};
function item_ph_key:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "pick up" then
		removeItemViaInstance( self );
		addItem( self, inventory );
		message( "ego", "You picked up the key." );
		message( "ego", "Key is now in inventory."); -- generalise to item.lua funciton
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_key:commandDouble( com, item )
	if com == "use" and item.name == "door" then
		if self.parent == inventory then
			if item.locked == "yes" then
				message( "narrator", "You unlock the door using the key." );
				item.locked = "no";
			else
				message( "narrator", "You lock the door using the key." );
				item.locked = "yes";
			end
		else
			message( "ego", "I'd have to pick up the key to use it!" );
		end
	else
		message( "ego", "That doesn't work." );
	end
end


item_ph_jumper = {	name = "jumper",
					description = "A white woolen knitted patterned jumper. Known as an Aran jumper.",
					pickUpable = "yes",
					parent = nil
				};
function item_ph_jumper:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "pick up" then
		removeItemViaInstance( self );
		addItem( self, inventory );
		message( "ego", "You picked up the jumper." );
		message( "ego", "Jumper is now in inventory.");
	elseif com == "use" then
		message( "ego", "You put on the jumper." );
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_jumper:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end


item_ph_closet = {	name = "closet",
					description = "A tall wooden closet. It's design is simple and it is certainly homemade.",
					pickUpable = "no",
					closed = "yes",
					jumperSpawned = "no",
					parent = nil
				};
function item_ph_closet:command( com )
	if com == "look at" then
		message( "ego", self.description );
		if self.closed == "yes" then
			message( "ego", "It is closed." );
		else
			message( "ego", "It is open." );
		end
	elseif com == "open" then
		if self.closed == "yes" then
			closed = "no"
			message( "ego", "You have opened the closet.");
			if self.jumperSpawned == "no" then
				self.jumperSpawned = "yes";
				addItem( item_ph_jumper, rooms[curRoomIdx].items );
				message( "ego", "There's a jumper in the closet." );
			end
		else
			message( "ego", "The closet is already open." );
		end
	elseif com == "close" then
		if self.closed == "no" then
			message( "ego", "You have closed the closet." );
		else
			message( "ego", "The closet is already closed." );
		end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_closet:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

item_ph_bed = {		name = "bed",
					description = "A simple single bed. The bed clothes are well make and it looks comfortable.",
					pickUpable = "no",
					parent = nil
				};
function item_ph_bed:command( com )
	if com == "look at" then
		message( "ego", self.description );
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_bed:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end


item_ph_mirror = {	name = "mirror",
					description = "A small rectangular mirror. It is clean and you can see yourself clearly in it.",
					pickUpable = "no",
					parent = nil
				};
function item_ph_mirror:command( com )
	if com == "look at" then
		message( "ego", self.description );
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_mirror:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

-- hidden items

item_ph_window = {	name = "window",
					description = "Looking out the window you can see the pier and Kilronan harbour.",
					pickUpable = "no",
					parent = nil
				};
function item_ph_window:command( com )
	if com == "look at" then
		message( "ego", self.description );
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_window:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

item_ph_door = {	name = "door",
					description = "It's a normal enough door.",
					pickUpable = "no",
					open = "no",
					locked = "yes",
					parent = nil
				};
function item_ph_door:command( com )
	if com == "look at" then
		message( "ego", self.description );
		if self.locked == "yes" then
			message( "ego", "It is locked." );
		else
			message( "ego", "It is unlocked." );
			if self.open == "yes" then
				message( "ego", "It is open." );
			else
				message( "ego", "It is closed." );
			end
		end
	elseif com == "open" then
		if self.locked == "no" then
			if self.open == "no" then
				self.open = "yes";
				message( "ego", "You opened the door." );
			else
				message( "ego", "The door is already open." );
			end
		else
			message( "ego", "The door is locked so I can't open it. Where's the key?" );
		end
	elseif com == "close" then
		if self.open == "yes" then
			self.open = "no";
			message( "ego", "You closed the door." );
		else
			message( "ego", "The door is already closed." );
		end
	elseif com == "walk to" then
		if self.open == "yes" then
			if curRoomIdx == 1 then
				-- leave room 1
				rooms[curRoomIdx]:onExit();
				-- go to room 2
				curRoomIdx = 2;
				rooms[curRoomIdx]:onEnter();
			elseif curRoomIdx == 2 then
				-- leave room 2
				rooms[curRoomIdx]:onExit();
				-- go to room 1
				curRoomIdx = 1;
				rooms[curRoomIdx]:onEnter();
			else
				print( "ERROR, can't change room!" );
			end
		else
			message( "ego", "You can't walk through a closed door." );
		end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_door:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

item_ph_boards = { name = "boards",
					description = "Through a crack in the floor boards you can hear the murmur of Gaelic from the public house down stairs.",
					pickUpable = "no",
					parent = nil
				};
function item_ph_boards:command( com )
	if com == "look at" then
		message( "ego", self.description );
		if item_ph_key.found == "no" then
			message( "narrator", "Looking closer you see your key has fallen into the crack!" );
			item_ph_key.found = "yes";
			addItem( item_ph_key, rooms[curRoomIdx].items );
		end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_boards:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

					

-- Room creation
room = Room:new()

-- Add some room consts
room.name = "Pier House - Upstairs room";
room.description = "Basic lodgings for one person. The floor is made of cracked wooden boards. In the center of the room is a modern style single bed with sheets. A single small window looks out on the pier.";
room.items = { item_ph_closet, item_ph_bed, item_ph_mirror, item_ph_door };
room.hiddenItems = { item_ph_window, item_ph_boards }
connectItemsToParent( room.items );
connectItemsToParent( room.hiddenItems );

function room:onEnter()
	message( "ego", "You enter a room." );
	message( "narrator", self.description );
	if self.visited == "no" then
		-- do first time stuff
		message( "ego", "I've lost my key somewhere in the room. Where did I drop it?" );
		message( "ego", "I'll have to look for it." );
		self.visited = "yes";
	end
	showItems(self.items);
end

function room:onExit()
	message( "ego", "You leave the room." );
end

function room:onTurn()
end

room.id = curRoomInitIdx;

rooms[curRoomInitIdx] = room;
curRoomInitIdx = curRoomInitIdx + 1;
