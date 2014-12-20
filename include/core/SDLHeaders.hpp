/*
 * =====================================================================================
 *
 *       Filename:  SDLHeaders.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/12/2014 01:18:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef SDLHEADERS_HPP_
#define SDLHEADERS_HPP_

#ifdef __ANDROID__
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_mixer.h"
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
#endif

#endif // SDLHEADERS_HPP_
