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
#include <SDL_image.h>

#include "common/core/Exception.hpp"
#include "client/core/SDLLoader.hpp"

SDLLoader::~SDLLoader() {
	if(m_imgInitialized) IMG_Quit();
	if(m_sdlInitialized) SDL_Quit();
}

void SDLLoader::load() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw EXCEPTION("SDL init error:", SDL_GetError());
	} else {
		m_sdlInitialized = true;
	}

	int imgFlags = IMG_INIT_PNG;
	if((!IMG_Init(imgFlags)) & imgFlags) {
		throw EXCEPTION("SDL_image init error:", IMG_GetError());
	} else {
		m_imgInitialized = true;
	}
}
