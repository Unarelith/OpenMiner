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
#ifndef SERVERCONNECTSTATE_HPP_
#define SERVERCONNECTSTATE_HPP_

#include "InterfaceState.hpp"
#include "TextButton.hpp"
#include "TextInput.hpp"

class ServerConnectState : public InterfaceState {
	public:
		ServerConnectState(gk::ApplicationState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

		void setTexturePack(const std::string &texturePack) { m_texturePack = texturePack; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		TextInput m_textInput;

		TextButton m_connectButton;
		TextButton m_cancelButton;

		std::string m_texturePack;
};

#endif // SERVERCONNECTSTATE_HPP_
