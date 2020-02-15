/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef SERVERBLOCK_HPP_
#define SERVERBLOCK_HPP_

#include "Block.hpp"

class Client;
class ServerCommandHandler;
class ServerPlayer;

class ServerBlock : public Block {
	public:
		ServerBlock(u32 id, const TilesDef &tiles, const std::string &name, const std::string &label)
			: Block(id, tiles, name, label) {}

		void onTick(const glm::ivec3 &, std::unordered_map<u16, ServerPlayer> &, Chunk &, World &, ServerCommandHandler &) const;
		bool onBlockActivated(const glm::ivec3 &pos, Player &player, World &world, Client &client, u16 screenWidth, u16 screenHeight, u8 guiScale) const;
		void onBlockPlaced(const glm::ivec3 &pos, World &world) const;

		bool canUpdate() const { return m_onTick.valid(); }

		void setOnBlockActivated(const sol::function &function) { m_onBlockActivated = function; m_canBeActivated = m_onBlockActivated.valid(); }
		void setOnTick(const sol::function &function) { m_onTick = function; m_canUpdate = m_onTick.valid(); }
		void setOnBlockPlaced(const sol::function &function) { m_onBlockPlaced = function; }

	private:
		sol::unsafe_function m_onBlockActivated;
		sol::unsafe_function m_onTick;
		sol::unsafe_function m_onBlockPlaced;
		mutable bool m_onTickEnabled = true;
};

#endif // SERVERBLOCK_HPP_
