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
#include "AnimationController.hpp"
#include "PositionComponent.hpp"

void AnimationController::update(entt::registry &registry) {
	registry.view<PositionComponent, AnimationComponent>().each([](auto, auto &position, auto &animation) {
		for (auto &it : animation.list) {
			if (it.type == AnimationType::Translation) {
				float dx = it.translation.dx;
				float dy = it.translation.dy;
				float dz = it.translation.dz;

				if (it.translation.cx + it.translation.dx > it.translation.max
				 || it.translation.cx + it.translation.dx < it.translation.min)
					dx = (it.translation.loop) ? -dx : 0;

				if (it.translation.cy + it.translation.dy > it.translation.max
				 || it.translation.cy + it.translation.dy < it.translation.min)
					dy = (it.translation.loop) ? -dy : 0;

				if (it.translation.cz + it.translation.dz > it.translation.max
				 || it.translation.cz + it.translation.dz < it.translation.min)
					dz = (it.translation.loop) ? -dz : 0;

				position.x += dx;
				position.y += dy;
				position.z += dz;

				it.translation.cx += dx;
				it.translation.cy += dy;
				it.translation.cz += dz;

				it.translation.dx = dx;
				it.translation.dy = dy;
				it.translation.dz = dz;
			}
			// else if (it.type == AnimationType::Rotation)
			// 	transformable.rotate(it.rotation.angle, {it.rotation.axisX, it.rotation.axisY, it.rotation.axisZ});
		}
	});
}

