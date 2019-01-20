-- Wooden Axe
mod:crafting_recipe {
	result = {
		id = "default:axe_wood",
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
}

-- Wooden Hoe
mod:crafting_recipe {
	result = {
		id = "default:hoe_wood",
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
}

-- Wooden Pickaxe
mod:crafting_recipe {
	result = {
		id = "default:pickaxe_wood",
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
}

-- Wooden Shovel
mod:crafting_recipe {
	result = {
		id = "default:shovel_wood",
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
}

-- Wooden Sword
mod:crafting_recipe {
	result = {
		id = "default:sword_wood",
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
}

-- Stone Axe
mod:crafting_recipe {
	result = {
		id = "default:axe_stone",
		amount = 1
	},

	pattern = {
		"##",
		"#|",
		" |"
	},

	keys = {
		['#'] = "default:cobblestone",
		['|'] = "default:stick",
	}
}

-- Stone Hoe
mod:crafting_recipe {
	result = {
		id = "default:hoe_stone",
		amount = 1
	},

	pattern = {
		"##",
		" |",
		" |"
	},

	keys = {
		['#'] = "default:cobblestone",
		['|'] = "default:stick",
	}
}

-- Stone Pickaxe
mod:crafting_recipe {
	result = {
		id = "default:pickaxe_stone",
		amount = 1
	},

	pattern = {
		"###",
		" | ",
		" | "
	},

	keys = {
		['#'] = "default:cobblestone",
		['|'] = "default:stick",
	}
}

-- Stone Shovel
mod:crafting_recipe {
	result = {
		id = "default:shovel_stone",
		amount = 1
	},

	pattern = {
		"#",
		"|",
		"|"
	},

	keys = {
		['#'] = "default:cobblestone",
		['|'] = "default:stick",
	}
}

-- Stone Sword
mod:crafting_recipe {
	result = {
		id = "default:sword_stone",
		amount = 1
	},

	pattern = {
		"#",
		"#",
		"|"
	},

	keys = {
		['#'] = "default:cobblestone",
		['|'] = "default:stick",
	}
}

-- Stick
mod:crafting_recipe {
	result = {
		id = "default:stick",
		amount = 4
	},

	pattern = {
		'#',
		'#'
	},

	keys = {['#'] = "default:planks"}
}

-- Planks
mod:crafting_recipe {
	result = {
		id = "default:planks",
		amount = 4
	},
	pattern = {"#"},
	keys = {["#"] = "default:wood"}
}

-- Workbench
mod:crafting_recipe {
	result = {
		id = "default:workbench",
		amount = 1
	},
	pattern = {
		"##",
		"##"
	},
	keys = {["#"] = "default:planks"}
}

-- Furnace
mod:crafting_recipe {
	result = {
		id = "default:furnace",
		amount = 1
	},

	pattern = {
		"###",
		"# #",
		"###"
	},

	keys = {["#"] = "default:cobblestone"}
}


-- Plank Slab
mod:crafting_recipe {
	result = {
		id = "default:slab_planks",
		amount = 6
	},

	pattern = {"###"},

	keys = {['#'] = "default:planks"}
}

-- Glowstone
mod:crafting_recipe {
	result = {
		id = "default:glowstone",
		amount = 99
	},

	pattern = {"#"},

	keys = {['#'] = "default:leaves"},
}

-- Iron Ingot
mod:smelting_recipe {
	input = {id = "default:ore_iron", amount = 1},
	output = {id = "default:ingot_iron", amount = 1}
}

-- Stone
mod:smelting_recipe {
	input = {id = "default:cobblestone", amount = 1},
	output = {id = "default:stone", amount = 1}
}

-- Glass
mod:smelting_recipe {
	input = {id = "default:sand", amount = 1},
	output = {id = "default:glass", amount = 1}
}

-- Charcoal
mod:smelting_recipe {
	input = {id = "default:wood", amount = 1},
	output = {id = "default:charcoal", amount = 1}
}

