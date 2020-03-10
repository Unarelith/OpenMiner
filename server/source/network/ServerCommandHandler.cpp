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
#include "BlockData.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include "ServerPlayer.hpp"
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
	const ServerPlayer &player = m_players.at(clientID);

	sf::Packet packet;
	packet << Network::Command::PlayerPosUpdate;
	packet << clientID;
	packet << player.x() << player.y() << player.z();
	packet << isTeleportation;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
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

		m_players.emplace(client.id, client);

		auto &player = m_players.at(client.id);
		player.setPosition(m_spawnPosition.x, m_spawnPosition.y, m_spawnPosition.z);

		// FIXME: Find a better way to give starting items
		m_scriptEngine.lua()["init"](player);

		sf::Packet invPacket;
		invPacket << Network::Command::PlayerInvUpdate << client.id;
		invPacket << m_players.at(client.id).inventory();
		client.tcpSocket->send(invPacket);

		// Send spawn packet to all clients for this player
		sf::Packet spawnPacket;
		spawnPacket << Network::Command::PlayerSpawn << client.id;
		spawnPacket << m_spawnPosition.x << m_spawnPosition.y << m_spawnPosition.z;
		m_server.sendToAllClients(spawnPacket);
	});

	m_server.setCommandCallback(Network::Command::ClientDisconnect, [this](ClientInfo &client, sf::Packet &) {
		auto it = m_players.find(client.id);
		if (it != m_players.end())
			m_players.erase(it);
	});

	m_server.setCommandCallback(Network::Command::ChunkRequest, [this](ClientInfo &client, sf::Packet &packet) {
		s32 cx, cy, cz;
		packet >> cx >> cy >> cz;

		getWorldForClient(client.id).sendRequestedData(client, cx, cy, cz);
	});

	m_server.setCommandCallback(Network::Command::PlayerInvUpdate, [this](ClientInfo &client, sf::Packet &packet) {
		u16 clientId;
		packet >> clientId;
		if (clientId == client.id) {
			packet >> m_players.at(client.id).inventory();
		}
	});

	m_server.setCommandCallback(Network::Command::PlayerPosUpdate, [this](ClientInfo &client, sf::Packet &packet) {
		double x, y, z;
		u16 clientId;
		packet >> clientId;
		packet >> x >> y >> z;

		if (clientId == client.id)
			m_players.at(client.id).setPosition(x, y, z);
	});

	m_server.setCommandCallback(Network::Command::PlayerPlaceBlock, [this](ClientInfo &client, sf::Packet &packet) {
		s32 x, y, z;
		u32 block;
		packet >> x >> y >> z >> block;

		ServerWorld &world = getWorldForClient(client.id);
		world.setBlock(x, y, z, block & 0xffff);
		world.setData(x, y, z, block >> 16);

		m_scriptEngine.luaCore().onEvent(LuaEventType::OnBlockPlaced, glm::ivec3{x, y, z}, m_players.at(client.id), world, client, *this);

		sf::Packet answer;
		answer << Network::Command::BlockUpdate << x << y << z << block;
		m_server.sendToAllClients(answer);
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
			DEBUG("Failed to send inventory GUI\n", error.what());
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
			DEBUG("Failed to send creative window GUI\n", error.what());
		}
	});

	m_server.setCommandCallback(Network::Command::BlockActivated, [this](ClientInfo &client, sf::Packet &packet) {
		s32 x, y, z;
		u16 screenWidth, screenHeight;
		u8 guiScale;
		packet >> x >> y >> z >> screenWidth >> screenHeight >> guiScale;

		ServerWorld &world = getWorldForClient(client.id);

		u16 id = world.getBlock(x, y, z);
		ServerBlock &block = (ServerBlock &)(m_registry.getBlock(id));
		block.onBlockActivated({x, y, z}, m_players.at(client.id), world, client, *this, screenWidth, screenHeight, guiScale);
	});

	m_server.setCommandCallback(Network::Command::BlockInvUpdate, [this](ClientInfo &client, sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = getWorldForClient(client.id).getBlockData(pos.x, pos.y, pos.z);
		if (data)
			packet >> data->inventory;
		else
			DEBUG("BlockInvUpdate: No block data found at", pos.x, pos.y, pos.z);
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
	m_players.at(clientID).setPosition(x, y, z);
}

inline ServerWorld &ServerCommandHandler::getWorldForClient(u16 clientID) {
	auto it = m_players.find(clientID);
	if (it == m_players.end())
		throw EXCEPTION("Player instance not found for client", clientID);

	return m_worldController.getWorld(it->second.dimension());
}

