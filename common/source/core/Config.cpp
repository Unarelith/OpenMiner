/*
 * =====================================================================================
 *
 *       Filename:  Config.cpp
 *
 *    Description:
 *
 *        Created:  06/07/2018 11:41:16
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"

int GUI_SCALE = 3; // FIXME

// Gameplay
bool Config::isFlyModeEnabled = false;
bool Config::isNoClipEnabled = false;

// Graphics
bool Config::isTorchSmoothLightingEnabled = true;
bool Config::isSunSmoothLightingEnabled = false;
bool Config::isAmbientOcclusionEnabled = false;
bool Config::isWireframeModeEnabled = false;
u16 Config::renderDistance = 8;

