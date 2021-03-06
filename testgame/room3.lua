-----
-- Test Room
-----

-- Room 3 --


item_ph_sandwich = { 	name = "sandwich",
						description = "A sandwich with cured salted pork inside.",
						pickUpable = "no"
					};
function item_ph_sandwich:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "pick up" then
		message( "narrator", "The bar man sees you pick up the sandwich." );
		message( "bar man", "If you're hungry now why don't you eat it? It'll get ruined in your pocket." );
		message( "narrator", "You leave down the sandwich." );
	elseif com == "use" then
		message( "narrator", "You pick up the sandwich and eat it." );
		message( "bar man", "Good stuff eh? Tis the best bread around, made by my wife." );
		removeItemViaInstance( self );
	else
		message( "That doesn't work." );
	end
end
function item_ph_sandwich:commandDouble( com, item )
	if com == "give" and item.name == "dog" then
		message( "narrator", "You give the sandwich to the dog." );
		message( "dog", "Woof!" );
		message( "narrator", "The dog gobbles it up double quick." );
		message( "bar man", "Hey! I spent five turns making that sandwich and you just gave it to the dog!" );
		message( "old man", "Heh heh." );
		removeItemViaInstance( self );
	else
		message( "ego", "That doesn't work." );
	end
end


item_ph_shilelagh = { 	name = "shilelagh",
						description = "A knobly length of ashen wood.",
						pickUpable = "yes",
						inBar = "yes"
					};
function item_ph_shilelagh:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "pick up" then
		if self.parent ~= inventory then
			if self.inBar then
				message( "old man", "So you want my shilelagh eh? Bedad stranger, have it." );
				self.inBar = "no";
			end
			message( "ego", "You pick up the shilelagh." );
			removeItemViaInstance( self );
			addItem( self, inventory );
			message( "ego", "Shilelagh is now in inventory.");
		else
			message( "ego", "You already have it." ); --TODO generalise this to executeCommand(...)
		end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_shilelagh:commandDouble( com, item )
	if com == "use" and item.name == "dog" then
		if self.usedOnce ~= "yes" then
			message( "dog", "Grrrrrr!!!!" );
			message( "old man", "Leave me dog alone! Ya skyup ya!" );
			message( "bar man", "Any more of that and there's no pints for you! Jaysus." );
			message( "ego", "What the hell got into me there? Better not do that again....");
			self.usedOnce = "yes";
		else
			message( "dog", "Yelllpp!" );
			message( "narrator", "You hit the dog." );
			message( "old man", "Sprinkles! No, you hit sprinkles!" );
			message( "bar man", "That's it!" );
			message( "narrator", "The bar man shoots you with a shotgun. You die." );
			-- TODO kill character
		end
	else
		message( "ego", "That doesn't work." );
	end
end

item_ph_newspaper = {	name = "newspaper",
						description = "A week old edition of An Phoblacht newspaper.",
						pickUpable = "yes"
					};
function item_ph_newspaper:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "pick up" then
		message( "bar man", "Ah leave it at the bar, it's our only copy." );
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_newspaper:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

item_ph_bottle = {		name = "bottle",
						description = "A bottle of clear liquid.",
						pickUpable = "yes"
					};
function item_ph_bottle:command( com )
	if com == "look at" then
		message( "ego", self.description );
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_bottle:commandDouble( com, item )
	if item.name == "pint" then
		if item.isFull == "yes" then
			if item.contains == "stout" then
				message( "ego", "I don't want to pour that strong stuff on top of my stout!" );
			elseif item.contains == "poteen" then
				message( "ego", "There's already poteen inside in it." );
			end
			return;
		end
		if item_ph_barman.parent ~= nil then
			message( "narrator", "You reach out and take hold of the bottle." );
			message( "bar man", "Now stranger, I can't let you drink that, it'd kill ya." );
			message( "narrator", "You leave the bottle alone." );
			return;
		end
		if item.finished == "yes" then
			message( "ego", "I've already had some of that, any more would kill me!" );
			return;
		end
		-- otherwise, the pint is filled
		message( "narrator", "You pour a drop of poteen into the pint glass." );
		item.contains = "poteen";
		item.isFull = "yes";
	else
		message( "ego", "That doesn't work." );
	end
end

item_ph_pint = { name = "pint",
				description = "It's a pint glass.",
				pickUpable = "yes",
				isFull = "yes",
				contains = "nothing",
				finished = "no"
				}
function item_ph_pint:command( com )
	if com == "look at" then
		message( "ego", self.description );
		message( "ego", "It contains " .. self.contains .. "." );
	elseif com == "use" or com == "pick up" then
		if self.isFull == "yes" and self.contains == "stout" then
			message( "narrator", "You drink the pint of stout and put it back on the bar." );
			self.isFull = "no";
			self.contains = "nothing";
		elseif self.isFull == "yes" and self.contains == "poteen" then
			message( "narrator", "You take a cautious sip of the clear liquid." );
			message( "narrator", "It hits your throat like a burning fire. You cough uncontrolably for a minute." );
			message( "you", "Jesus! This stuff is strong!" );
			message( "narrator", "The old man laughs at you." );
			message( "old man", "Ah young man, there you have it! Good for the health, slante!" );
			self.isFull = "no";
			self.contains = "nothing";
			self.finished = "yes";
		else
			message( "narrator", "There's nothing left in the glass to drink.");
		end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_pint:commandDouble( com, item )
	if item.name == "bottle" then
		message( "ego", "Don't I mean that the other way around?" );
	else
		message( "ego", "That doesn't work." );
	end
end

item_ph_barman = {	name = "bar man",
					description = "A young man wearing drab worn woolen suit. He looks to be planning a way out.",
					pickUpable = "no",
					hasPouredPint = "no",
					hasLeft = "no",
					turnsUntilBack = 0
				};	
function dialog_barman( line, nothing )
	local finished = 0
	if line == "1" then
		message( "you", dialogOptions[1] );
		message( "bar man", "Fine weather for ducks!" );
		finished = -1
	elseif line == "2" then
		message( "you", dialogOptions[2] );
		if item_ph_barman.hasPouredPint == "no" then
			message( "bar man", "Right so." );
			message( "narrator", "The bar man pours you a pint." );
			message( "bar man", "Here you are." );
			message( "narrator", "The bar man sets the pint down on the bar." );
			addItem( item_ph_pint, rooms[curRoomIdx].items );
			item_ph_pint.contains = "stout";
			item_ph_barman.hasPouredPint = "yes";
		else
			message( "bar man", "I think you've had enough sir." );
		end
		finished = -1
	elseif line == "3" then
		message( "you", dialogOptions[3] );
		message( "bar man", "Local mans poteen. I'd say it's a bit strong fer ya.");
		finished = -1
	elseif line == "4" then
		message( "you", dialogOptions[4] );
		if item_ph_barman.hasLeft == "no" then
			message( "bar man", "Right so stranger, it'll be a few minutes then.");
			message( "narrator", "The bar man disapears into the back room. No one is watching the bar now." );
			removeItemViaInstance( item_ph_barman );
			item_ph_barman.hasLeft = "yes";
			item_ph_barman.turnsUntilBack = 5;
		else
			message( "bar man", "You can't be so hungry again?! Anyway, that's the last of the bread gone, sorry stranger." );
		end
		finished = -1
	else
		message( "narrator", "Please choose an option, 1 to 4" );
	end
	-- is finished?
	if finished ~= 0 then
		processFunction = executeCommand;
		activeFunction = 1;
	end
end
function item_ph_barman:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "talk to" then
		message( "narrator", "What do you want to say? (choose a number)" );
		dialogOptions = { "How's it goin?", "A pint please.", "What's in that bottle?", "Can you make me a sandwich?" };
		showDialogOptions();
		processFunction = dialog_barman;
		activeFunction = 2;
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_barman:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end


item_ph_oldman = {	name = "old man",
					description = "An old man hunched over a pint of stout. He ignores his smouldering pipe and box of matches sitting beside him at the bar.",
					pickUpable = "no",
					says = { "Young man.", "Come over here will ya.", "Hey young fella.", "Hey." },
					saysCount = 1,
					talkedTo = "no",
					-- for cut scene
					cutSceneCount = 1
				};
function item_ph_oldman:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "talk to" then
		if self.talkedTo == "no" then
			message( "narrator", "You approach the old man sitting at the bar." );
			message( "old man", "Pull up a chair here beside me young fla'." );
			message( "narrator", "You grab a barstool, set it beside the old man and sit on it." );
			message( "old man", "Let me tell you a story." );
			message( "ego", "Please do." );
			message( "old man", "A farmer was in great distress as his crops had failed, and his cow had died on him. One night he told his wife to make him a fine new sack for flour before the next morning; and when it was finished he started off with it before the dawn." );
			message( "old man", "At that time there was a gentleman who had been taken by the fairies, and made an officer among them, and it was often people would see him and him riding on a white horse at dawn and in the evening." );
			message( "old man", "The poor man went down to the place where they used to see the officer, and when he came by on his horse, he asked the loan of two hundred and a half of flour, for he was in great want." );
			message( "old man", "The officer called the fairies out of a hole in the rocks where they stored their wheat, and told them to give the poor man what he was asking. Then he told him to come back and pay him in a year, and rode away." );
			message( "old man", "When the poor man got home he wrote down the day on a piece of paper, and that day year he came back and paid the officer." );
			message( "old man", "That is my story." );
			self.talkedTo = "yes";
		else
			message( "old man", "Sure I've said my piece lad, leave me be now." );
		end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_oldman:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

item_ph_dog = {		name = "dog",
					description = "An old sheep dog sitting quietly under the old mans stool.",
					pickUpable = "no",
					talkCounter = 1
				};
function item_ph_dog:command( com )
	if com == "look at" then
		message( "ego", self.description );
	elseif com == "talk to" then
		message( "ego", "Here doggy!" );
		if self.talkCounter == 1 then
			message( "dog", "Grrrrrr!!!" );
		elseif self.talkCounter == 2 then
			message( "dog", "Woof woof!!!" );
		elseif self.talkCounter == 3 then
			message( "dog", "Arfff arfff!" );
		end
		self.talkCounter = self.talkCounter + 1
		if self.talkCounter > 3 then self.talkCounter = 1 end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_dog:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end

item_ph_frontdoor = {	name = "door",
						description = "A rickity old door.",
						pickUpable = "no",
						open = "no"
					};
function item_ph_frontdoor:command( com )
	if com == "look at" then
		message( "ego", self.description );
		if self.open == "yes" then
			message( "ego", "It's open." );
		else
			message( "ego", "It's closed." );
		end
		if item_ph_oldman.talkedTo == "no" then
			message( "narrator", "Through a crack in the door you can see it's pissing down outside. Better find something to do in the bar until it stops." );
		else
			message( "narrator", "Through a crack in the door you can see that the rain has stopped." );
		end
	elseif com == "open" then
		if item_ph_oldman.talkedTo == "no" then
			message( "bar man", "Hey, leave that door closed until this rain stops! Why don't you have a pint or a chat while you wait?" );
		else
			if self.open == "no" then
				self.open = "yes"
				message( "ego", "You opened the door." );
			else
				message( "ego", "The door is already open." );
			end
		end
	elseif com == "closed" then
		if self.open == "yes" then
			self.open = "no"
			message( "ego", "You closed the door." );
		else
			message( "ego", "The door is already closed." );
		end
	else
		message( "ego", "That doesn't work." );
	end
end
function item_ph_frontdoor:commandDouble( com, item )
	message( "ego", "That doesn't work." );
end


-- Room creation
room = Room:new()

-- Add some room consts
room.name = "Pier House - Public House";
room.description = "You are in a small traditional island pub. It has a bar with one tap and a divider at the end of the bar for the snug. The barman is behind the bar; an old man is at the bar with his dog underneath the barstool. Beside the bar is a stairs leading to the first floor.";
room.items = { item_ph_shilelagh, item_ph_newspaper, item_ph_bottle, item_ph_stairs, item_ph_frontdoor, item_ph_barman, item_ph_oldman, item_ph_dog };
room.hiddenItems = { };
connectItemsToParent( room.items );
-- connectItemsToParent( room.hiddenItems );

function room:onEnter()
	message( "ego", "You enter the public house." );
	message( "narrator", self.description );
	if self.visited == "no" then
		-- do first time stuff
		self.visited = "yes";
	end
	showItems(self.items);
end;

function room:onExit()
	message( "ego", "You leave the public house." );
end

function room:onTurn()
	if item_ph_oldman.talkedTo == "no" then
		message( "old man", item_ph_oldman.says[item_ph_oldman.saysCount] );
		item_ph_oldman.saysCount = item_ph_oldman.saysCount + 1;
		if item_ph_oldman.saysCount > table.getn(item_ph_oldman.says) then
			item_ph_oldman.saysCount = 1;
		end
	end
	if item_ph_barman.hasLeft == "yes" then
		item_ph_barman.turnsUntilBack = item_ph_barman.turnsUntilBack - 1;
		if item_ph_barman.turnsUntilBack == 0 then
			addItem( item_ph_barman, rooms[curRoomIdx].items );
			message( "narrator", "The bar man comes back carrying a sandwich on a piece of cloth." );
			message( "bar man", "There you are stranger, eat well." );
			message( "narrator", "The bar man sets the sandwich down on the bar." );
			addItem( item_ph_sandwich, rooms[curRoomIdx].items );
			item_ph_barman.hasLeft = "no";
		else
			message( "narrator", "You can hear the bar man working in the kitchen." );
		end
	end
end

rooms[curRoomInitIdx] = room;
curRoomInitIdx = curRoomInitIdx + 1;
