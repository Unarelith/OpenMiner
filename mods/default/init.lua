print("Hello from Lua!")

-- FIXME
SCREEN_WIDTH = 1600
SCREEN_HEIGHT = 1050
GUI_SCALE = 3

mod = LuaMod.new("default")

dofile("mods/default/blocks.lua")
dofile("mods/default/items.lua")
dofile("mods/default/recipes.lua")

openminer:world():terrain_generator():set_blocks({
	dirt = "default:dirt",
	grass = "default:grass",
	stone = "default:stone",
	log = "default:wood",
	leaves = "default:leaves",
	flower = "default:flower",
	water = "default:water",
	sand = "default:sand"
})

function init(player)
	local player_inv = player:inventory()

	player_inv:add_stack("default:workbench", 1);
	player_inv:add_stack("default:dirt", 64);
	player_inv:add_stack("default:grass", 64);
	player_inv:add_stack("default:stone", 64);
	player_inv:add_stack("default:glass", 64);
	player_inv:add_stack("default:glowstone", 64);
	player_inv:add_stack("default:furnace", 1);
	player_inv:add_stack("default:stone_pickaxe", 1);
	player_inv:add_stack("default:stone_axe", 1);

	player_inv:add_stack("default:wood", 64);
	player_inv:add_stack("default:planks", 64);
	player_inv:add_stack("default:cobblestone", 64);
	player_inv:add_stack("default:stick", 64);
	player_inv:add_stack("default:stone_hoe", 1);
	player_inv:add_stack("default:stone_shovel", 1);
	player_inv:add_stack("default:iron_ore", 64);
	player_inv:add_stack("default:coal", 64);
end

function show_inventory(client)
	local gui = LuaGUI.new()

	-- FIXME: Replace this by gui:set_size() and gui:set_centered()
	local gui_pos = {
		x = SCREEN_WIDTH / GUI_SCALE / 2.0 - 176 / 2.0,
		y = SCREEN_HEIGHT / GUI_SCALE / 2.0 - 166 / 2.0
	}

	gui:player_inventory {
		name = "inventory",
		pos = gui_pos,
	}

	gui:show(client)
end

