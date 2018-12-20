print("Hello from Lua!")

Registry:registerBlock(0,  0,   "Air")
Registry:registerBlock(1,  37,  "Dirt")

local cobblestone = Registry:registerBlock(2,  38,  "Cobblestone")
cobblestone.hardness = 2
cobblestone.harvestRequirements = 1

Registry:registerBlock(3,  226, "Grass"):setItemDrop(1, 1)
Registry:registerBlock(4,  266, "Leaves").hardness = 0.5
Registry:registerBlock(5,  277, "Wood").hardness = 2

local stone = Registry:registerBlock(6,  402, "Stone")
stone.hardness = 1.5
stone.harvestRequirements = 1
stone:setItemDrop(2, 1)

Registry:registerBlock(7,  369, "Sand")
Registry:registerBlock(8,  457, "Water")
Registry:registerBlock(9,  168, "Glass")

local coalOre = Registry:registerBlock(10, 36,  "Coal Ore")
coalOre.hardness = 3
coalOre.harvestRequirements = 1
coalOre:setItemDrop(38, 1)

Registry:registerBlock(11, 316, "Planks")
Registry:registerBlock(12, 218, "Glowstone")
Registry:registerBlock(13, 77,  "Workbench")
Registry:registerBlock(14, 164, "Furnace")

local ironOre = Registry:registerBlock(15, 254, "Iron Ore")
ironOre.hardness = 3
ironOre.harvestRequirements = 1

Registry:registerBlock(16, 316, "Plank Slab")

for i = 17, 31 do
	Registry:registerBlock(i, 4, "Undefined")
end

Registry:registerItemBlock(0,  "")
Registry:registerItemBlock(1,  "Dirt")
Registry:registerItemBlock(2,  "Cobblestone")
Registry:registerItemBlock(3,  "Grass")
Registry:registerItemBlock(4,  "Leaves")
Registry:registerItemBlock(5,  "Wood")
Registry:registerItemBlock(6,  "Stone")
Registry:registerItemBlock(7,  "Sand")
Registry:registerItemBlock(8,  "Water")
Registry:registerItemBlock(9,  "Glass")
Registry:registerItemBlock(10, "Coal Ore")
Registry:registerItemBlock(11, "Planks")
Registry:registerItemBlock(12, "Glowstone")
Registry:registerItemBlock(13, "Workbench")
Registry:registerItemBlock(14, "Furnace")
Registry:registerItemBlock(15, "Iron Ore")
Registry:registerItemBlock(16, "Plank Slab")

for i = 17, 31 do
	Registry:registerItemBlock(i, "Undefined")
end

Registry:registerItem(32, "Stick", 324)

local stoneAxe = Registry:registerItem(33, "Stone Axe", 325)
stoneAxe.miningSpeed = 4
stoneAxe.harvestCapability = 4

Registry:registerItem(34, "Stone Hoe", 326)

local stonePickaxe = Registry:registerItem(35, "Stone Pickaxe", 327)
stonePickaxe.miningSpeed = 4
stonePickaxe.harvestCapability = 1

local stoneShovel = Registry:registerItem(36, "Stone Shovel", 328)
stoneShovel.miningSpeed = 4
stoneShovel.harvestCapability = 2

Registry:registerItem(37, "Stone Sword", 329)
Registry:registerItem(38, "Coal", 111).burnTime = 1600
Registry:registerItem(39, "Iron Ingot", 232)
Registry:registerItem(40, "Charcoal", 41).burnTime = 1600

local woodenAxe = Registry:registerItem(41, "Wooden Axe", 337)
woodenAxe.miningSpeed = 2
woodenAxe.harvestCapability = 4

Registry:registerItem(42, "Wooden Hoe", 338)

local woodenPickaxe = Registry:registerItem(43, "Wooden Pickaxe", 339)
woodenPickaxe.miningSpeed = 2
woodenPickaxe.harvestCapability = 1

local woodenShovel = Registry:registerItem(44, "Wooden Shovel", 340)
woodenShovel.miningSpeed = 2
woodenShovel.harvestCapability = 2

Registry:registerItem(45, "Wooden Sword", 341)

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

