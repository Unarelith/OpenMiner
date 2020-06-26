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
#ifndef SETTINGSMENUSTATE_HPP_
#define SETTINGSMENUSTATE_HPP_

#include <gk/graphics/RectangleShape.hpp>

#include "GameKey.hpp"
#include "InterfaceState.hpp"
#include "MenuWidget.hpp"

struct GuiScaleChangedEvent;
class Key;

class SettingsMenuState : public InterfaceState {
	public:
		SettingsMenuState(gk::ApplicationState *parent = nullptr);

		void init() override;

		void onEvent(const sf::Event &event) override;

	private:
		void onGuiScaleChanged(const GuiScaleChangedEvent &event);

		void updateWidgetPosition() override;

		void doneButtonAction();

		void addMainButtons();
		void addGameplayButtons();
		void addInterfaceButtons();
		void addGraphicsButtons();
		void addInputButtons();

		TextButton &addToggleButton(const std::string &text, bool &configOption, bool worldReloadRequested = false);

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Image m_background{"texture-menu_background"};
		gk::RectangleShape m_filter1;
		gk::RectangleShape m_filter2;

		Text m_title;

		MenuWidget m_menuWidget;
		TextButton m_doneButton;

		u16 m_currentKey = GameKey::Undefined;
		TextButton *m_currentKeyButton = nullptr;
		Key *m_key = nullptr;

		enum class MenuState {
			Main,
			Gameplay,
			Graphics,
			Input
		};

		MenuState m_state = MenuState::Main;
};

#endif // SETTINGSMENUSTATE_HPP_
