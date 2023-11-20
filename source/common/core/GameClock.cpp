/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <cmath>

#include <SDL.h>

#include "common/core/GameClock.hpp"

GameClock *GameClock::s_instance = nullptr;

u32 GameClock::getTicks(bool realTime) {
	if (realTime) {
		return SDL_GetTicks();
	}
	else {
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_ticks;
	}
}

void GameClock::updateGame(const std::function<void(void)> &updateFunc) {
	std::unique_lock<std::mutex> lock(m_mutex);

	m_numUpdates = 0;

	while (m_lag >= m_timestep && m_numUpdates < 10) {
		m_ticks += m_timestep;

		lock.unlock();
		updateFunc();
		lock.lock();

		m_lag -= m_timestep;
		m_numUpdates++;
	}
}

void GameClock::drawGame(const std::function<void(void)> &drawFunc) {
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_numUpdates > 0) {
		lock.unlock();
		drawFunc();
		lock.lock();
		++m_frames;
	}

	lock.unlock();

	computeFramesPerSecond();
	waitForNextFrame();
}

void GameClock::waitForNextFrame() {
	u32 currentTicks = getTicks(true);

	std::unique_lock<std::mutex> lock(m_mutex);

	u32 lastFrameDuration = currentTicks - m_timeDropped - m_lastFrameDate;

	if (lastFrameDuration < m_timestep) {
		SDL_Delay(m_timestep - lastFrameDuration);
	}

	lock.unlock();

	measureLastFrameDuration();
}

void GameClock::measureLastFrameDuration() {
	u32 currentTicks = getTicks(true);

	std::lock_guard<std::mutex> lock(m_mutex);

	u32 now = currentTicks - m_timeDropped;
	u32 lastFrameDuration = now - m_lastFrameDate;

	m_lastFrameDate = now;
	m_lag += lastFrameDuration;

	if (m_lag >= 200) {
		m_timeDropped += m_lag - m_timestep;
		m_lag = m_timestep;
		m_lastFrameDate = currentTicks - m_timeDropped;
	}
}

void GameClock::computeFramesPerSecond() {
	u32 currentTicks = getTicks(true);

	std::unique_lock<std::mutex> lock(m_mutex);

	if (currentTicks - m_fpsTimer > 1000) {
		m_fps = u16(std::floor(m_frames / (float(currentTicks - m_fpsTimer) / 1000.0f) + 0.5f));
		m_fpsTimer = currentTicks;

		m_frames = 0;
	}
}
