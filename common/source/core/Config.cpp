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

// FIXME
float SCREEN_WIDTH = 1600;
float SCREEN_HEIGHT = 1050;

// FIXME
int GUI_SCALE = 3;

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

// Input
u8 Config::mouseSensitivity = 8;

