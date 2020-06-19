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
#include <gk/core/ISerializable.hpp>

#include <entt/entt.hpp>

#include "Network.hpp"
#include "NetworkUtils.hpp"
#include "Scene.hpp"

static bool areComponentsRegistered = false;

Scene::Scene() {
	if (!areComponentsRegistered) {
		registerComponents();
		areComponentsRegistered = true;
	}
}

void Scene::update() {
	for (auto &controller : m_controllers)
		controller->update(m_registry);
}

// Opaque entity stamper: https://github.com/skypjack/entt/issues/481#issuecomment-627614706
entt::entity Scene::createEntityFromModel(entt::registry &modelRegistry, entt::entity modelEntity) {
	if (!modelRegistry.valid(modelEntity)) {
		gkError() << "Failed to create entity from model: Model entity with ID" << (u32)modelEntity << "doesn't exist";
		return entt::null;
	}

	auto other = m_registry.create();
	modelRegistry.visit(modelEntity, [&](const auto &component) {
		const auto type = entt::resolve_type(component);
		const auto any = type.func("get"_hs).invoke({}, std::ref(modelRegistry), modelEntity);
		type.func("set"_hs).invoke({}, std::ref(m_registry), other, any);
	});
	return other;
}

template<typename T>
T &get(entt::registry &registry, entt::entity entity) {
	return registry.get_or_emplace<T>(entity);
}

template<typename T>
T &set(entt::registry &registry, entt::entity entity, const T &instance) {
	return registry.emplace_or_replace<T>(entity, instance);
}

template<typename T>
Network::Packet serialize(entt::entity entity, T &component) {
	Network::Packet packet;
	if constexpr(std::is_base_of_v<gk::ISerializable, std::decay_t<T>>) {
		if (component.isUpdated) {
			packet << component.packetType << entity << component;
			component.isUpdated = false;
		}
	}
	return packet;
}

template<typename Type>
void extend_meta_type() {
	entt::meta<Type>().type()
		.template func<&get<Type>, entt::as_ref_t>("get"_hs)
		.template func<&set<Type>>("set"_hs)
		.template func<&serialize<Type>>("serialize"_hs);
}

#include <gk/core/Box.hpp>

#include "AnimationComponent.hpp"
#include "DrawableDef.hpp"
#include "ItemStack.hpp"
#include "LuaCallbackComponent.hpp"
#include "NetworkComponent.hpp"
#include "PositionComponent.hpp"
#include "RotationComponent.hpp"

void Scene::registerComponents() {
	extend_meta_type<gk::DoubleBox>();
	extend_meta_type<AnimationComponent>();
	extend_meta_type<DrawableDef>();
	extend_meta_type<ItemStack>();
	extend_meta_type<LuaCallbackComponent>();
	extend_meta_type<NetworkComponent>();
	extend_meta_type<PositionComponent>();
	extend_meta_type<RotationComponent>();
}

