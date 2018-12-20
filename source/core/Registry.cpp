/*
 * =====================================================================================
 *
 *       Filename:  Registry.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:43:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "CraftingRecipe.hpp"
#include "ItemBlock.hpp"
#include "Registry.hpp"
#include "SmeltingRecipe.hpp"

#include "BlockFurnace.hpp"
#include "BlockWater.hpp"
#include "BlockWorkbench.hpp"
#include "XMLFile.hpp"

Registry *Registry::s_instance = nullptr;

void Registry::registerRecipes() {
	XMLFile doc("resources/config/recipes.xml");

	tinyxml2::XMLElement *recipeElement = doc.FirstChildElement("recipes").FirstChildElement("recipe").ToElement();
	while (recipeElement) {
		std::string type = recipeElement->Attribute("type");
		if (type == "craft") {
			std::vector<std::string> pattern;
			std::map<char, std::vector<u32>> keys;
			ItemStack result;
			bool isShapeless = false;

			tinyxml2::XMLElement *patternElement = recipeElement->FirstChildElement("pattern");
			while (patternElement) {
				pattern.emplace_back(patternElement->Attribute("string"));
				patternElement = patternElement->NextSiblingElement("pattern");
			}

			tinyxml2::XMLElement *keyElement = recipeElement->FirstChildElement("key");
			while (keyElement) {
				char ch = keyElement->Attribute("char")[0];
				u32 item = keyElement->UnsignedAttribute("item");

				std::vector<u32> items;
				items.emplace_back(item);
				keys.emplace(ch, items);

				keyElement = keyElement->NextSiblingElement("key");
			}

			tinyxml2::XMLElement *resultElement = recipeElement->FirstChildElement("result");
			if (resultElement) {
				u16 item = resultElement->UnsignedAttribute("item");
				u16 amount = resultElement->UnsignedAttribute("amount");
				result = ItemStack{item, amount};
			}

			registerRecipe<CraftingRecipe>(pattern, keys, result, isShapeless);
		}
		else if (type == "smelt") {
			tinyxml2::XMLElement *inputElement = recipeElement->FirstChildElement("input");
			u16 inputItem = inputElement->UnsignedAttribute("id");
			u16 inputAmount = inputElement->UnsignedAttribute("amount");
			ItemStack input{inputItem, inputAmount};

			tinyxml2::XMLElement *outputElement = recipeElement->FirstChildElement("output");
			u16 outputItem = outputElement->UnsignedAttribute("id");
			u16 outputAmount = outputElement->UnsignedAttribute("amount");
			ItemStack output{outputItem, outputAmount};

			registerRecipe<SmeltingRecipe>(input, output);
		}

		recipeElement = recipeElement->NextSiblingElement("recipe");
	}
}

const Recipe *Registry::getRecipe(const Inventory &inventory) const {
	for (auto &recipe : m_recipes) {
		if (recipe->isMatching(inventory))
			return recipe.get();
	}
	return nullptr;
}

