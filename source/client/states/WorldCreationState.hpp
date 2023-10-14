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
#ifndef WORLDCREATIONSTATE_HPP_
#define WORLDCREATIONSTATE_HPP_

#include "InterfaceState.hpp"
#include "MenuWidget.hpp"
#include "TextInput.hpp"

class TitleScreenState;

class WorldCreationState : public InterfaceState {
	public:
		WorldCreationState(TitleScreenState *titleScreen, const std::string &originalName = "");

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void updateWidgetPosition() override;

		void draw(RenderTarget &target, RenderStates states) const override;

		gk::Image m_background{"texture-menu_background"};
		RectangleShape m_filter;

		Text m_title;

		TextInput m_nameInput;
		TextInput m_seedInput;

		MenuWidget m_menuWidget{2, 1};

		Text m_errorText;

		bool m_isEdition = false;
};

#endif // WORLDCREATIONSTATE_HPP_
