/*
 * =====================================================================================
 *
 *       Filename:  ClientCommandHandler.hpp
 *
 *    Description:
 *
 *        Created:  17/03/2019 17:01:13
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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
		ClientCommandHandler(Client &client, ClientWorld &world, ClientPlayer &player, gk::Camera &camera, std::unordered_map<u16, PlayerBox> &playerBoxes)
			: m_client(client), m_world(world), m_player(player), m_camera(camera), m_playerBoxes(playerBoxes) {}

		void sendPlayerInvUpdate();
		void sendPlayerDigBlock(const glm::vec4 &selectedBlock);
		void sendPlayerPlaceBlock(s32 x, s32 y, s32 z, u32 block);
		void sendBlockActivated(const glm::vec4 &selectedBlock);
		void sendBlockInvUpdate(Inventory &inventory);
		void sendChunkRequest(s32 chunkX, s32 chunkY, s32 chunkZ);

		void setupCallbacks();

		bool isRegistryInitialized() const { return m_isRegistryInitialized; }

	private:
		Client &m_client;
		ClientWorld &m_world;
		ClientPlayer &m_player;
		gk::Camera &m_camera;

		std::unordered_map<u16, PlayerBox> &m_playerBoxes;

		bool m_isRegistryInitialized = false;
};

#endif // CLIENTCOMMANDHANDLER_HPP_
