/*
 * =====================================================================================
 *
 *       Filename:  SDLManager.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  20/12/2014 01:24:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:
 *
 * =====================================================================================
 */
#include "Exception.hpp"
#include "SDLHeaders.hpp"
#include "SDLManager.hpp"

bool SDLManager::sdlInitialized = false;
bool SDLManager::imgInitialized = false;
bool SDLManager::mixInitialized = false;

void SDLManager::init() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		EXCEPTION("SDL init error:", SDL_GetError());
	} else {
		sdlInitialized = true;
	}

	int imgFlags = IMG_INIT_PNG;
	if(!IMG_Init(imgFlags) & imgFlags) {
		EXCEPTION("SDL image init error:", SDL_GetError());
	} else {
		imgInitialized = true;
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
		EXCEPTION("SDL image init error:", SDL_GetError());
	} else {
		mixInitialized = true;
	}

	Mix_AllocateChannels(32);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
	Mix_Volume(-1, MIX_MAX_VOLUME);
}

void SDLManager::free() {
	if(mixInitialized) Mix_CloseAudio();
	if(imgInitialized) IMG_Quit();
	if(sdlInitialized) SDL_Quit();
}

