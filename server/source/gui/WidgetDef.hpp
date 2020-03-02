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
#ifndef WIDGETDEF_HPP_
#define WIDGETDEF_HPP_

#include <string>

#include <gk/core/IntTypes.hpp>

#include <sol.hpp>

#include "ISerializable.hpp"
#include "LuaWidget.hpp"
#include "NetworkUtils.hpp"

class WidgetDef : public ISerializable {
	public:
		WidgetDef(u8 type) : m_type(type) {}

		void serialize(sf::Packet &packet) const override;

		virtual void loadFromLuaTable(const sol::table &table);

	protected:
		u8 m_type = 0; // See LuaWidget

		std::string m_name;

		s32 m_x = 0;
		s32 m_y = 0;
};

#endif // WIDGETDEF_HPP_
