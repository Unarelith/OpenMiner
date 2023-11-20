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
#ifndef TEXTUREPACKSELECTIONSTATE_HPP_
#define TEXTUREPACKSELECTIONSTATE_HPP_

#include "client/graphics/Image.hpp"
#include "client/gui/MenuWidget.hpp"
#include "client/gui/ScrollableList.hpp"
#include "client/gui/Text.hpp"
#include "client/states/InterfaceState.hpp"

class TexturePackSelectionState : public InterfaceState {
	public:
		TexturePackSelectionState(DrawableState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

	private:
		void updateWidgetPosition() override;
		void loadTexturePackList();

		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background{"texture-menu_background"};
		RectangleShape m_filter1;
		RectangleShape m_filter2;

		Text m_title;

		ScrollableList m_texturePackList;

		MenuWidget m_menuWidget{2, 1};
};

#endif // TEXTUREPACKSELECTIONSTATE_HPP_
