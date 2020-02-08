/*
 * =====================================================================================
 *
 *       Filename:  ServerCommandHandler.cpp
 *
 *    Description:
 *
 *        Created:  17/03/2019 17:12:52
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Registry.hpp"
#include "ScriptEngine.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"
#include "ServerCommandHandler.hpp"

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

	m_server.setCommandCallback(Network::Command::PlayerInventory, [this](Client &client, sf::Packet &) {
		m_scriptEngine.lua()["show_inventory"](client);
	});

	m_server.setCommandCallback(Network::Command::BlockActivated, [this](Client &client, sf::Packet &packet) {
		s32 x, y, z;
		packet >> x >> y >> z;

		u16 id = m_world.getBlock(x, y, z);
		((ServerBlock &)(m_registry.getBlock(id))).onBlockActivated({x, y, z}, m_players.at(client.id), m_world, client);
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
}

