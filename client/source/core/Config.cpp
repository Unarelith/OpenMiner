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
#include "Config.hpp"

// Gameplay
bool Config::isFlyModeEnabled = false;
bool Config::isNoClipEnabled = false;

// Interface
bool Config::isBlockInfoWidgetEnabled = true;

// Graphics
u16 Config::renderDistance = 8;
bool Config::isTorchSmoothLightingEnabled = true;
bool Config::isSunSmoothLightingEnabled = true;
bool Config::isAmbientOcclusionEnabled = false;
bool Config::isWireframeModeEnabled = false;
bool Config::isFullscreenModeEnabled = false;
bool Config::isVerticalSyncEnabled = true;
float Config::cameraFOV = 70.0f;
u16 Config::screenWidth = 1600;
u16 Config::screenHeight = 1050;
u8 Config::guiScale = 3;

// Input
u8 Config::mouseSensitivity = 8;

#include <iostream>

#include <gk/core/Filesystem.hpp>

#include <sol.hpp>

void Config::loadConfigFromFile(const char *file) {
	if (gk::Filesystem::fileExists(file)) {
		sol::state lua;

		try {
			lua.safe_script_file(file);

			isFlyModeEnabled = lua["isFlyModeEnabled"].get_or(isFlyModeEnabled);
			isNoClipEnabled = lua["isNoClipEnabled"].get_or(isNoClipEnabled);

			isBlockInfoWidgetEnabled = lua["isBlockInfoWidgetEnabled"].get_or(isBlockInfoWidgetEnabled);

			renderDistance = lua["renderDistance"].get_or(renderDistance);
			isTorchSmoothLightingEnabled = lua["isTorchSmoothLightingEnabled"].get_or(isTorchSmoothLightingEnabled);
			isSunSmoothLightingEnabled = lua["isSunSmoothLightingEnabled"].get_or(isSunSmoothLightingEnabled);
			isAmbientOcclusionEnabled = lua["isAmbientOcclusionEnabled"].get_or(isAmbientOcclusionEnabled);
			isWireframeModeEnabled = lua["isWireframeModeEnabled"].get_or(isWireframeModeEnabled);
			isFullscreenModeEnabled = lua["isFullscreenModeEnabled"].get_or(isFullscreenModeEnabled);
			isVerticalSyncEnabled = lua["isVerticalSyncEnabled"].get_or(isVerticalSyncEnabled);
			cameraFOV = lua["cameraFOV"].get_or(cameraFOV);
			screenWidth = lua["screenWidth"].get_or(screenWidth);
			screenHeight = lua["screenHeight"].get_or(screenHeight);
			guiScale = lua["guiScale"].get_or(guiScale);

			mouseSensitivity = lua["mouseSensitivity"].get_or(mouseSensitivity);

			std::cout << "Config file loaded successfully" << std::endl;
		}
		catch (sol::error &e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

