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
#ifndef CLIENTCOMMANDHANDLER_HPP_
#define CLIENTCOMMANDHANDLER_HPP_

#include <unordered_map>

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
		void sendPlayerDigBlock(const glm::ivec4 &selectedBlock);
		void sendPlayerPlaceBlock(s32 x, s32 y, s32 z, u32 block);
		void sendPlayerInventoryRequest();
		void sendBlockActivated(const glm::ivec4 &selectedBlock);
		void sendBlockInvUpdate(Inventory &inventory);
		void sendChunkRequest(s32 chunkX, s32 chunkY, s32 chunkZ);
		void sendChatMessage(const std::string &message);

		void setupCallbacks();

		bool isRegistryInitialized() const { return m_isRegistryInitialized; }

	private:
		Client &m_client;
		ClientWorld &m_world;
		ClientPlayer &m_player;

		std::unordered_map<u16, PlayerBox> &m_playerBoxes;

		bool m_isRegistryInitialized = false;
};

#endif // CLIENTCOMMANDHANDLER_HPP_
