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
#include "InventoryCube.hpp"
#include "ItemDropFactory.hpp"
#include "ItemStack.hpp"
#include "Registry.hpp"

void ItemDropFactory::create(entt::DefaultRegistry &registry, double x, double y, double z, const std::string &itemID, u16 amount) {
	auto entity = registry.create();

	InventoryCube &cube = registry.assign<InventoryCube>(entity, 0.25f, true);
	cube.setOrigin(cube.size() / 2.f, cube.size() / 2.f, cube.size() / 2.f);
	cube.setPosition(x + 0.5, y + 0.5, z + 0.5);
	cube.updateVertexBuffer(Registry::getInstance().getBlockFromStringID(itemID));

	auto &animationComponent = registry.assign<AnimationComponent>(entity);
	animationComponent.addRotation(0.f, 0.f, 1.f, 0.5f);
	animationComponent.addTranslation(0.f, 0.f, -0.0005f, -0.2f, 0.f, true);

	registry.assign<gk::DoubleBox>(entity, 0., 0., 0., cube.size(), cube.size(), cube.size());
	registry.assign<ItemStack>(entity, itemID, amount);
}

