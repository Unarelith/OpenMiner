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
#include <SDL.h>

#include "Mouse.hpp"

Window *Mouse::s_window = nullptr;

void Mouse::resetToWindowCenter() {
	SDL_WarpMouseInWindow(s_window->window(), s_window->getSize().x / 2, s_window->getSize().y / 2);
}

void Mouse::setCursorGrabbed(bool grabbed) {
	SDL_SetRelativeMouseMode((SDL_bool)grabbed);
}

void Mouse::setCursorVisible(bool visible) {
	SDL_ShowCursor(visible);
}

Vector2i Mouse::getPosition() {
	Vector2i pos;
	SDL_GetMouseState(&pos.x, &pos.y);
	return pos;
}

bool Mouse::isInRect(const IntRect &rect) {
	return rect.contains(getPosition());
}

