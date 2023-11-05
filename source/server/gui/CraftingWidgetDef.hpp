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
#ifndef CRAFTINGWIDGETDEF_HPP_
#define CRAFTINGWIDGETDEF_HPP_

#include "WidgetDef.hpp"

class CraftingWidgetDef : public WidgetDef {
	public:
		CraftingWidgetDef() : WidgetDef(LuaWidget::CraftingWidget) {}

		void serialize(sf::Packet &packet) const override;

		void loadFromLuaTable(const sol::table &table) override;

	private:
		void loadInventory(const sol::table &table);

		s32 m_resultX = 0;
		s32 m_resultY = 0;

		std::string m_shiftDestination;

		std::string m_inventory;

		Vector3i m_blockPosition; // inventory == "block"

		u16 m_offset = 0;             // inventory == "block"
		u16 m_size = 3;               // inventory == "temp" or "block"
};

#endif // CRAFTINGWIDGETDEF_HPP_
