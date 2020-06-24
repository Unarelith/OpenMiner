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
#include <fstream>

#include <gk/core/Debug.hpp>

#include <filesystem.hpp>

#include "Network.hpp"
#include "NetworkComponent.hpp"
#include "Registry.hpp"
#include "ServerWorld.hpp"
#include "WorldSaveBasicBackend.hpp"

namespace fs = ghc::filesystem;

void WorldSaveBasicBackend::load(const std::string &name) {
	gkInfo() << ("Loading '" + name + "'...").c_str();

	std::ifstream file("saves/" + name + ".dat", std::ofstream::binary);

	if (file.is_open()) {
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);

		char *buffer = new char[length];
		file.read(buffer, length);

		Network::Packet save;
		save.append(buffer, length);

		delete[] buffer;

		for (auto &world : m_worldList) {
			unsigned int chunkCount;
			save >> chunkCount;

			// gkInfo() << "Loading dimension" << world.dimension().id() << "| Chunk count:" << chunkCount;

			for (unsigned int i = 0 ; i < chunkCount ; ++i) {
				int cx, cy, cz;
				save >> cx >> cy >> cz;

				ServerChunk &chunk = world.createChunk(cx, cy, cz);
				for (u8 z = 0 ; z < Chunk::height ; ++z) {
					for (u8 y = 0 ; y < Chunk::depth ; ++y) {
						for (u8 x = 0 ; x < Chunk::width ; ++x) {
							bool hasMetadata;
							save >> hasMetadata;

							if (hasMetadata) {
								BlockData *blockData = chunk.addBlockData(x, y, z);
								save >> blockData->inventory >> blockData->meta >> blockData->useAltTiles;
							}

							u32 data;
							u8 light;
							save >> data >> light;

							chunk.setBlockRaw(x, y, z, data & 0xffff);
							chunk.setData(x, y, z, data >> 16);
							chunk.lightmap().setLightData(x, y, z, light);

							const Block &block = Registry::getInstance().getBlock(data & 0xffff);
							if (block.canUpdate())
								chunk.addTickingBlock(x, y, z, block);
						}
					}
				}

				chunk.setInitialized(true);
				chunk.setSent(false);
				chunk.setModified(true);
			}

			loadEntities(save, world);
		}
	}

	// gkInfo() << "Loading done.";
}

void WorldSaveBasicBackend::save(const std::string &name) {
	gkInfo() << ("Saving '" + name + "'...").c_str();

	fs::create_directory("saves");

	std::ofstream file("saves/" + name + ".dat", std::ofstream::binary | std::ofstream::trunc);

	Network::Packet save;
	for (auto &world : m_worldList) {
		Network::Packet chunks;
		unsigned int chunkCount = 0;
		for (auto &it : world.chunks()) {
			if (!it.second->isInitialized() || !it.second->hasBeenModified()) continue;

			const gk::Vector3i &chunkpos = it.first;
			const Chunk::DataArray &data = it.second->data();
			chunks << chunkpos.x << chunkpos.y << chunkpos.z;

			for (u8 z = 0 ; z < Chunk::height ; ++z) {
				for (u8 y = 0 ; y < Chunk::depth ; ++y) {
					for (u8 x = 0 ; x < Chunk::width ; ++x) {
						BlockData *blockData = it.second->getBlockData(x, y, z);
						if (blockData)
							chunks << true << blockData->inventory << blockData->meta << blockData->useAltTiles;
						else
							chunks << false;

						chunks << u32(data[z][y][x])
							<< u8(it.second->lightmap().getLightData(x, y, z));
					}
				}
			}

			++chunkCount;
		}

		// gkInfo() << "Saving dimension" << world.dimension().id() << "| Chunk count:" << chunkCount;

		save << chunkCount;
		save.append(chunks.getData(), chunks.getDataSize());

		saveEntities(save, world);
	}

	file.write((const char *)save.getData(), save.getDataSize());

	// gkInfo() << "Saving done.";
}

#include "AnimationComponent.hpp"
#include "DrawableDef.hpp"
#include "PositionComponent.hpp"
#include "RotationComponent.hpp"

void WorldSaveBasicBackend::loadEntities(sf::Packet &save, ServerWorld &world) {
	entt::registry &registry = world.scene().registry();
	registry.clear();

	u32 componentCount;
	save >> componentCount;

	// gkDebug() << "Loading" << componentCount << "components in dimension" << world.dimension().id();

	for (u32 i = 0 ; i < componentCount ; ++i) {
		Network::Command packetID;
		save >> packetID;

		entt::entity entityID;
		save >> entityID;

		// gkDebug() << "Loading packet" << Network::commandToString(packetID) << "for entity" << std::underlying_type_t<entt::entity>(entityID);

		gk::ISerializable *component = nullptr;
		auto it = m_entityMap.find(entityID);
		if (it == m_entityMap.end()) {
			if (packetID == Network::Command::EntitySpawn) {
				entt::entity entity = registry.create();
				m_entityMap.emplace(entityID, entity);
				registry.emplace<NetworkComponent>(entity, entity);
			}
			else {
				gkWarning() << "World load: Failed to add a component to nonexistent entity:" << std::underlying_type_t<entt::entity>(entityID);
			}
		}
		else {
			if (packetID == Network::Command::EntitySpawn) {
				gkWarning() << "World load: Trying to recreate an entity:" << std::underlying_type_t<entt::entity>(entityID);
			}
			else if (packetID == Network::Command::EntityPosition) {
				component = &registry.emplace<PositionComponent>(it->second);
			}
			else if (packetID == Network::Command::EntityRotation) {
				component = &registry.emplace<RotationComponent>(it->second);
			}
			else if (packetID == Network::Command::EntityAnimation) {
				component = &registry.emplace<AnimationComponent>(it->second);
			}
			else if (packetID == Network::Command::EntityDrawableDef) {
				component = &registry.emplace<DrawableDef>(it->second);
			}

			if (component)
				component->deserialize(save);
			else
				gkWarning() << "Unknown component with packet ID" << std::hex << (int)packetID;
		}
	}
}

void WorldSaveBasicBackend::saveEntities(sf::Packet &save, ServerWorld &world) {
	entt::registry &registry = world.scene().registry();

	u32 componentCount = 0;
	Network::Packet entities;
	registry.view<NetworkComponent>().each([&] (auto entity, auto &network) {
		entities << Network::Command::EntitySpawn << network.entityID;
		++componentCount;

		registry.visit(entity, [&] (const auto &component_type) {
			const auto &type = entt::resolve_type(component_type);
			const auto &component = type.func("get"_hs).invoke({}, std::ref(registry), entity);
			Network::Packet packet = type.func("serialize"_hs).invoke({}, entity, component, true).template cast<Network::Packet>();
			if (packet.getDataSize()) {
				// gkDebug() << "Serializing component" << type.prop("name"_hs).value().template cast<std::string>() << "for entity" << std::underlying_type_t<entt::entity>(entity) << "of size" << packet.getDataSize();
				entities.append(packet.getData(), packet.getDataSize());
				++componentCount;
			}
		});
	});

	save << componentCount;
	save.append(entities.getData(), entities.getDataSize());

	// gkDebug() << "Saving" << componentCount << "components in dimension" << world.dimension().id();
}

