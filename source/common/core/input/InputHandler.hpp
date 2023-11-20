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
#ifndef INPUTHANDLER_HPP_
#define INPUTHANDLER_HPP_

#include <map>

#include "common/core/IntTypes.hpp"

using GameKeyID = u32;

class InputHandler {
	public:
		virtual ~InputHandler() = default;

		virtual bool isKeyPressed(GameKeyID key) { return m_keysPressed[key]; }
		virtual bool isKeyPressedOnce(GameKeyID key);
		virtual bool isKeyPressedWithDelay(GameKeyID key, u16 delay);

		const std::map<GameKeyID, bool> &keysPressed() const { return m_keysPressed; }

	protected:
		void addKey(GameKeyID key);

		std::map<GameKeyID, bool> m_keysPressed;
		std::map<GameKeyID, bool> m_keysPressedOnce;
		std::map<GameKeyID, u32> m_lastTimePressed;
};

#endif // INPUTHANDLER_HPP_
