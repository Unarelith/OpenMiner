/*
 * =====================================================================================
 *
 *       Filename:  SmeltingRecipe.cpp
 *
 *    Description:
 *
 *        Created:  30/06/2018 03:04:30
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "SmeltingRecipe.hpp"

SmeltingRecipe::SmeltingRecipe(const ItemStack &input, const ItemStack &output) : Recipe("smelt", output) {
	m_input = input;
}

void SmeltingRecipe::serialize(sf::Packet &packet) {
	packet << m_result << m_input;
}

void SmeltingRecipe::deserialize(sf::Packet &packet) {
	packet >> m_result >> m_input;
}

bool SmeltingRecipe::isMatching(const Inventory &inventory) const {
	return (inventory.getStack(0, 0).item().id() == m_input.item().id()
	     && inventory.getStack(0, 0).amount() >= m_input.amount());
}

