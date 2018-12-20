print("Hello from Lua!")

dofile("mods/items.lua")
dofile("mods/recipes.lua")

function init()
	Player:inventory():addStack(13, 1);
	Player:inventory():addStack(1, 64);
	Player:inventory():addStack(3, 64);
	Player:inventory():addStack(6, 64);
	Player:inventory():addStack(9, 64);
	Player:inventory():addStack(12, 64);
	Player:inventory():addStack(14, 1);
	Player:inventory():addStack(16, 64);
	Player:inventory():addStack(35, 1);
	Player:inventory():addStack(5, 64);
	Player:inventory():addStack(11, 64);
	Player:inventory():addStack(2, 64);
	Player:inventory():addStack(32, 64);
	Player:inventory():addStack(33, 1);
	Player:inventory():addStack(34, 1);
	Player:inventory():addStack(35, 1);
	Player:inventory():addStack(36, 1);
	Player:inventory():addStack(15, 64);
	Player:inventory():addStack(38, 64);
end

