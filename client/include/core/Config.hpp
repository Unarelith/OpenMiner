/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <gk/core/IntTypes.hpp>

namespace Config {
	// Gameplay
	extern bool isFlyModeEnabled;
	extern bool isNoClipEnabled;

	// Graphics
	extern u16 renderDistance;
	extern bool isTorchSmoothLightingEnabled;
	extern bool isSunSmoothLightingEnabled;
	extern bool isAmbientOcclusionEnabled;
	extern bool isWireframeModeEnabled;
	extern bool isFullscreenModeEnabled;
	extern float cameraFOV;
	extern u16 screenWidth;
	extern u16 screenHeight;
	extern u8 guiScale;

	// Input
	extern u8 mouseSensitivity;

	void loadConfigFromFile(const char *file);
}

#endif // CONFIG_HPP_
