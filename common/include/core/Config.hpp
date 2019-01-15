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

	constexpr float SCREEN_WIDTH  = 1600;
	constexpr float SCREEN_HEIGHT = 1050;

	constexpr float FOV = 45.0f;

	constexpr float DIST_NEAR = 0.01f;
	constexpr float DIST_FAR  = 1000.0f;

	constexpr int CHUNK_WIDTH  = 16.0f;
	constexpr int CHUNK_HEIGHT = 32.0f;
	constexpr int CHUNK_DEPTH  = 16.0f;

	constexpr int SEALEVEL = 4;
}

extern int GUI_SCALE; // FIXME

namespace Config {
	// Gameplay
	extern bool isFlyModeEnabled;

	// Graphics
	extern bool isSmoothLightingEnabled;
	extern bool isAmbientOcclusionEnabled;
	extern bool isWireframeModeEnabled;
	extern u16 renderDistance;
}

#endif // CONFIG_HPP_
