-----
-- Test Room
-----

-- Room 2 --

item_ph_picture = {	name = "picture",
					description = "A framed picture of several smiling men in typical Aran dress standing beside an overturned Currach.",
					pickUpable = "no"
				};
function item_ph_picture:command( com )
	if com == "look at" then
		message( "ego", self.description );
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_picture:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

-- hidden items

item_ph_stairs = {	name = "stairs",
						description = "A stairway connecting the public house with the first floor.",
						pickUpable = "no"
					};
function item_ph_stairs:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "walk to" then
		if curRoomIdx == 2 then
			-- leave room 2
			rooms[curRoomIdx]:onExit();
			-- go to room 3
			curRoomIdx = 3;
			rooms[curRoomIdx]:onEnter();
		elseif curRoomIdx == 3 then
			-- leave room 3
			rooms[curRoomIdx]:onExit();
			-- go to room 2
			curRoomIdx = 2;
			rooms[curRoomIdx]:onEnter();
		end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_stairs:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end


-- Room creation
room = Room:new()

-- Add some room consts
room.name = "Pier House - Upstairs hallway";
room.description = "You are in a narrow hallway. You can hear the banter from the pub downstairs rising up from the stairway. The walls are bare and unpapered, but for a single framed picture. The hallway is unlit so it's quite dark here.";
room.items = { item_ph_picture };
room.hiddenItems = { item_ph_stairs, item_ph_door };
connectItemsToParent( room.items );
connectItemsToParent( room.hiddenItems );

function room:onEnter()
	message( "ego", "You enter a hallway." );
	message( "narrator", self.description );
	if self.visited == "no" then
		-- do first time stuff
		self.visited = "yes";
	end
	showItems(self.items);
end;

function room:onExit()
	message( "ego", "You leave the hallway." );
end

function room:onTurn()
end

rooms[curRoomInitIdx] = room;
curRoomInitIdx = curRoomInitIdx + 1;
