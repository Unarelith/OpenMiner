print("Hello from Lua!")

mod = LuaMod.new("default")

dofile("mods/blocks.lua")
dofile("mods/items.lua")
dofile("mods/recipes.lua")

function init()
	player:inventory():add_stack("default:workbench", 1);
	player:inventory():add_stack("default:dirt", 64);
	player:inventory():add_stack("default:grass", 64);
	player:inventory():add_stack("default:stone", 64);
	player:inventory():add_stack("default:glass", 64);
	player:inventory():add_stack("default:glowstone", 64);
	player:inventory():add_stack("default:furnace", 1);
	player:inventory():add_stack("default:slab_planks", 64);
	player:inventory():add_stack("default:pickaxe_stone", 1);

	player:inventory():add_stack("default:wood", 64);
	player:inventory():add_stack("default:planks", 64);
	player:inventory():add_stack("default:cobblestone", 64);
	player:inventory():add_stack("default:stick", 64);
	player:inventory():add_stack("default:axe_stone", 1);
	player:inventory():add_stack("default:hoe_stone", 1);
	player:inventory():add_stack("default:pickaxe_stone", 1);
	player:inventory():add_stack("default:shovel_stone", 1);
	player:inventory():add_stack("default:ore_iron", 64);
	player:inventory():add_stack("default:coal", 64);
end

