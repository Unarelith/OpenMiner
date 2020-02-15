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

// Graphics
u16 Config::renderDistance = 8;
bool Config::isTorchSmoothLightingEnabled = true;
bool Config::isSunSmoothLightingEnabled = true;
bool Config::isAmbientOcclusionEnabled = false;
bool Config::isWireframeModeEnabled = false;
bool Config::isFullscreenModeEnabled = false;
float Config::cameraFOV = 70.0f;
u16 Config::screenWidth = 1600;
u16 Config::screenHeight = 1050;
u8 Config::guiScale = 3;

// Input
u8 Config::mouseSensitivity = 8;

