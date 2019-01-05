/*
 * =====================================================================================
 *
 *       Filename:  BlockFurnace.cpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:00:35
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <gk/core/ApplicationStateStack.hpp>
#include <gk/core/Exception.hpp>

#include "BlockFurnace.hpp"
#include "FurnaceWidget.hpp"
#include "InventoryState.hpp"
#include "Player.hpp"
#include "Registry.hpp"
#include "World.hpp"

BlockFurnace::BlockFurnace() : Block(BlockType::Furnace, 164, "default:furnace", "Furnace") {
}

bool BlockFurnace::onBlockActivated(const glm::ivec3 &blockPosition, Player &player, World &world) const {
	BlockData *data = world.getBlockData(blockPosition.x, blockPosition.y, blockPosition.z);
	if (!data)
		throw EXCEPTION("BlockFurnace at (", blockPosition.x, blockPosition.y, blockPosition.z, ") has no inventory");

	auto &inventoryState = gk::ApplicationStateStack::getInstance().push<InventoryState>(&gk::ApplicationStateStack::getInstance().top());
	inventoryState.setupWidget<FurnaceWidget>(player.inventory(), *data);
	return true;
}

void BlockFurnace::onTick(const glm::ivec3 &blockPosition, Player &, Chunk &, World &world) const {
	BlockData *data = world.getBlockData(blockPosition.x, blockPosition.y, blockPosition.z);
	if (!data)
		throw EXCEPTION("BlockFurnace at (", blockPosition.x, blockPosition.y, blockPosition.z, ") has no inventory");

	const ItemStack &inputStack = data->inventory.getStack(0, 0);
	const ItemStack &outputStack = data->inventory.getStack(1, 0);
	const ItemStack &fuelStack = data->inventory.getStack(2, 0);

	u16 ticksRemaining = data->data & 0xffff;
	u16 currentBurnTime = (data->data >> 16) & 0xffff;
	u16 itemProgress = (data->data >> 32) & 0xffff;

	const Recipe *recipe = Registry::getInstance().getRecipe(data->inventory);
	if (recipe && recipe->type() != "smelt")
		recipe = nullptr;

	if (ticksRemaining == 0 && recipe && fuelStack.amount() && (!outputStack.item().id() || !outputStack.amount() || outputStack.item().id() == recipe->result().item().id())) {
		data->inventory.setStack(2, 0, fuelStack.item().name(), fuelStack.amount() - 1);
		ticksRemaining = fuelStack.item().burnTime();
		currentBurnTime = fuelStack.item().burnTime();
		world.setData(blockPosition.x, blockPosition.y, blockPosition.z, 1);
	}
	else if (ticksRemaining > 0) {
		--ticksRemaining;
		if (recipe && (!outputStack.item().id() || !outputStack.amount() || outputStack.item().id() == recipe->result().item().id()))
			++itemProgress;
		else
			itemProgress = 0;
	}
	else if (ticksRemaining == 0) {
		currentBurnTime = 0;
		world.setData(blockPosition.x, blockPosition.y, blockPosition.z, 0);
	}

	if (itemProgress >= 200 && recipe) {
		itemProgress = 0;
		data->inventory.setStack(0, 0, (inputStack.amount() - 1 > 0) ? inputStack.item().name() : "", inputStack.amount() - 1);
		data->inventory.setStack(1, 0, recipe->result().item().name(), outputStack.amount() + recipe->result().amount());
	}

	data->data = ticksRemaining | ((u32)currentBurnTime << 16) | ((u64)itemProgress << 32);
}

