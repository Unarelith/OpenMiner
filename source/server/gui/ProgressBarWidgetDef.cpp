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
#include "common/core/Debug.hpp"
#include "server/gui/ProgressBarWidgetDef.hpp"

void ProgressBarWidgetDef::serialize(sf::Packet &packet) const {
	WidgetDef::serialize(packet);

	packet << m_type << m_blockPosition
		<< m_meta << m_maxMeta << m_maxValue
		<< m_texture << m_clipRect;
}

void ProgressBarWidgetDef::loadFromLuaTable(const sol::table &table) {
	WidgetDef::loadFromLuaTable(table);

	loadType(table);

	sol::optional<sol::table> blockTable = table["block"];
	if (blockTable != sol::nullopt) {
		m_blockPosition.x = blockTable.value()["x"];
		m_blockPosition.y = blockTable.value()["y"];
		m_blockPosition.z = blockTable.value()["z"];
	}
	else {
		logError() << "Attribute 'block' not defined for bar" << m_name;
		return;
	}

	m_meta = table["meta"].get<std::string>();
	m_maxValue = table["max_value"].get_or<u32>(0);
	if (m_maxValue == 0) {
		m_maxMeta = table["max_meta"].get<std::string>();
	}

	m_texture = table["texture"].get<std::string>();

	sol::optional<sol::table> clipRectTable = table["clip"];
	if (clipRectTable != sol::nullopt) {
		m_clipRect.x = clipRectTable.value()["x"];
		m_clipRect.y = clipRectTable.value()["y"];
		m_clipRect.sizeX = clipRectTable.value()["width"];
		m_clipRect.sizeY = clipRectTable.value()["height"];
	}
}

inline void ProgressBarWidgetDef::loadType(const sol::table &table) {
	sol::object typeObject = table["type"].get<sol::object>();
	if (typeObject.valid()) {
		if (typeObject.get_type() == sol::type::string) {
			static const std::unordered_map<std::string, ProgressBarType> types = {
				{"item_process", ProgressBarType::ItemProcess},
				{"burn_process", ProgressBarType::BurnProcess},
			};

			auto it = types.find(typeObject.as<std::string>());
			if (it != types.end())
				m_type = u8(it->second);
			else
				logError() << "In" << m_name << "definition: Type invalid";
		}
		else
			logError() << "In" << m_name << "definition: Type must be a string";
	}
}
