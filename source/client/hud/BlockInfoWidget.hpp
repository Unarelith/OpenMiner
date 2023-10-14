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
#ifndef BLOCKINFOWIDGET_HPP_
#define BLOCKINFOWIDGET_HPP_

#include <gk/graphics/Sprite.hpp>

#include "ItemWidget.hpp"

class BlockInfoWidget : public Widget {
	public:
		BlockInfoWidget(Widget *parent = nullptr);

		void update() override;

		const BlockState *currentBlock() const { return m_currentBlock; }
		void setCurrentBlock(const BlockState *blockState);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Inventory m_inventory{1, 1};
		ItemWidget m_itemWidget{m_inventory, 0, 0, this};

		gk::Sprite m_background{"texture-toasts", 160, 32};
		Text m_text;

		bool m_isVisible = false;

		const BlockState *m_currentBlock = nullptr;
};

#endif // BLOCKINFOWIDGET_HPP_
