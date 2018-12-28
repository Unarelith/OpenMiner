-- Wooden Axe
registry:register_crafting_recipe({
	result = {
		item = "default:axe_wood",
		amount = 1
	},

	pattern = {
		"##",
		"#|",
		" |"
	},

	keys = {
		['#'] = "default:planks",
		['|'] = "default:stick",
	}
})

-- Wooden Hoe
registry:register_crafting_recipe({
	result = {
		item = "default:hoe_wood",
		amount = 1
	},

	pattern = {
		"##",
		" |",
		" |"
	},

	keys = {
		['#'] = "default:planks",
		['|'] = "default:stick",
	}
})

-- Wooden Pickaxe
registry:register_crafting_recipe({
	result = {
		item = "default:pickaxe_wood",
		amount = 1
	},

	pattern = {
		"###",
		" | ",
		" | "
	},

	keys = {
		['#'] = "default:planks",
		['|'] = "default:stick",
	}
})

-- Wooden Shovel
registry:register_crafting_recipe({
	result = {
		item = "default:shovel_wood",
		amount = 1
	},

	pattern = {
		"#",
		"|",
		"|"
	},

	keys = {
		['#'] = "default:planks",
		['|'] = "default:stick",
	}
})

-- Wooden Sword
registry:register_crafting_recipe({
	result = {
		item = "default:sword_wood",
		amount = 1
	},

	pattern = {
		"#",
		"#",
		"|"
	},

	keys = {
		['#'] = "default:planks",
		['|'] = "default:stick",
	}
})

-- Stone Axe
registry:register_crafting_recipe({
	result = {
		item = 33,
		amount = 1
	},

	pattern = {
		"##",
		"#|",
		" |"
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stone Hoe
registry:register_crafting_recipe({
	result = {
		item = 34,
		amount = 1
	},

	pattern = {
		"##",
		" |",
		" |"
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stone Pickaxe
registry:register_crafting_recipe({
	result = {
		item = 35,
		amount = 1
	},

	pattern = {
		"###",
		" | ",
		" | "
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stone Shovel
registry:register_crafting_recipe({
	result = {
		item = 36,
		amount = 1
	},

	pattern = {
		"#",
		"|",
		"|"
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stone Sword
registry:register_crafting_recipe({
	result = {
		item = 37,
		amount = 1
	},

	pattern = {
		"#",
		"#",
		"|"
	},

	keys = {
		['#'] = 2,
		['|'] = 32
	}
})

-- Stick
registry:register_crafting_recipe({
	result = {
		item = "default:wood",
		amount = 4
	},

	pattern = {
		'#',
		'#'
	},

	keys = {['#'] = "default:planks"}
})

-- Planks
registry:register_crafting_recipe({
	result = {
		item = "default:planks",
		amount = 4
	},
	pattern = {"#"},
	keys = {["#"] = "default:wood"}
})

-- Workbench
registry:register_crafting_recipe({
	result = {
		item = "default:workbench",
		amount = 1
	},
	pattern = {
		"##",
		"##"
	},
	keys = {["#"] = "default:planks"}
})

-- Furnace
registry:register_crafting_recipe({
	result = {
		item = "default:furnace",
		amount = 1
	},

	pattern = {
		"###",
		"# #",
		"###"
	},

	keys = {["#"] = "default:cobblestone"}
})


-- Plank Slab
registry:register_crafting_recipe({
	result = {
		item = "default:slab_planks",
		amount = 6
	},

	pattern = {"###"},

	keys = {['#'] = "default:planks"}
})

-- Iron Ingot
registry:register_smelting_recipe({
	input = {item = "default:ore_iron", amount = 1},
	output = {item = "default:ingot_iron", amount = 1}
})

-- Stone
registry:register_smelting_recipe({
	input = {item = "default:cobblestone", amount = 1},
	output = {item = "default:stone", amount = 1}
})

-- Glass
registry:register_smelting_recipe({
	input = {item = "default:sand", amount = 1},
	output = {item = "default:glass", amount = 1}
})

-- Charcoal
registry:register_smelting_recipe({
	input = {item = "default:wood", amount = 1},
	output = {item = "default:charcoal", amount = 1}
})

