registry:register_block(0, 0,  "Air")
registry:register_block(1, 37, "Dirt")

local cobblestone = registry:register_block(2, 38, "Cobblestone")
cobblestone.hardness = 2
cobblestone.harvest_requirements = 1

registry:register_block(3, 226, "Grass"):set_item_drop(1, 1)
registry:register_block(4, 266, "Leaves").hardness = 0.5
registry:register_block(5, 277, "Wood").hardness = 2

local stone = registry:register_block(6, 402, "Stone")
stone.hardness = 1.5
stone.harvest_requirements = 1
stone:set_item_drop(2, 1)

registry:register_block(7, 369, "Sand")
registry:register_block(8, 457, "Water")
registry:register_block(9, 168, "Glass")

local coalOre = registry:register_block(10, 36, "Coal Ore")
coalOre.hardness = 3
coalOre.harvest_requirements = 1
coalOre:set_item_drop(38, 1)

registry:register_block(11, 316, "Planks")
registry:register_block(12, 218, "Glowstone")
registry:register_block(13, 77,  "Workbench")
registry:register_block(14, 164, "Furnace")

local ironOre = registry:register_block(15, 254, "Iron Ore")
ironOre.hardness = 3
ironOre.harvest_requirements = 1

registry:register_block(16, 316, "Plank Slab")

for i = 17, 31 do
	registry:register_block(i, 4, "Undefined")
end

registry:register_item_block(0,  "")
registry:register_item_block(1,  "Dirt")
registry:register_item_block(2,  "Cobblestone")
registry:register_item_block(3,  "Grass")
registry:register_item_block(4,  "Leaves")
registry:register_item_block(5,  "Wood")
registry:register_item_block(6,  "Stone")
registry:register_item_block(7,  "Sand")
registry:register_item_block(8,  "Water")
registry:register_item_block(9,  "Glass")
registry:register_item_block(10, "Coal Ore")
registry:register_item_block(11, "Planks")
registry:register_item_block(12, "Glowstone")
registry:register_item_block(13, "Workbench")
registry:register_item_block(14, "Furnace")
registry:register_item_block(15, "Iron Ore")
registry:register_item_block(16, "Plank Slab")

for i = 17, 31 do
	registry:register_item_block(i, "Undefined")
end

registry:register_item(32, "Stick", 324)

local stone_axe = registry:register_item(33, "Stone Axe", 325)
stone_axe.mining_speed = 4
stone_axe.harvest_capability = 4

registry:register_item(34, "Stone Hoe", 326)

local stone_pickaxe = registry:register_item(35, "Stone Pickaxe", 327)
stone_pickaxe.mining_speed = 4
stone_pickaxe.harvest_capability = 1

local stoneShovel = registry:register_item(36, "Stone Shovel", 328)
stoneShovel.mining_speed = 4
stoneShovel.harvest_capability = 2

registry:register_item(37, "Stone Sword", 329)

local coal = registry:register_item(38, "Coal", 111)
coal.is_fuel = true
coal.burn_time = 1600

registry:register_item(39, "Iron Ingot", 232)

local charcoal = registry:register_item(40, "Charcoal", 41)
charcoal.is_fuel = true
charcoal.burn_time = 1600

local woodenAxe = registry:register_item(41, "Wooden Axe", 337)
woodenAxe.mining_speed = 2
woodenAxe.harvest_capability = 4

registry:register_item(42, "Wooden Hoe", 338)

local woodenPickaxe = registry:register_item(43, "Wooden Pickaxe", 339)
woodenPickaxe.mining_speed = 2
woodenPickaxe.harvest_capability = 1

local woodenShovel = registry:register_item(44, "Wooden Shovel", 340)
woodenShovel.mining_speed = 2
woodenShovel.harvest_capability = 2

registry:register_item(45, "Wooden Sword", 341)

