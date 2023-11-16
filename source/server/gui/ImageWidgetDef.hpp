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
#ifndef IMAGEWIDGETDEF_HPP_
#define IMAGEWIDGETDEF_HPP_

#include "common/core/Rect.hpp"
#include "server/gui/WidgetDef.hpp"

class ImageWidgetDef : public WidgetDef {
	public:
		ImageWidgetDef() : WidgetDef(LuaWidget::Image) {}

		void serialize(sf::Packet &packet) const override;

		void loadFromLuaTable(const sol::table &table) override;

	private:
		std::string m_texture;

		FloatRect m_clipRect;
};

#endif // IMAGEWIDGETDEF_HPP_
