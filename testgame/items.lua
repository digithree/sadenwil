-----
-- Items
-----

item_invalid = { name = "invalid", description = "no description", pickUpable = "no" };

function connectItemsToParent( list )
	for i, item in ipairs(list) do
		item.parent = list;
	end
end


function showItems( items )
	local str = "I see- ";
	local c = 1
	for i, item in ipairs(items) do
		if item.name ~= "invalid" then
			if c > 1 then
				str = str .. ", ";
			end
			str = str .. item.name;
			c = c + 1;
		end
	end
	message( "ego", str );
end

function addItem( item, list )
	for i, listItem in ipairs(list) do
		if listItem.name == "invalid" then
			list[i] = item;
			item.parent = list;
			return;
		end
	end
	local idx = table.getn(list);
	list[idx+1] = item;
	item.parent = list;
end

function removeItemViaName( itemName, list )
	for i, listItem in ipairs(list) do
		if listItem.name == itemName then
			listItem.parent = nil;
			local ret = listItem;
			list[i] = item_invalid;
			return ret;
		end
	end
end

function removeItemViaInstance( item )
	if item.parent ~= nil then
		for i, listItem in ipairs(item.parent) do
			if listItem.name == item.name then
				local ret = item;
				item.parent[i] = item_invalid;
				item.parent = nil;
				return ret;
			end
		end
	end
	return nil;
end


inventory = {};

item_inventory = { name = "inventory",
					description = "A white woolen knitted patterned jumper. Known as an Aran jumper.",
					pickUpable = "no",
					parent = nil
				};
function item_inventory:command( com )
	if com == "look at" then
		local str = "nothing";
		if table.getn(inventory) > 0 then
			str = "";
			for i, item in ipairs(inventory) do
				if i > 1 then
					str = str .. ", ";
				end
				str = str .. item.name;
			end
		end
		message( "You have", str );
	else
		message( "ego", "That doesn't work." );
	end
end

globalItems = {};
addItem( item_inventory, globalItems );
