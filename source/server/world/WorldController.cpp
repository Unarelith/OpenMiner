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
#include <filesystem>
#include <fstream>

#include "Registry.hpp"
#include "WorldController.hpp"

void WorldController::init(PlayerList &players) {
	for (const Dimension &dimension : m_registry.dimensions()) {
		m_worldList.emplace_back(players, dimension, m_clock);
		m_worldList.back().setServer(m_server);
	}
}

void WorldController::clearEntities() {
	for (auto &it : m_worldList)
		it.scene().clear();
}

void WorldController::update() {
	for (auto &it : m_worldList)
		it.update();
}

void WorldController::load(const std::string &name) {
	gkDebug() << ("Loading '" + name + "'...").c_str();

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

			gkDebug() << "Loading dimension" << world.dimension().id() << "| Chunk count:" << chunkCount;

			for (unsigned int i = 0 ; i < chunkCount ; ++i) {
				int cx, cy, cz;
				save >> cx >> cy >> cz;

				ServerChunk &chunk = world.createChunk(cx, cy, cz);
				for (u8 z = 0 ; z < Chunk::height ; ++z) {
					for (u8 y = 0 ; y < Chunk::depth ; ++y) {
						for (u8 x = 0 ; x < Chunk::width ; ++x) {
							u32 data;
							u8 light;
							save >> data >> light;

							chunk.setBlockRaw(x, y, z, data & 0xffff);
							chunk.setData(x, y, z, data >> 16);
							chunk.lightmap().setLightData(x, y, z, light);
						}
					}
				}

				chunk.setInitialized(true);
				chunk.setSent(false);
			}
		}
	}

	gkDebug() << "Loading done.";
}

void WorldController::save(const std::string &name) {
	gkDebug() << ("Saving '" + name + "'...").c_str();

	std::filesystem::create_directory("saves");

	std::ofstream file("saves/" + name + ".dat", std::ofstream::binary | std::ofstream::trunc);

	Network::Packet save;
	for (auto &world : m_worldList) {
		Network::Packet chunks;
		unsigned int chunkCount = 0;
		for (auto &chunk : world.chunks()) {
			if (!chunk.second->isInitialized()) continue;

			const gk::Vector3i &chunkpos = chunk.first;
			const Chunk::DataArray &data = chunk.second->data();
			chunks << chunkpos.x << chunkpos.y << chunkpos.z;

			for (u8 z = 0 ; z < Chunk::height ; ++z) {
				for (u8 y = 0 ; y < Chunk::depth ; ++y) {
					for (u8 x = 0 ; x < Chunk::width ; ++x) {
						chunks << u32(data[z][y][x])
							<< u8(chunk.second->lightmap().getLightData(x, y, z));
					}
				}
			}

			++chunkCount;
		}

		gkDebug() << "Saving dimension" << world.dimension().id() << "| Chunk count:" << chunkCount;

		save << chunkCount;
		save.append(chunks.getData(), chunks.getDataSize());
	}

	file.write((const char *)save.getData(), save.getDataSize());

	gkDebug() << "Saving done.";
}
