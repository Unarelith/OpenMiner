print("Hello from Lua!")

-- FIXME
SCREEN_WIDTH = 1600
SCREEN_HEIGHT = 1050
GUI_SCALE = 3

mod = LuaMod.new("default")

dofile("mods/default/bit.lua") -- FIXME
dofile("mods/default/blocks.lua")
dofile("mods/default/items.lua")
dofile("mods/default/recipes.lua")

function init(player)
	local player_inv = player:inventory()

	player_inv:add_stack("default:workbench", 1);
	player_inv:add_stack("default:dirt", 64);
	player_inv:add_stack("default:grass", 64);
	player_inv:add_stack("default:stone", 64);
	player_inv:add_stack("default:glass", 64);
	player_inv:add_stack("default:glowstone", 64);
	player_inv:add_stack("default:furnace", 1);
	player_inv:add_stack("default:slab_planks", 64);
	player_inv:add_stack("default:stone_pickaxe", 1);

	player_inv:add_stack("default:wood", 64);
	player_inv:add_stack("default:planks", 64);
	player_inv:add_stack("default:cobblestone", 64);
	player_inv:add_stack("default:stick", 64);
	player_inv:add_stack("default:stone_axe", 1);
	player_inv:add_stack("default:stone_hoe", 1);
	player_inv:add_stack("default:stone_pickaxe", 1);
	player_inv:add_stack("default:stone_shovel", 1);
	player_inv:add_stack("default:iron_ore", 64);
	player_inv:add_stack("default:coal", 64);
end

