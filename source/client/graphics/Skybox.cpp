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
#include <gk/gl/GLCheck.hpp>
#include <gk/graphics/Color.hpp>

#include <gk/core/GameClock.hpp>

#include "Skybox.hpp"
#include "Vertex.hpp"

Skybox::Skybox(gk::Camera &camera) : m_camera(camera) {
	setPosition(150, -10, 0);
}

void Skybox::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	// Subtract the camera position - see comment in ClientWorld::draw()
	gk::Vector3d cameraPosition = m_camera.getDPosition();
	states.transform.rotate(fmod(gk::GameClock::getInstance().getTicks(true) / 1000.f, 360), {0, 1, 0});
	states.transform.translate(getPosition().x - cameraPosition.x, getPosition().y - cameraPosition.y, getPosition().z - cameraPosition.z);

	target.draw(m_sun, states);
}

