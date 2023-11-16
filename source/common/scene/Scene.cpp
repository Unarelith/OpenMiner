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
#include <entt/entt.hpp>

#include "core/Debug.hpp"
#include "core/ISerializable.hpp"
#include "network/Network.hpp"
#include "network/NetworkUtils.hpp"
#include "scene/Scene.hpp"
#include "scene/component/ComponentType.hpp"

static bool areComponentsRegistered = false;

using namespace entt::literals;

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
		logError() << "Failed to create entity from model: Model entity with ID" << (u32)modelEntity << "doesn't exist";
		return entt::null;
	}

	auto other = m_registry.create();
	modelRegistry.visit(modelEntity, [&](const auto &component) {
		const auto type = entt::resolve(component);
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
Network::Packet serialize(entt::entity entity, T &component, bool useUpdateStatus) {
	Network::Packet packet;
	if constexpr(std::is_base_of_v<ISerializable, std::decay_t<T>>) {
		if (!useUpdateStatus || component.isUpdated) {
			packet << component.packetType << entity << component;
			if (useUpdateStatus)
				component.isUpdated = false;
		}
	}
	return packet;
}

template<typename T>
Network::Packet save(T &component) {
	Network::Packet packet;
	packet << component;
	return packet;
}

template<typename Type>
void extend_meta_type(const std::string &name, ComponentType type, bool isSerializable, bool isSavable) {
	entt::meta<Type>().type()
			.prop("name"_hs, name)
			.prop("type"_hs, type)
			.prop("is_serializable"_hs, isSerializable)
			.prop("is_savable"_hs, isSavable)
		.template func<&get<Type>, entt::as_ref_t>("get"_hs)
		.template func<&set<Type>>("set"_hs)
		.template func<&serialize<Type>>("serialize"_hs)
		.template func<&save<Type>>("save"_hs);
}

#include "core/Box.hpp"
#include "inventory/ItemStack.hpp"
#include "scene/component/AnimationComponent.hpp"
#include "scene/component/DrawableDef.hpp"
#include "scene/component/NetworkComponent.hpp"
#include "scene/component/PositionComponent.hpp"
#include "scene/component/RotationComponent.hpp"

void Scene::registerComponents() {
	extend_meta_type<DoubleBox>          ("DoubleBox",          ComponentType::Hitbox,    false, true);
	extend_meta_type<AnimationComponent> ("AnimationComponent", ComponentType::Animation, true,  true);
	extend_meta_type<DrawableDef>        ("DrawableDef",        ComponentType::Drawable,  true,  true);
	extend_meta_type<ItemStack>          ("ItemStack",          ComponentType::ItemStack, false, true);
	extend_meta_type<NetworkComponent>   ("NetworkComponent",   ComponentType::Network,   false, false);
	extend_meta_type<PositionComponent>  ("PositionComponent",  ComponentType::Position,  true,  true);
	extend_meta_type<RotationComponent>  ("RotationComponent",  ComponentType::Rotation,  true,  true);
	extend_meta_type<std::string>        ("EntityID",           ComponentType::EntityID,  false, true);
}
