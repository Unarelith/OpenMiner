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
#include "AnimationComponent.hpp"
#include "BlockData.hpp"
#include "DrawableDef.hpp"
#include "NetworkComponent.hpp"
#include "PlayerList.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include "ServerCommandHandler.hpp"
#include "WorldController.hpp"

void ServerCommandHandler::sendBlockDataUpdate(s32 x, s32 y, s32 z, const BlockData *blockData, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::BlockDataUpdate << x << y << z
		<< blockData->meta << blockData->useAltTiles;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendBlockInvUpdate(s32 x, s32 y, s32 z, const Inventory &inventory, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::BlockInvUpdate << x << y << z << inventory;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendPlayerPosUpdate(u16 clientID, bool isTeleportation, const ClientInfo *client) const {
	const ServerPlayer *player = m_players.getPlayer(clientID);
	if (player) {
		sf::Packet packet;
		packet << Network::Command::PlayerPosUpdate;
		packet << clientID;
		packet << player->x() << player->y() << player->z();
		packet << isTeleportation;

		if (!client)
			m_server.sendToAllClients(packet);
		else
			client->tcpSocket->send(packet);
	}
	else
		gkError() << ("Failed to send pos update for player " + std::to_string(clientID) + ": Player not found").c_str();
}

void ServerCommandHandler::sendPlayerChangeDimension(u16 clientID, s32 x, s32 y, s32 z, u16 dimension, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::PlayerChangeDimension;
	packet << clientID << x << y << z << dimension;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendChatMessage(u16 clientID, const std::string &message, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::ChatMessage << clientID << message;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendEntitySpawn(u32 entityID, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::EntitySpawn << entityID;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendEntityDespawn(u32 entityID, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::EntityDespawn << entityID;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendEntityPosition(u32 entityID, double x, double y, double z, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::EntityPosition << entityID << x << y << z;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendEntityRotation(u32 entityID, float w, float x, float y, float z, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::EntityRotation << entityID << w << x << y << z;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendEntityAnimation(u32 entityID, const AnimationComponent &animation, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::EntityAnimation << entityID << animation;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendEntityDrawableDef(u32 entityID, DrawableDef &drawableDef, const ClientInfo *client) const {
	sf::Packet packet;
	packet << Network::Command::EntityDrawableDef << entityID << drawableDef;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::setupCallbacks() {
	m_server.setConnectionCallback([this](ClientInfo &client) {
		sf::Packet packet;
		packet << Network::Command::RegistryData;
		m_registry.serialize(packet);
		client.tcpSocket->send(packet);

		// Send already connected players to the new client
		for (auto &it : m_players) {
			sf::Packet spawnPacket;
			spawnPacket << Network::Command::PlayerSpawn << it.first;
			spawnPacket << it.second.x() << it.second.y() << it.second.z();
			client.tcpSocket->send(spawnPacket);
		}

		auto &player = m_players.addPlayer(client);
		player.setPosition(m_spawnPosition.x, m_spawnPosition.y, m_spawnPosition.z);

		// FIXME: Find a better way to give starting items
		m_scriptEngine.lua()["init"](player);

		sf::Packet invPacket;
		invPacket << Network::Command::PlayerInvUpdate << client.id;
		invPacket << player.inventory();
		client.tcpSocket->send(invPacket);

		// Send spawn packet to all clients for this player
		sf::Packet spawnPacket;
		spawnPacket << Network::Command::PlayerSpawn << client.id;
		spawnPacket << m_spawnPosition.x << m_spawnPosition.y << m_spawnPosition.z;
		m_server.sendToAllClients(spawnPacket);

		// Send entities to the client
		m_worldController.getWorld(player.dimension()).scene().sendEntities(client);
	});

	m_server.setCommandCallback(Network::Command::ClientDisconnect, [this](ClientInfo &client, sf::Packet &) {
		m_players.removePlayer(client.id);
	});

	m_server.setCommandCallback(Network::Command::ChunkRequest, [this](ClientInfo &client, sf::Packet &packet) {
		s32 cx, cy, cz;
		packet >> cx >> cy >> cz;

		getWorldForClient(client.id).sendRequestedData(client, cx, cy, cz);
	});

	m_server.setCommandCallback(Network::Command::PlayerInvUpdate, [this](ClientInfo &client, sf::Packet &packet) {
		u16 clientId;
		packet >> clientId;

		ServerPlayer *player = m_players.getPlayer(clientId);
		if (player) {
			if (clientId == client.id) {
				packet >> player->inventory();
			}
		}
		else
			gkError() << ("Failed to update inventory of player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::PlayerPosUpdate, [this](ClientInfo &client, sf::Packet &packet) {
		double x, y, z;
		u16 clientId;
		packet >> clientId;
		packet >> x >> y >> z;

		ServerPlayer *player = m_players.getPlayer(clientId);
		if (player) {
			if (clientId == client.id)
				player->setPosition(x, y, z);
		}
		else
			gkError() << ("Failed to update position of player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::PlayerPlaceBlock, [this](ClientInfo &client, sf::Packet &packet) {
		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			s32 x, y, z;
			u32 block;
			packet >> x >> y >> z >> block;

			ServerWorld &world = getWorldForClient(client.id);
			world.setData(x, y, z, block >> 16);
			world.setBlock(x, y, z, block & 0xffff);

			m_scriptEngine.luaCore().onEvent(LuaEventType::OnBlockPlaced, glm::ivec3{x, y, z}, *player, world, client, *this);

			sf::Packet answer;
			answer << Network::Command::BlockUpdate << x << y << z << block;
			m_server.sendToAllClients(answer);
		}
		else
			gkError() << ("Failed to place block using player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::PlayerDigBlock, [this](ClientInfo &client, sf::Packet &packet) {
		s32 x, y, z;
		packet >> x >> y >> z;
		getWorldForClient(client.id).setBlock(x, y, z, 0);

		sf::Packet answer;
		answer << Network::Command::BlockUpdate << x << y << z << u32(0);
		m_server.sendToAllClients(answer);
	});

	m_server.setCommandCallback(Network::Command::PlayerInventory, [this](ClientInfo &client, sf::Packet &packet) {
		u16 screenWidth, screenHeight;
		u8 guiScale;
		packet >> screenWidth >> screenHeight >> guiScale;

		sol::unsafe_function func = m_scriptEngine.lua()["show_inventory"];

		try {
			func(client, screenWidth, screenHeight, guiScale);
		}
		catch (const sol::error &error) {
			gkError() << "Failed to send inventory GUI: " << error.what();
		}
	});

	m_server.setCommandCallback(Network::Command::PlayerCreativeWindow, [this](ClientInfo &client, sf::Packet &packet) {
		u16 screenWidth, screenHeight;
		u8 guiScale;
		packet >> screenWidth >> screenHeight >> guiScale;

		sol::unsafe_function func = m_scriptEngine.lua()["show_creative_window"];

		try {
			func(client, screenWidth, screenHeight, guiScale);
		}
		catch (const sol::error &error) {
			gkError() << "Failed to send creative window GUI: " << error.what();
		}
	});

	m_server.setCommandCallback(Network::Command::BlockActivated, [this](ClientInfo &client, sf::Packet &packet) {
		ServerPlayer *player = m_players.getPlayer(client.id);
		if (player) {
			s32 x, y, z;
			u16 screenWidth, screenHeight;
			u8 guiScale;
			packet >> x >> y >> z >> screenWidth >> screenHeight >> guiScale;

			ServerWorld &world = getWorldForClient(client.id);

			u16 id = world.getBlock(x, y, z);
			ServerBlock &block = (ServerBlock &)(m_registry.getBlock(id));
			bool hasBeenActivated = block.onBlockActivated({x, y, z}, *player, world, client, *this, screenWidth, screenHeight, guiScale);

			if (hasBeenActivated)
				m_scriptEngine.luaCore().onEvent(LuaEventType::OnBlockActivated, glm::ivec3{x, y, z}, block, *player, world, client, *this);
		}
		else
			gkError() << ("Failed to activate block using player " + std::to_string(client.id) + ": Player not found").c_str();
	});

	m_server.setCommandCallback(Network::Command::BlockInvUpdate, [this](ClientInfo &client, sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = getWorldForClient(client.id).getBlockData(pos.x, pos.y, pos.z);
		if (data)
			packet >> data->inventory;
		else
			gkError() << "BlockInvUpdate: No block data found at" << pos.x << pos.y << pos.z;
	});

	m_server.setCommandCallback(Network::Command::BlockDataUpdate, [this](ClientInfo &client, sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = getWorldForClient(client.id).getBlockData(pos.x, pos.y, pos.z);
		if (data) {
			packet >> data->meta >> data->useAltTiles;
		}
	});

	m_server.setCommandCallback(Network::Command::ChatMessage, [this](ClientInfo &client, sf::Packet &packet) {
		u16 clientID;
		std::string message;
		packet >> clientID >> message;

		if (message[0] != '/' || (message.length() > 1 && message[1] == '/')) {
			if (message[0] == '/' && message.length() > 1 && message[1] == '/')
				sendChatMessage(clientID, message.substr(1));
			else
				sendChatMessage(clientID, message);
		}
		else {
			m_chatCommandHandler.parseCommand(message.substr(1), client);
		}
	});
}

void ServerCommandHandler::setPlayerPosition(u16 clientID, s32 x, s32 y, s32 z) {
	ServerPlayer *player = m_players.getPlayer(clientID);
	if (player)
		player->setPosition(x, y, z);
	else
		gkError() << ("Failed to set position for player " + std::to_string(clientID) + ": Player not found").c_str();
}

inline ServerWorld &ServerCommandHandler::getWorldForClient(u16 clientID) {
	ServerPlayer *player = m_players.getPlayer(clientID);
	if (!player)
		throw EXCEPTION("Player instance not found for client", clientID);

	return m_worldController.getWorld(player->dimension());
}

