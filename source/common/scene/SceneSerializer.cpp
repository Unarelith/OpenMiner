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
#include "DrawableDef.hpp"
#include "ItemStack.hpp"
#include "Scene.hpp"
#include "SceneSerializer.hpp"

void SceneSerializer::serialize(sf::Packet &packet, const Scene &scene) const {
	m_outputArchive.setPacket(packet);

	scene.registry().snapshot().component<
		AnimationComponent,
		gk::DoubleBox,
		ItemStack,
		gk::Transformable,
		DrawableDef
	>(m_outputArchive);
}

void SceneSerializer::deserialize(sf::Packet &packet, Scene &scene) {
	m_inputArchive.setPacket(packet);

	scene.registry().loader().component<
		AnimationComponent,
		gk::DoubleBox,
		ItemStack,
		gk::Transformable,
		DrawableDef
	>(m_inputArchive);
}

void SceneSerializer::OutputArchive::operator()(entt::entity entity) {
	// gkDebug() << "Entity:" << (u32)entity;
	(*m_packet) << (u32)entity;
}

void SceneSerializer::OutputArchive::operator()(std::underlying_type_t<entt::entity> size) {
	// gkDebug() << "Size:" << size;
	(*m_packet) << size;
}

void SceneSerializer::InputArchive::operator()(entt::entity &entity) {
	u32 entityID;
	(*m_packet) >> entityID;
	entity = (entt::entity)entityID;
	// gkDebug() << "Entity:" << (u32)entity;
}

void SceneSerializer::InputArchive::operator()(std::underlying_type_t<entt::entity> &size) {
	// gkDebug() << "Size:" << size;
	(*m_packet) >> size;
}

