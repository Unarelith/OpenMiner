/*
 * =====================================================================================
 *
 *       Filename:  ServerApplication.cpp
 *
 *    Description:
 *
 *        Created:  09/01/2019 19:41:15
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ServerApplication.hpp"
#include "ServerBlock.hpp"

using namespace std::literals::string_literals;

ServerApplication::ServerApplication(int argc, char **argv) : gk::CoreApplication(argc, argv) {
	m_loadSDL = false;
}

void ServerApplication::init() {
	m_argumentParser.addArgument("port", {"-p", "--port", true});

	gk::CoreApplication::init();

	if (m_argumentParser.getArgument("port").isFound)
		m_port = std::stoi(m_argumentParser.getArgument("port").parameter);

	Registry::setInstance(m_registry);

	m_scriptEngine.init();
	m_luaCore.setPlayer(m_player);
	m_luaCore.setWorld(m_world);

	try {
		m_scriptEngine.lua()["openminer"] = &m_luaCore;
		m_scriptEngine.lua().script("init()");
	}
	catch (const sol::error &e) {
		std::cerr << e.what() << std::endl;
	}

	m_server.init(m_port);
	m_server.setRunning(true);

	setupServerCallbacks();
}

void ServerApplication::setupServerCallbacks() {
	m_server.setConnectionCallback([this](Client &client) {
		sf::Packet packet;
		packet << Network::Command::RegistryData;
		m_registry.serialize(packet);
		client.tcpSocket->send(packet);

		sf::Packet invPacket;
		invPacket << Network::Command::PlayerInvUpdate;
		m_player.serialize(invPacket);
		client.tcpSocket->send(invPacket);

		m_world.sendWorldData(client);
	});

	m_server.setCommandCallback(Network::Command::PlayerInvUpdate, [this](Client &, sf::Packet &packet) {
		m_player.deserialize(packet);
	});

	m_server.setCommandCallback(Network::Command::ChunkRequest, [this](Client &client, sf::Packet &packet) {
		s32 cx, cy, cz;
		packet >> cx >> cy >> cz;

		m_world.sendRequestedData(client, cx, cy, cz);
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

	m_server.setCommandCallback(Network::Command::BlockActivated, [this](Client &client, sf::Packet &packet) {
		s32 x, y, z;
		packet >> x >> y >> z;

		u16 id = m_world.getBlock(x, y, z);
		((ServerBlock &)(m_registry.getBlock(id))).onBlockActivated({x, y, z}, m_player, m_world, client);
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
			packet >> data->data;
		}
	});
}

void ServerApplication::mainLoop() {
	while (m_server.isRunning()) {
		m_server.handleGameEvents();

		m_server.handleKeyState();

		m_clock.updateGame([&] {
			m_world.update(m_server, m_player);
		});

		m_clock.waitForNextFrame();
	}
}

