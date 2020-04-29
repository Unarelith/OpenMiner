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
#include "DrawableComponent.hpp"
#include "DrawableDef.hpp"
#include "ItemDropFactory.hpp"
#include "ItemStack.hpp"
#include "NetworkComponent.hpp"
#include "PositionComponent.hpp"
#include "Registry.hpp"
#include "RotationComponent.hpp"

void ItemDropFactory::create(entt::registry &registry, double x, double y, double z, u16 dimension, const std::string &itemID, u16 amount) {
	auto entity = registry.create();
	registry.emplace<PositionComponent>(entity, x, y, z, dimension);
	registry.emplace<RotationComponent>(entity);
	registry.emplace<NetworkComponent>(entity, entity);

	auto &drawableDef = registry.emplace<DrawableDef>(entity);
	auto &cube = drawableDef.addInventoryCube();
	cube.size = 0.25f;
	cube.origin = gk::Vector3f{cube.size / 2.f, cube.size / 2.f, cube.size / 2.f};
	cube.blockID = itemID;

	auto &animationComponent = registry.emplace<AnimationComponent>(entity);
	animationComponent.addRotation(0.f, 0.f, 1.f, 0.5f);
	animationComponent.addTranslation(0.f, 0.f, -0.0005f, -0.2f, 0.f, true);

	registry.emplace<gk::DoubleBox>(entity, 0., 0., 0., cube.size, cube.size, cube.size);
	registry.emplace<ItemStack>(entity, itemID, amount);
}

