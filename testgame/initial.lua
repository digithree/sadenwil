-----
-- Initial conditions
-----


-- *** for rooms ***
firstRoom = "room1"

Room = { name = "voidRoom", 
		visited = "no",
		items = {},
		hiddenItems = {},
		id = -1
		};

function Room:getItems()
	local items = {};
	local c = 1;
	for i, item in ipairs(self.items) do
		items[c] = item;
		c = c + 1;
	end
	for i, item in ipairs(self.hiddenItems) do
		items[c] = item;
		c = c + 1;
	end
	return items;
end

function Room:new (obj)
	obj = obj or {}
	setmetatable(obj, self)
	self.__index = self
	return obj
end

-- all rooms
rooms = {};
roomScriptNames = { "room1.lua" } -- TODO set back to room1
-- for init
curRoomInitIdx = 1;
curRoomIdx = 1;


