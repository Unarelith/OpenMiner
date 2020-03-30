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
#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <gk/core/IntTypes.hpp>

namespace Config {
	// Gameplay
	extern bool isFlyModeEnabled;
	extern bool isNoClipEnabled;

	// Interface
	extern bool isBlockInfoWidgetEnabled;
	extern bool isFpsCounterEnabled;
	extern bool isHotbarVisible;
	extern bool isCrosshairVisible;

	// Graphics
	extern u16 renderDistance;
	extern bool isTorchSmoothLightingEnabled;
	extern bool isSunSmoothLightingEnabled;
	extern bool isAmbientOcclusionEnabled;
	extern bool isWireframeModeEnabled;
	extern bool isFullscreenModeEnabled;
	extern bool isVerticalSyncEnabled;
	extern float cameraFOV;
	extern u16 screenWidth;
	extern u16 screenHeight;
	extern u8 guiScale;
	extern u8 mipmapLevels;
	extern float aoStrength;

	// Input
	extern u8 mouseSensitivity;

	void loadConfigFromFile(const char *file);
}

#endif // CONFIG_HPP_
