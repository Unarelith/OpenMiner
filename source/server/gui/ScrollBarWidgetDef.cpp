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
#include "server/gui/ScrollBarWidgetDef.hpp"

void ScrollBarWidgetDef::serialize(sf::Packet &packet) const {
	WidgetDef::serialize(packet);

	packet << m_texture << m_clipRect << m_minY << m_maxY << m_widget;
}

void ScrollBarWidgetDef::loadFromLuaTable(const sol::table &table) {
	WidgetDef::loadFromLuaTable(table);

	m_texture = table["texture"].get<std::string>();

	sol::optional<sol::table> clipRectTable = table["clip"];
	if (clipRectTable != sol::nullopt) {
		m_clipRect.x = clipRectTable.value()["x"];
		m_clipRect.y = clipRectTable.value()["y"];
		m_clipRect.sizeX = clipRectTable.value()["width"];
		m_clipRect.sizeY = clipRectTable.value()["height"];
	}

	m_minY = table["min_y"].get<u16>();
	m_maxY = table["max_y"].get<u16>();

	m_widget = table["widget"].get<std::string>();
}
