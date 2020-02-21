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
#include "BlockData.hpp"
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"
#include "ServerCommandHandler.hpp"

void ServerCommandHandler::sendBlockDataUpdate(s32 x, s32 y, s32 z, const BlockData *blockData, const Client *client) const {
	sf::Packet packet;
	packet << Network::Command::BlockDataUpdate << x << y << z
		<< blockData->meta << blockData->useAltTiles;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendBlockInvUpdate(s32 x, s32 y, s32 z, const Inventory &inventory, const Client *client) const {
	sf::Packet packet;
	packet << Network::Command::BlockInvUpdate << x << y << z << inventory;

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::sendPlayerPosUpdate(u16 clientID, const ServerPlayer &player, const Client *client) const {
	sf::Packet packet;
	packet << Network::Command::PlayerPosUpdate;
	packet << clientID;
	packet << player.x() << player.y() << player.z();

	if (!client)
		m_server.sendToAllClients(packet);
	else
		client->tcpSocket->send(packet);
}

void ServerCommandHandler::setupCallbacks() {
	m_server.setConnectionCallback([this](Client &client) {
		sf::Packet packet;
		packet << Network::Command::RegistryData;
		m_registry.serialize(packet);
		client.tcpSocket->send(packet);

		// FIXME: Duplicated below, why?
		// Here the new client is not part of m_players though
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

		// FIXME: Duplicated above, why?
		sf::Packet spawnPacket;
		spawnPacket << Network::Command::PlayerSpawn << client.id;
		spawnPacket << m_spawnPosition.x << m_spawnPosition.y << m_spawnPosition.z;
		m_server.sendToAllClients(spawnPacket);

		// m_world.sendSpawnData(client, player);
	});

	m_server.setCommandCallback(Network::Command::ChunkRequest, [this](Client &client, sf::Packet &packet) {
		s32 cx, cy, cz;
		packet >> cx >> cy >> cz;

		m_world.sendRequestedData(client, cx, cy, cz);
	});

	m_server.setCommandCallback(Network::Command::PlayerInvUpdate, [this](Client &client, sf::Packet &packet) {
		u16 clientId;
		packet >> clientId;
		if (clientId == client.id) {
			packet >> m_players.at(client.id).inventory();
		}
	});

	m_server.setCommandCallback(Network::Command::PlayerPosUpdate, [this](Client &client, sf::Packet &packet) {
		s32 x, y, z;
		u16 clientId;
		packet >> clientId;
		packet >> x >> y >> z;

		if (clientId == client.id)
			m_players.at(client.id).setPosition(x, y, z);
	});

	m_server.setCommandCallback(Network::Command::PlayerPlaceBlock, [this](Client &, sf::Packet &packet) {
		s32 x, y, z;
		u32 block;
		packet >> x >> y >> z >> block;
		m_world.setBlock(x, y, z, block);

		sf::Packet answer;
		answer << Network::Command::BlockUpdate << x << y << z << block;
		m_server.sendToAllClients(answer);
	});

	m_server.setCommandCallback(Network::Command::PlayerDigBlock, [this](Client &, sf::Packet &packet) {
		s32 x, y, z;
		packet >> x >> y >> z;
		m_world.setBlock(x, y, z, 0);

		sf::Packet answer;
		answer << Network::Command::BlockUpdate << x << y << z << u32(0);
		m_server.sendToAllClients(answer);
	});

	m_server.setCommandCallback(Network::Command::PlayerInventory, [this](Client &client, sf::Packet &packet) {
		u16 screenWidth, screenHeight;
		u8 guiScale;
		packet >> screenWidth >> screenHeight >> guiScale;

		m_scriptEngine.lua()["show_inventory"](client, screenWidth, screenHeight, guiScale);
	});

	m_server.setCommandCallback(Network::Command::BlockActivated, [this](Client &client, sf::Packet &packet) {
		s32 x, y, z;
		u16 screenWidth, screenHeight;
		u8 guiScale;
		packet >> x >> y >> z >> screenWidth >> screenHeight >> guiScale;

		u16 id = m_world.getBlock(x, y, z);
		((ServerBlock &)(m_registry.getBlock(id))).onBlockActivated({x, y, z}, m_players.at(client.id), m_world, client, screenWidth, screenHeight, guiScale);
	});

	m_server.setCommandCallback(Network::Command::BlockInvUpdate, [this](Client &, sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = m_world.getBlockData(pos.x, pos.y, pos.z);
		if (data)
			packet >> data->inventory;
		else
			DEBUG("BlockInvUpdate: No block data found at", pos.x, pos.y, pos.z);
	});

	m_server.setCommandCallback(Network::Command::BlockDataUpdate, [this](Client &, sf::Packet &packet) {
		gk::Vector3<s32> pos;
		packet >> pos.x >> pos.y >> pos.z;

		BlockData *data = m_world.getBlockData(pos.x, pos.y, pos.z);
		if (data) {
			packet >> data->meta >> data->useAltTiles;
		}
	});

	m_server.setCommandCallback(Network::Command::ChatMessage, [this](Client &, sf::Packet &packet) {
		// u16 id;
		// std::string message;
		// packet >> id >> message;

		m_server.sendToAllClients(packet);
	});
}

