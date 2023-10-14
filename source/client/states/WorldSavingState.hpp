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
#ifndef WORLDSAVINGSTATE_HPP_
#define WORLDSAVINGSTATE_HPP_

#include <gk/graphics/Image.hpp>

#include "InterfaceState.hpp"
#include "Text.hpp"

class Client;

class WorldSavingState : public InterfaceState {
	public:
		WorldSavingState(Client &client, bool closeClient, gk::ApplicationState *parent = nullptr);

		void update() override;

	private:
		void updateWidgetPosition() override;

		void draw(RenderTarget &target, RenderStates states) const override;

		gk::Image m_background{"texture-menu_background"};
		RectangleShape m_filter;

		Client &m_client;

		bool m_closeClient = false;

		bool m_isWorldSaved = false;
		mutable bool m_hasBeenRendered = false;

		Text m_text;
};

#endif // WORLDSAVINGSTATE_HPP_
