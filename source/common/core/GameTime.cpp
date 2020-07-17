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

#include <gk/core/GameClock.hpp>

#include "GameTime.hpp"
#include "Sky.hpp"

u64 GameTime::s_ticks = 0;
u16 GameTime::s_ticksPerSecond = 0;

float GameTime::getCurrentTime(float offset, float speed) {
	return std::fmod((s_ticks + dayStartOffset) * daySpeed * speed + offset, dayLength) / dayLength;
}

float GameTime::getSunlightIntensityFromTime(float time) {
	static const float pi = 3.1415927f;
	return std::clamp(0.5f + std::sin(2 * pi * time) * 2.0f, 0.0f, 1.0f);
}

gk::Color GameTime::getSkyColorFromTime(const Sky &sky, float time) {
	float sunlight = getSunlightIntensityFromTime(time);

	gk::Color skyColor = sky.color();
	skyColor.r = std::clamp(sunlight - (1.f - skyColor.r), 0.0f, skyColor.r);
	skyColor.g = std::clamp(sunlight - (1.f - skyColor.g), 0.0f, skyColor.g);
	skyColor.b = std::clamp(sunlight - (1.f - skyColor.b), 0.0f, skyColor.b);
	skyColor.a = 1.f;

	return skyColor;
}

void GameTime::incrementTicks() {
	++s_ticks;

	updateTpsCounter();
}

void GameTime::updateTpsCounter() {
	static u64 tpsTimer = gk::GameClock::getInstance().getTicks(true);
	static u8 tpsCount = 0;

	u64 currentClockTicks = gk::GameClock::getInstance().getTicks(true);
	++tpsCount;

	if (currentClockTicks - tpsTimer > 1000) {
		s_ticksPerSecond = floor(tpsCount / ((currentClockTicks - tpsTimer) / 1000.0f) + 0.5f);
		tpsTimer = currentClockTicks;
		tpsCount = 0;
	}
}

