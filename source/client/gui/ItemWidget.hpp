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

#include "common/inventory/Inventory.hpp"

#include "client/graphics/Image.hpp"
#include "client/gui/InventoryCube.hpp"
#include "client/gui/Text.hpp"
#include "client/gui/Widget.hpp"

class BlockState;

class ItemWidget : public Widget {
	public:
		ItemWidget(Inventory &inventory, u16 x, u16 y, Widget *parent = nullptr);

		void update() override;
		void updateImage(const BlockState *blockState = nullptr);

		void changeItem(u16 x, u16 y);
		void disable();

		const ItemStack &stack() const { return m_inventory.getStack(m_x, m_y); }
		void setStack(const std::string &name, u16 amount = 1);

		u16 x() const { return m_x; }
		u16 y() const { return m_y; }

		bool hasChanged() const { return m_hasChanged; }
		void setChanged(bool hasChanged) { m_hasChanged = hasChanged; }

		bool isEnabled() const { return m_isEnabled; }

	protected:
		void draw(RenderTarget &target, RenderStates states) const override;

	private:
		Inventory &m_inventory;

		u16 m_x = 0;
		u16 m_y = 0;

		TextureAtlas &m_textureAtlas;

		Image m_image;
		Text m_text;

		InventoryCube m_cube{10};

		bool m_isImage = false;

		bool m_hasChanged = false;

		std::string m_oldTexturePack{""};

		bool m_isEnabled = true;

		u16 m_itemID = 0;
		u16 m_itemAmount = 0;
};

#endif // ITEMWIDGET_HPP_
