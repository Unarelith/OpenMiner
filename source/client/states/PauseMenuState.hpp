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
#ifndef PAUSEMENUSTATE_HPP_
#define PAUSEMENUSTATE_HPP_

#include "client/gui/MenuWidget.hpp"
#include "client/states/InterfaceState.hpp"

class Client;
struct GuiScaleChangedEvent;

class PauseMenuState : public InterfaceState {
	public:
		PauseMenuState(Client &client, DrawableState *parent = nullptr);

		void init() override;

		void onEvent(const SDL_Event &event) override;

	private:
		void onGuiScaleChanged(const GuiScaleChangedEvent &event);

		void updateWidgetPosition() override;

		void draw(RenderTarget &target, RenderStates states) const override;

		MenuWidget m_menuWidget{1, 4};

		Client &m_client;
};

#endif // PAUSEMENUSTATE_HPP_
