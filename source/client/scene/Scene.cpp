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
#include "ClientPlayer.hpp"
#include "Scene.hpp"

struct RotationAnimation {
	float axisX;
	float axisY;
	float axisZ;

	float angle;
};

#include "InventoryCube.hpp"
#include "ItemStack.hpp"
#include "Registry.hpp"

Scene::Scene(ClientPlayer &player) : m_player(player) {
}

void Scene::init() {
	auto testEntity = m_registry.create();

	InventoryCube &cube = m_registry.assign<InventoryCube>(testEntity, 0.25f, true);
	cube.setOrigin(cube.size() / 2.f, cube.size() / 2.f, cube.size() / 2.f);
	cube.setPosition(14 + cube.getOrigin().x, 13 + cube.getOrigin().y, 16 + cube.getOrigin().z);
	cube.updateVertexBuffer(Registry::getInstance().getBlockFromStringID("default:cobblestone"));

	m_registry.assign<RotationAnimation>(testEntity, 0.f, 0.f, 1.f, 1.f);
	m_registry.assign<gk::DoubleBox>(testEntity, 0., 0., 0., cube.size(), cube.size(), cube.size());
	m_registry.assign<ItemStack>(testEntity, "default:stick", 1);

	m_isInitialized = true;
}

void Scene::update() {
	if (!m_isInitialized) init();

	m_registry.view<InventoryCube, RotationAnimation>().each([](auto, auto &cube, auto &rotation) {
		cube.rotate(rotation.angle, {rotation.axisX, rotation.axisY, rotation.axisZ});
	});

	m_registry.view<InventoryCube, gk::DoubleBox, ItemStack>().each([this](auto entity, auto &cube, auto &box, auto &itemStack) {
		gk::DoubleBox hitbox = box + cube.getPosition();
		gk::DoubleBox playerHitbox = m_player.hitbox() + gk::Vector3d{m_player.x(), m_player.y(), m_player.z()};
		if (hitbox.intersects(playerHitbox)) {
			m_player.inventory().addStack(itemStack.item().stringID(), itemStack.amount());
			m_registry.destroy(entity);
		}
	});
}

void Scene::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (!m_camera) return;

	// Subtract the camera position - see comment in ClientWorld::draw()
	gk::Vector3d cameraPosition = m_camera->getDPosition();
	states.transform.translate(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z);

	m_registry.view<InventoryCube>().each([&](auto, auto &cube) {
		target.draw(cube, states);
	});
}

