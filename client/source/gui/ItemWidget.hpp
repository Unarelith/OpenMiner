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
#ifndef ITEMWIDGET_HPP_
#define ITEMWIDGET_HPP_

#include <gk/graphics/Image.hpp>

#include "Inventory.hpp"
#include "InventoryCube.hpp"
#include "Text.hpp"
#include "Widget.hpp"

class ItemWidget : public Widget {
	public:
		ItemWidget(Inventory &inventory, u16 x, u16 y, Widget *parent = nullptr);

		void update() override;
		void updateImage(const Block *block = nullptr);

		const ItemStack &stack() const { return m_inventory.getStack(m_x, m_y); }
		void setStack(const std::string &name, unsigned int amount = 1);

	protected:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

	private:
		Inventory &m_inventory;

		unsigned int m_x = 0;
		unsigned int m_y = 0;

		TextureAtlas &m_textureAtlas;

		gk::Image m_image;
		Text m_text;

		InventoryCube m_cube{10};

		bool m_isImage = false;
};

#endif // ITEMWIDGET_HPP_
