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
#include <cassert>
#include <iomanip>
#include <sstream>

#include <gk/core/IntTypes.hpp>
#include <gk/core/SDLHeaders.hpp>

#include "Utils.hpp"

namespace utils {

bool regexMatch(const std::string &str, const std::string &regex) {
	std::regex re(regex);
	std::cmatch m;

	return std::regex_match(str.c_str(), m, re);
}

std::string getCurrentTime(const char *format) {
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);

	std::stringstream sstream;
	sstream << std::put_time(&tm, format);

	return sstream.str();
}

// TODO: A class similar to `sf::Image` should be created around SDL_Surface and SDL2_image

// From: https://stackoverflow.com/a/5867170/1392477
SDL_Surface *flipSDLSurface(SDL_Surface *sfc) noexcept {
	SDL_Surface *result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
			sfc->format->BytesPerPixel * 8, sfc->format->Rmask, sfc->format->Gmask,
			sfc->format->Bmask, sfc->format->Amask);

	u8 *pixels = (u8 *)sfc->pixels;
	u8 *rpixels = (u8 *)result->pixels;

	u32 pitch = sfc->pitch;
	u32 pxlength = pitch * sfc->h;

	assert(result != nullptr);

	for (u32 line = 0; line < (u32)sfc->h; ++line) {
		u32 pos = line * pitch;
		for (u32 i = 0 ; i <= pitch ; ++i) {
			rpixels[pos + i] = pixels[pxlength - pos - (pitch - i)];
		}
	}

	return result;
}

} // namespace utils
