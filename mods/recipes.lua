-- Wooden Axe
registry:register_crafting_recipe({
	result = {
		item = 41,
		amount = 1
	},

	pattern = {
		"##",
		"#|",
		" |"
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Wooden Hoe
registry:register_crafting_recipe({
	result = {
		item = 42,
		amount = 1
	},

	pattern = {
		"##",
		" |",
		" |"
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Wooden Pickaxe
registry:register_crafting_recipe({
	result = {
		item = 43,
		amount = 1
	},

	pattern = {
		"###",
		" | ",
		" | "
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Wooden Shovel
registry:register_crafting_recipe({
	result = {
		item = 44,
		amount = 1
	},

	pattern = {
		"#",
		"|",
		"|"
	},

	keys = {
		['#'] = 11,
		['|'] = 32
	}
})

-- Wooden Sword
registry:register_crafting_recipe({
	result = {
		item = 45,
		amount = 1
	},

	pattern = {
		"#",
		"#",
		"|"
	},

	keys = {
		['#'] = 11,
		['|'] = 32
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
		item = 32,
		amount = 4
	},

	pattern = {
		'#',
		'#'
	},

	keys = {['#'] = 11}
})

-- Planks
registry:register_crafting_recipe({
	result = {
		item = 11,
		amount = 4
	},
	pattern = {"#"},
	keys = {["#"] = 5}
})

-- Workbench
registry:register_crafting_recipe({
	result = {
		item = 13,
		amount = 1
	},
	pattern = {
		"##",
		"##"
	},
	keys = {["#"] = 11}
})

-- Furnace
registry:register_crafting_recipe({
	result = {
		item = 14,
		amount = 1
	},

	pattern = {
		"###",
		"# #",
		"###"
	},

	keys = {["#"] = 2}
})


-- Plank Slab
registry:register_crafting_recipe({
	result = {
		item = 16,
		amount = 6
	},

	pattern = {"###"},

	keys = {['#'] = 11}
})

-- Iron Ingot
registry:register_smelting_recipe({
	input = {item = 15, amount = 1},
	output = {item = 39, amount = 1}
})

-- Stone
registry:register_smelting_recipe({
	input = {item = 2, amount = 1},
	output = {item = 6, amount = 1}
})

-- Glass
registry:register_smelting_recipe({
	input = {item = 7, amount = 1},
	output = {item = 9, amount = 1}
})

-- Charcoal
registry:register_smelting_recipe({
	input = {item = 5, amount = 1},
	output = {item = 40, amount = 1}
})

