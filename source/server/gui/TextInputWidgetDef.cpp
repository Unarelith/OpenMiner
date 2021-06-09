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
#include "TextInputWidgetDef.hpp"

void TextInputWidgetDef::serialize(sf::Packet &packet) const {
	WidgetDef::serialize(packet);

	packet << m_width << m_height << m_placeholder << m_placeholderColor << m_inventory;
}

void TextInputWidgetDef::loadFromLuaTable(const sol::table &table) {
	WidgetDef::loadFromLuaTable(table);

	m_width = table["width"].get<u16>();
	m_height = table["height"].get<u16>();

	m_placeholder = table["placeholder"].get_or<std::string>("");

	sol::optional<sol::table> placeholderColor = table["placeholder_color"];
	if (placeholderColor != sol::nullopt) {
		m_placeholderColor = gk::Color::fromRGBA32(
			placeholderColor.value().get<u8>(1),
			placeholderColor.value().get<u8>(2),
			placeholderColor.value().get<u8>(3),
			placeholderColor.value().get_or<u8>(4, 255)
		);
	}

	m_inventory = table["inventory"].get<std::string>();
}

