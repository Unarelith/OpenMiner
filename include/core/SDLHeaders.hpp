/*
 * =====================================================================================
 *
 *       Filename:  SDLHeaders.hpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 01:18:31
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SDLHEADERS_HPP_
#define SDLHEADERS_HPP_

#ifdef USE_SDL
	#ifdef __ANDROID__
		#include "SDL.h"
		#include "SDL_image.h"
		#include "SDL_mixer.h"
	#else
		#include <SDL2/SDL.h>
		#include <SDL2/SDL_image.h>
		#include <SDL2/SDL_mixer.h>
	#endif

#elif defined USE_SFML
	#include <SFML/Graphics.hpp>
#endif // USE_SDL, USE_SFML

#endif // SDLHEADERS_HPP_
