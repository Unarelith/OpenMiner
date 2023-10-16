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
#include <glm/gtx/quaternion.hpp>

#include "DrawableDef.hpp"
#include "DrawableComponent.hpp"
#include "InventoryCube.hpp"
#include "PositionComponent.hpp"
#include "RenderingController.hpp"
#include "RotationComponent.hpp"

#include "Registry.hpp"

void RenderingController::update(entt::registry &registry) {
	registry.view<DrawableDef>(entt::exclude<DrawableComponent>).each([&](auto entity, auto &drawableDef) {
		const InventoryCubeDef &cubeDef = drawableDef.getInventoryCubeDef();

		DrawableComponent &drawable = registry.get_or_emplace<DrawableComponent>(entity);

		InventoryCube &cube = drawable.setDrawable<InventoryCube>(cubeDef.size, true);
		cube.setOrigin(cubeDef.origin);
		cube.updateVertexBuffer(Registry::getInstance().getBlockFromStringID(cubeDef.blockID));

		registry.remove<DrawableDef>(entity);
	});
}

#ifdef OM_NOT_IMPLEMENTED_SCENE_DRAW
void RenderingController::draw(entt::registry &registry, RenderTarget &target, RenderStates states) {
	registry.view<DrawableComponent, PositionComponent, RotationComponent>().each([&](auto, auto &drawable, auto &position, auto &rotation) {
		gk::Transformable transformable;
		transformable.setPosition((float)position.x, (float)position.y, (float)position.z);
		transformable.getRotationTransform().getMatrix() = glm::toMat4(rotation.quat);

		RenderStates drawStates = states;
		drawStates.transform *= transformable.getTransform();
		drawable.draw(target, drawStates);
	});
}
#endif // OM_NOT_IMPLEMENTED_SCENE_DRAW
