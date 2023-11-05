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
#ifndef PROGRESSBARWIDGETDEF_HPP_
#define PROGRESSBARWIDGETDEF_HPP_

#include "WidgetDef.hpp"

// Duplicated in ProgressBarWidget
enum class ProgressBarType : u8 {
	ItemProcess = 0,
	BurnProcess = 1
};

class ProgressBarWidgetDef : public WidgetDef {
	public:
		ProgressBarWidgetDef() : WidgetDef(LuaWidget::ProgressBarWidget) {}

		void serialize(sf::Packet &packet) const override;

		void loadFromLuaTable(const sol::table &table) override;

	private:
		void loadType(const sol::table &table);

		u8 m_type = 0;

		Vector3i m_blockPosition;

		// NOTE: m_maxMeta and m_maxValue can't be used together
		std::string m_meta;
		std::string m_maxMeta;
		u32 m_maxValue = 0;

		std::string m_texture;
		FloatRect m_clipRect;
};

#endif // PROGRESSBARWIDGETDEF_HPP_
