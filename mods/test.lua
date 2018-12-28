print("Hello from Lua!")

dofile("mods/blocks.lua")
dofile("mods/items.lua")
dofile("mods/recipes.lua")

function init()
	player:inventory():add_stack(13, 1);
	player:inventory():add_stack(1, 64);
	player:inventory():add_stack(3, 64);
	player:inventory():add_stack(6, 64);
	player:inventory():add_stack(9, 64);
	player:inventory():add_stack(12, 64);
	player:inventory():add_stack(14, 1);
	player:inventory():add_stack(16, 64);
	player:inventory():add_stack(35, 1);
	player:inventory():add_stack(5, 64);
	player:inventory():add_stack(11, 64);
	player:inventory():add_stack(2, 64);
	player:inventory():add_stack(32, 64);
	player:inventory():add_stack(33, 1);
	player:inventory():add_stack(34, 1);
	player:inventory():add_stack(35, 1);
	player:inventory():add_stack(36, 1);
	player:inventory():add_stack(15, 64);
	player:inventory():add_stack(38, 64);
end

