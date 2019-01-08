print("Hello from Lua!")

-- FIXME
SCREEN_WIDTH = 1600
SCREEN_HEIGHT = 1050
GUI_SCALE = 3

mod = LuaMod.new("default")

dofile("mods/bit.lua") -- FIXME
dofile("mods/blocks.lua")
dofile("mods/workbench.lua")
dofile("mods/furnace.lua")
dofile("mods/items.lua")
dofile("mods/recipes.lua")

function init()
	local player_inv = openminer:player():inventory()

	player_inv:add_stack("default:workbench", 1);
	player_inv:add_stack("default:dirt", 64);
	player_inv:add_stack("default:grass", 64);
	player_inv:add_stack("default:stone", 64);
	player_inv:add_stack("default:glass", 64);
	player_inv:add_stack("default:glowstone", 64);
	player_inv:add_stack("default:furnace", 1);
	player_inv:add_stack("default:slab_planks", 64);
	player_inv:add_stack("default:pickaxe_stone", 1);

	player_inv:add_stack("default:wood", 64);
	player_inv:add_stack("default:planks", 64);
	player_inv:add_stack("default:cobblestone", 64);
	player_inv:add_stack("default:stick", 64);
	player_inv:add_stack("default:axe_stone", 1);
	player_inv:add_stack("default:hoe_stone", 1);
	player_inv:add_stack("default:pickaxe_stone", 1);
	player_inv:add_stack("default:shovel_stone", 1);
	player_inv:add_stack("default:ore_iron", 64);
	player_inv:add_stack("default:coal", 64);
end

