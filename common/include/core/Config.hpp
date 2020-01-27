/*
 * =====================================================================================
 *
 *       Filename:  Config.hpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 13:45:14
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <gk/core/IntTypes.hpp>

namespace {
	constexpr const char *APP_NAME = "OpenMiner";

	constexpr float DIST_NEAR = 0.1f;
	constexpr float DIST_FAR  = 1000.0f;

	constexpr int CHUNK_WIDTH  = 16;
	constexpr int CHUNK_HEIGHT = 32;
	constexpr int CHUNK_DEPTH  = 16;

	constexpr int SEALEVEL = 4;
}

// FIXME
extern float SCREEN_WIDTH;
extern float SCREEN_HEIGHT;

// FIXME
extern int GUI_SCALE;

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

	// Input
	extern u8 mouseSensitivity;
}

#endif // CONFIG_HPP_
