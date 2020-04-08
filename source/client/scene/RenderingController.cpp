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
#include "DrawableDef.hpp"
#include "DrawableComponent.hpp"
#include "InventoryCube.hpp"
#include "RenderingController.hpp"

#include <gk/core/Debug.hpp>
#include <gk/core/GameClock.hpp>

#include "Registry.hpp"

void RenderingController::draw(entt::registry &registry, gk::RenderTarget &target, gk::RenderStates states) {
	registry.view<DrawableDef>().each([&](auto entity, auto &drawableDef) {
		const InventoryCubeDef &cubeDef = drawableDef.getInventoryCubeDef();

		DrawableComponent &drawable = (!registry.has<DrawableComponent>())
			? registry.assign<DrawableComponent>(entity)
			: registry.get<DrawableComponent>(entity);

		InventoryCube &cube = drawable.setDrawable<InventoryCube>(cubeDef.size, true);
		cube.setOrigin(cubeDef.origin);
		cube.updateVertexBuffer(Registry::getInstance().getBlockFromStringID(cubeDef.blockID));

		registry.remove<DrawableDef>(entity);
	});

	registry.view<DrawableComponent, gk::Transformable>().each([&](auto, auto &drawable, auto &transformable) {
		gk::RenderStates drawStates = states;
		drawStates.transform *= transformable.getTransform();
		drawable.draw(target, drawStates);
	});
}

