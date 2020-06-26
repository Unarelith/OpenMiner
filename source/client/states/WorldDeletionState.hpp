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
#ifndef WORLDDELETIONSTATE_HPP_
#define WORLDDELETIONSTATE_HPP_

#include "InterfaceState.hpp"
#include "TextButton.hpp"

class TitleScreenState;

class WorldDeletionState : public InterfaceState {
	public:
		WorldDeletionState(const std::string &worldName, TitleScreenState *titleScreen);

		void onEvent(const sf::Event &event) override;

		void update() override;

	private:
		void updateWidgetPosition() override;

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		Text m_text;

		TextButton m_confirmButton;
		TextButton m_cancelButton;
};

#endif // WORLDDELETIONSTATE_HPP_
