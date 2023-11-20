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
#include "client/graphics/Camera.hpp"
#include "client/scene/ClientScene.hpp"
#include "client/scene/controller/AnimationController.hpp"
#include "client/scene/controller/RenderingController.hpp"

ClientScene::ClientScene() {
	m_controllers.emplace_back(new AnimationController);
	m_clientControllers.emplace_back(new RenderingController);
}

void ClientScene::update() {
	Scene::update();

	for (auto &controller : m_clientControllers)
		controller->update(m_registry);
}

void ClientScene::draw(RenderTarget &target, RenderStates states) const {
	if (!m_camera) return;

	// Subtract the camera position - see comment in ClientWorld::draw()
	Vector3d cameraPosition = m_camera->getDPosition();
	states.transform.translate((float)-cameraPosition.x, (float)-cameraPosition.y, (float)-cameraPosition.z);

	for (auto &controller : m_clientControllers)
		controller->draw(m_registry, target, states);
}
