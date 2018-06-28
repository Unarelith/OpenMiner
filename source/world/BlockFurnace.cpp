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
#include "FurnaceWidget.hpp"
#include "InventoryState.hpp"
#include "Player.hpp"

BlockFurnace::BlockFurnace() : Block(BlockType::Furnace, 164) {
}

bool BlockFurnace::onBlockActivated(const glm::ivec3 &, Player &player, World &) const {
	auto &inventoryState = ApplicationStateStack::getInstance().push<InventoryState>(&ApplicationStateStack::getInstance().top());
	inventoryState.setupWidget<FurnaceWidget>(player.inventory(), player.hotbarInventory());
	return true;
}

void BlockFurnace::onTick(const glm::ivec3 &blockPosition, Player &player, Chunk &chunk, World &world) const {
}

