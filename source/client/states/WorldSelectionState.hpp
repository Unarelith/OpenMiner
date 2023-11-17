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
#ifndef WORLDSELECTIONSTATE_HPP_
#define WORLDSELECTIONSTATE_HPP_

#include "client/gui/MenuWidget.hpp"
#include "client/gui/ScrollableList.hpp"
#include "client/states/InterfaceState.hpp"

class TitleScreenState;

class WorldSelectionState : public InterfaceState {
	public:
		WorldSelectionState(TitleScreenState *titleScreen);

		void onEvent(const SDL_Event &event) override;

	private:
		void updateWidgetPosition() override;
		void loadSaveList();

		void draw(RenderTarget &target, RenderStates states) const override;

		TitleScreenState *m_titleScreen = nullptr;

		Image m_background{"texture-menu_background"};
		RectangleShape m_filter1;
		RectangleShape m_filter2;

		Text m_title;

		ScrollableList m_worldList;

		MenuWidget m_menuWidget1{2, 1};
		MenuWidget m_menuWidget2{2, 1};
		MenuWidget m_menuWidget3{2, 1};
};

#endif // WORLDSELECTIONSTATE_HPP_
