registry:register_block({
	id = 0,
	texture = 0,
	name = "Air",
})

registry:register_block({
	id = 1,
	texture = 37,
	name = "Dirt",
})

registry:register_block({
	id = 2,
	texture = 38,
	name = "Cobblestone",
	hardness = 2,
	harvest_requirements = 1,
})

registry:register_block({
	id = 3,
	texture = 226,
	name = "Grass",
	item_drop = {
		id = 1,
		amount = 1
	}
})

registry:register_block({
	id = 4,
	texture = 266,
	name = "Leaves",
	hardness = 0.5,
})

registry:register_block({
	id = 5,
	texture = 277,
	name = "Wood",
	hardness = 2
})

registry:register_block({
	id = 6,
	texture = 402,
	name = "Stone",
	hardness = 1.5,
	harvest_requirements = 1,
	item_drop = {
		id = 2,
		amount = 1
	},
})

registry:register_block({
	id = 7,
	texture = 369,
	name = "Sand"
})

registry:register_block({
	id = 8,
	texture = 457,
	name = "Water",
})

registry:register_block({
	id = 9,
	texture = 168,
	name = "Glass",
})

registry:register_block({
	id = 10,
	texture = 36,
	name = "Coal Ore",
	hardness = 3,
	harvest_requirements = 1,
	item_drop = {
		id = 38,
		amount = 1
	},
})

registry:register_block({
	id = 11,
	texture = 316,
	name = "Planks",
})

registry:register_block({
	id = 12,
	texture = 218,
	name = "Glowstone",
})

registry:register_block({
	id = 13,
	texture = 77,
	name = "Workbench",
})

registry:register_block({
	id = 14,
	texture = 164,
	name = "Furnace",
})

registry:register_block({
	id = 15,
	texture = 254,
	name = "Iron Ore",
	hardness = 3,
	harvest_requirements = 1,
})

registry:register_block({
	id = 16,
	texture = 316,
	name = "Plank Slab"
})

for i = 17, 31 do
	registry:register_block({
		id = i,
		texture = 4,
		name = "Undefined"
	})
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

