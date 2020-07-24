/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include "Block.hpp"
#include "NetworkUtils.hpp"
#include "Player.hpp"
#include "World.hpp"

Block::Block(u32 id, const std::string &stringID) {
	m_id = id;
	m_stringID = stringID;

	BlockState &defaultState = addState();
	defaultState.itemDrop(stringID);
	defaultState.itemDropAmount(1);
}

const TilesDef &Block::tiles(u16 stateID) const {
	if (stateID >= m_states.size()) {
		gkError() << ("Failed to get tiles for block '" + m_stringID + "': State").c_str() << stateID << "doesn't exist";
		return m_states[0].tiles();
	}

	return m_states[stateID].tiles();
}

void Block::serialize(sf::Packet &packet) const {
	packet << u32(m_id) << m_stringID << m_canUpdate << m_canBeActivated
		<< m_isRotatable << m_groups << m_states << m_param << m_customParamBits;
}

void Block::deserialize(sf::Packet &packet) {
	u32 id;
	packet >> id >> m_stringID >> m_canUpdate >> m_canBeActivated
		>> m_isRotatable >> m_groups >> m_states >> m_param >> m_customParamBits;
	m_id = id;

	for (auto &it : m_states) {
		it.setBlock(this);
		it.setDefaultState(&m_states.at(0));
	}

}

BlockState &Block::addState() {
	if (!m_states.empty())
		m_states.emplace_back(m_states.size(), this, &getState(0));
	else
		m_states.emplace_back(m_states.size(), this, nullptr);
	return m_states.back();
}

BlockState &Block::getState(u16 id) {
	if (id >= m_states.size()) {
		gkError() << "Failed to get state" << id << "in block" << m_stringID;
		return m_states.at(0);
	}

	return m_states.at(id);
}

const BlockState &Block::getState(u16 id) const {
	if (id >= m_states.size()) {
		gkError() << "Failed to get state" << id << "in block" << m_stringID;
		return m_states.at(0);
	}

	return m_states.at(id);
}

// Please update 'docs/lua-api-cpp.md' if you change this
void Block::initUsertype(sol::state &lua) {
	lua.new_usertype<Block>("Block",
		"id", &Block::id,
		"string_id", &Block::stringID,
		"mod_name", &Block::modName,
		"param", (const BlockParam &(Block::*)() const)&Block::param,
		"get_state", (const BlockState &(Block::*)(u16) const)&Block::getState
	);
}

