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
#ifndef SERVERBLOCK_HPP_
#define SERVERBLOCK_HPP_

#include "Block.hpp"

class ClientInfo;
class ServerChunk;
class ServerCommandHandler;
class ServerPlayer;
class ServerWorld;

class ServerBlock : public Block {
	public:
		ServerBlock(u32 id, const TilesDef &tiles, const std::string &name, const std::string &label)
			: Block(id, tiles, name, label) {}

		void onTick(const glm::ivec3 &pos, ServerChunk &chunk, ServerWorld &world, ServerCommandHandler &server) const;
		bool onBlockActivated(const glm::ivec3 &pos, ServerPlayer &player, ServerWorld &world, ClientInfo &client, ServerCommandHandler &server, u16 screenWidth, u16 screenHeight, u8 guiScale) const;
		void onBlockPlaced(const glm::ivec3 &pos, ServerWorld &world) const;
		void onBlockDestroyed(const glm::ivec3 &pos, ServerWorld &world) const;

		bool canUpdate() const { return m_onTick.valid(); }

		void setOnBlockActivated(const sol::protected_function &function) { m_onBlockActivated = function; m_canBeActivated = m_onBlockActivated.valid(); }
		void setOnTick(const sol::protected_function &function) { m_onTick = function; m_canUpdate = m_onTick.valid(); }
		void setOnBlockPlaced(const sol::protected_function &function) { m_onBlockPlaced = function; }
		void setOnBlockDestroyed(const sol::protected_function &function) { m_onBlockDestroyed = function; }

		bool isTickingRandomly() const { return m_isTickingRandomly; }
		void setTickRandomly(bool isTickingRandomly) { m_isTickingRandomly = isTickingRandomly; }

		float tickProbability() const { return m_tickProbability; }
		void setTickProbability(float tickProbability) { m_tickProbability = tickProbability; }

		static void initUsertype(sol::state &lua);

	private:
		sol::unsafe_function m_onBlockActivated;
		sol::unsafe_function m_onTick;
		sol::unsafe_function m_onBlockPlaced;
		sol::unsafe_function m_onBlockDestroyed;

		mutable bool m_onTickEnabled = true;

		bool m_isTickingRandomly = false;
		float m_tickProbability = 0.f;
};

#endif // SERVERBLOCK_HPP_
