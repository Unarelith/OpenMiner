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
#include "ApplicationStateStack.hpp"
#include "BlockFurnace.hpp"
#include "Exception.hpp"
#include "FurnaceWidget.hpp"
#include "InventoryState.hpp"
#include "Player.hpp"
#include "World.hpp"

BlockFurnace::BlockFurnace() : Block(BlockType::Furnace, 164) {
	m_canUpdate = true;
}

bool BlockFurnace::onBlockActivated(const glm::ivec3 &blockPosition, Player &player, World &world) const {
	BlockData *data = world.getBlockData(blockPosition.x, blockPosition.y, blockPosition.z);
	if (!data)
		throw EXCEPTION("BlockFurnace at (", blockPosition.x, blockPosition.y, blockPosition.z, ") has no inventory");

	auto &inventoryState = ApplicationStateStack::getInstance().push<InventoryState>(&ApplicationStateStack::getInstance().top());
	inventoryState.setupWidget<FurnaceWidget>(player.inventory(), player.hotbarInventory(), *data);
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

	if (ticksRemaining == 0 && fuelStack.amount() && inputStack.amount() && inputStack.item().id() == ItemType::IronOre) {
		data->inventory.setStack(2, 0, fuelStack.item().id(), fuelStack.amount() - 1);
		ticksRemaining = fuelStack.item().burnTime();
		currentBurnTime = fuelStack.item().burnTime();
	}
	else if (ticksRemaining > 0 && (!outputStack.amount() || !outputStack.item().id() || outputStack.item().id() == ItemType::IronIngot)) { // FIXME
		--ticksRemaining;
		++itemProgress;
	}
	else if (ticksRemaining == 0) {
		currentBurnTime = 0;
	}

	if (itemProgress >= 200) {
		itemProgress = 0;
		if (inputStack.item().id() == ItemType::IronOre && inputStack.amount()) {
			data->inventory.setStack(0, 0, inputStack.item().id(), inputStack.amount() - 1);
			data->inventory.setStack(1, 0, ItemType::IronIngot, outputStack.amount() + 1);
		}
	}

	data->data = ticksRemaining | (currentBurnTime << 16) | ((u32)itemProgress << 32);
}

