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
#include <gk/core/Debug.hpp>

#include "AnimationComponent.hpp"
#include "DrawableDef.hpp"
#include "LuaEntityLoader.hpp"
#include "LuaMod.hpp"
#include "NetworkComponent.hpp"
#include "PositionComponent.hpp"
#include "Registry.hpp"
#include "RotationComponent.hpp"

void LuaEntityLoader::loadEntity(const sol::table &table) const {
	std::string stringID = m_mod.id() + ":" + table["id"].get<std::string>();

	entt::registry &registry = Registry::getInstance().entityRegistry();
	entt::entity entity = Registry::getInstance().registerEntity(stringID);

	// TODO: Create a struct to store these functions
	sol::unsafe_function onSpawn = table["on_spawn"];
	sol::unsafe_function onCollision = table["on_collision"];

	sol::optional<sol::table> properties = table["properties"];
	if (properties != sol::nullopt) {
		sol::optional<sol::table> visual = properties.value()["visual"];
		if (visual != sol::nullopt) {
			std::string type = visual.value()["type"];
			if (type == "inventorycube") {
				auto &drawable = registry.emplace<DrawableDef>(entity);
				auto &cube = drawable.addInventoryCube();

				cube.size = visual.value()["size"].get_or(1.0f);

				sol::optional<sol::table> origin = visual.value()["origin"];
				if (origin != sol::nullopt) {
					cube.origin = {origin.value()[1], origin.value()[2], origin.value()[3]};
				}
			}
			else
				gkError() << "For entity '" + stringID + "': Visual type '" + type + "' unknown";
		}

		bool isRotatable = properties.value()["is_rotatable"].get_or(false);
		if (isRotatable)
			registry.emplace<RotationComponent>(entity);

		sol::optional<sol::table> animation = properties.value()["animation"];
		if (animation != sol::nullopt) {
			auto &animationComponent = registry.emplace<AnimationComponent>(entity);
			for (auto &it : animation.value()) {
				sol::table anim = it.second.as<sol::table>();
				std::string type = anim["type"];
				if (type == "rotation") {
					sol::table axis = anim["axis"];
					animationComponent.addRotation(axis[1], axis[2], axis[3], anim["angle"]);
				}
				else if (type == "translation") {
					sol::table delta = anim["delta"];
					animationComponent.addTranslation(delta[1], delta[2], delta[3], anim["min"], anim["max"], anim["loop"].get_or(true));
				}
				else
					gkError() << "For entity '" + stringID + "': Animation type '" + type + "' unknown";
			}
		}

		sol::optional<sol::table> hitboxTable = properties.value()["hitbox"];
		if (hitboxTable != sol::nullopt) {
			registry.emplace<gk::FloatBox>(entity,
				hitboxTable.value()[1], hitboxTable.value()[2], hitboxTable.value()[3],
				hitboxTable.value()[4], hitboxTable.value()[5], hitboxTable.value()[6]
			);
		}
	}
}

#include "WorldController.hpp"

void LuaEntityLoader::spawnEntity(const std::string &entityID, const sol::table &table) const {
	gk::Vector3f pos;
	u16 dim;

	sol::optional<sol::table> position = table["position"];
	sol::optional<u16> dimension = table["dimension"];
	if (position != sol::nullopt && dimension != sol::nullopt) {
		pos = {position.value()[1], position.value()[2], position.value()[3]};
		dim = dimension.value();
	}
	else {
		gkError() << "In mod '" + m_mod.id() + "': Cannot spawn entity '" + entityID + "' without a position and a dimension";
		return;
	}

	ItemStack stack;
	sol::optional<sol::table> itemStack = table["itemstack"];
	if (itemStack != sol::nullopt) {
		stack.setItem(itemStack.value()[1]);
		stack.setAmount(itemStack.value()[2].get_or(1));
	}

	// Create entity copy here
	entt::registry &registry = Registry::getInstance().entityRegistry();
	entt::entity entityModel = Registry::getInstance().getEntityFromStringID(entityID);
	if (entityModel != entt::null) {
		ServerWorld &world = m_worldController.getWorld(dim);
		entt::registry &sceneRegistry = world.scene().registry();
		entt::entity entity = sceneRegistry.create();

		sceneRegistry.emplace<PositionComponent>(entity, pos.x, pos.y, pos.z, dim);
		sceneRegistry.emplace<NetworkComponent>(entity, entity);

		if (registry.has<RotationComponent>(entityModel))
			sceneRegistry.emplace<RotationComponent>(entity);

		auto *drawable = registry.try_get<DrawableDef>(entityModel);
		if (drawable) {
			auto &drawableDef = sceneRegistry.emplace<DrawableDef>(entity, *drawable);
			if (stack.amount()) {
				auto &cube = drawableDef.getInventoryCubeDef();
				cube.blockID = stack.item().stringID();
			}
		}

		auto *animation = registry.try_get<AnimationComponent>(entityModel);
		if (animation)
			sceneRegistry.emplace<AnimationComponent>(entity, *animation);

		auto *hitbox = registry.try_get<gk::FloatBox>(entityModel);
		if (hitbox)
			sceneRegistry.emplace<gk::FloatBox>(entity, *hitbox);

		if (stack.amount())
			sceneRegistry.emplace<ItemStack>(entity, stack);
	}
	else
		gkError() << "In mod '" + m_mod.id() + "': Cannot find entity with id '" + entityID + "'";
}

