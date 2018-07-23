/*
 * =====================================================================================
 *
 *       Filename:  GameClock.cpp
 *
 *    Description:
 *
 *        Created:  14/12/2014 13:42:26
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "GameClock.hpp"
#include "SDLHeaders.hpp"

u32 GameClock::ticks = 0;

u32 GameClock::getTicks(bool realTime) {
#ifdef USE_SFML
    static sf::Clock clock;
#endif // USE_SFML
	if(realTime) {
#ifdef USE_SDL
		return SDL_GetTicks();
#elif defined USE_SFML
		return clock.getElapsedTime().asMilliseconds();
#endif // USE_SDL, USE_SFML
	} else {
		return ticks;
	}
}

void GameClock::measureLastFrameDuration() {
	u32 now = getTicks(true) - m_timeDropped;
	u32 lastFrameDuration = now - m_lastFrameDate;

	m_lastFrameDate = now;
	m_lag += lastFrameDuration;

	if(m_lag >= 200) {
		m_timeDropped += m_lag - m_timestep;
		m_lag = m_timestep;
		m_lastFrameDate = getTicks(true) - m_timeDropped;
	}
}

void GameClock::updateGame(std::function<void(void)> updateFunc) {
	m_numUpdates = 0;

	while(m_lag >= m_timestep && m_numUpdates < 10) {
		ticks += m_timestep;

		updateFunc();

		m_lag -= m_timestep;
		m_numUpdates++;
	}
}

void GameClock::drawGame(std::function<void(void)> drawFunc) {
	if(m_numUpdates > 0) {
		drawFunc();
	}

	waitForNextFrame();
}

void GameClock::waitForNextFrame() {
	u32 lastFrameDuration = getTicks(true) - m_timeDropped - m_lastFrameDate;

	if(lastFrameDuration < m_timestep) {
#ifdef USE_SDL
		SDL_Delay(m_timestep - lastFrameDuration);
#elif defined USE_SFML
        sf::sleep(sf::milliseconds(m_timestep - lastFrameDuration));
#endif // USE_SDL, USE_SFML
	}

	measureLastFrameDuration();
}

