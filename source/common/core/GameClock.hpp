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
#ifndef GAMECLOCK_HPP_
#define GAMECLOCK_HPP_

#include <functional>
#include <mutex>

#include "core/IntTypes.hpp"

class GameClock {
	public:
		u32 getTicks(bool realTime = false);
		u16 getFpsAverage() const { return m_fps; }

		void updateGame(const std::function<void(void)> &updateFunc);
		void drawGame(const std::function<void(void)> &drawFunc);

		void waitForNextFrame();

		void startFpsTimer() {
			m_fpsTimer = getTicks(true);
		}

		void setTimestep(u8 timestep) {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_timestep = timestep;
		}

		static GameClock &getInstance() { return *s_instance; }
		static void setInstance(GameClock &clock) { s_instance = &clock; }

	private:
		void measureLastFrameDuration();
		void computeFramesPerSecond();

		static GameClock *s_instance;

		u32 m_ticks = 0;
		u16 m_fps = 0;

		std::mutex m_mutex;

		u32 m_lastFrameDate = 0;
		u32 m_lag = 0;
		u32 m_timeDropped = 0;

		u8 m_timestep = 6;
		u8 m_numUpdates = 0;

		u32 m_fpsTimer = 0;
		u16 m_frames = 0;
};

#endif // GAMECLOCK_HPP_
