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

namespace {
	constexpr const char *APP_NAME = "KubKraft";

	constexpr float SCREEN_WIDTH  = 1600;
	constexpr float SCREEN_HEIGHT = 1050;

	constexpr float DIST_NEAR = 0.1f;
	constexpr float DIST_FAR  = 1000.0f;

	constexpr int CHUNK_WIDTH  = 16.0f;
	constexpr int CHUNK_HEIGHT = 32.0f;
	constexpr int CHUNK_DEPTH  = 16.0f;

	constexpr int SEALEVEL = 4;

	constexpr int GUI_SCALE = 3;
}

#endif // CONFIG_HPP_
