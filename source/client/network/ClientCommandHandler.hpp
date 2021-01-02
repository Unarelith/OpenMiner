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
#ifndef CLIENTCOMMANDHANDLER_HPP_
#define CLIENTCOMMANDHANDLER_HPP_

#include <unordered_map>

#include <entt/entt.hpp>

#include "PlayerBox.hpp"

namespace gk {
	class Camera;
}

class Client;
class ClientPlayer;
class ClientWorld;

class ClientCommandHandler {
	public:
		ClientCommandHandler(Client &client, ClientWorld &world, ClientPlayer &player, std::unordered_map<u16, PlayerBox> &playerBoxes)
			: m_client(client), m_world(world), m_player(player), m_playerBoxes(playerBoxes) {}

		void sendPlayerInvUpdate();
		void sendPlayerPosUpdate();
		void sendPlayerRotUpdate();
		void sendPlayerDigBlock(const glm::ivec4 &selectedBlock);
		void sendPlayerPlaceBlock(s32 x, s32 y, s32 z, u32 block);
		void sendPlayerHeldItemChanged(u8 hotbarSlot, u16 itemID);
		void sendPlayerReady();
		void sendBlockActivated(const glm::ivec4 &selectedBlock);
		void sendBlockInvUpdate(Inventory &inventory);
		void sendItemActivated(const glm::ivec4 &selectedBlock);
		void sendChunkUnload(s32 chunkX, s32 chunkY, s32 chunkZ);
		void sendChatMessage(const std::string &message);
		void sendKeyPressed(u16 keyID);

		void setupCallbacks();

		Client &client() { return m_client; }

		bool isRegistryInitialized() const { return m_isRegistryInitialized; }

		void setSingleplayer(bool isSingleplayer) { m_isSingleplayer = isSingleplayer; }

		using PlayerBoxMap = std::unordered_map<u16, PlayerBox>;
		using EntityMap = std::unordered_map<entt::entity, entt::entity>;

	private:
		Client &m_client;
		ClientWorld &m_world;
		ClientPlayer &m_player;

		PlayerBoxMap &m_playerBoxes;
		EntityMap m_entityMap;

		bool m_isRegistryInitialized = false;

		bool m_isSingleplayer = false;
};

#endif // CLIENTCOMMANDHANDLER_HPP_
