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
#include <algorithm>
#include <cmath>

#include "GameClock.hpp"
#include "GameTime.hpp"
#include "Sky.hpp"

u64 GameTime::s_ticks = 0;
u16 GameTime::s_ticksPerSecond = 0;

float GameTime::getCurrentTime(float offset, float speed) {
	return float(fmod(double(s_ticks + dayStartOffset) * daySpeed * speed + offset, dayLength)) / dayLength;
}

float GameTime::getSunlightIntensityFromTime(float time) {
	static const float pi = 3.1415927f;
	return std::clamp(0.5f + sinf(2 * pi * time) * 2.f, 0.f, 1.f);
}

Color GameTime::getSkyColorFromTime(const Sky &sky, float time) {
	float sunlight = getSunlightIntensityFromTime(time);

	Color skyColor = sky.color();
	skyColor.r = std::clamp(sunlight - (1.f - skyColor.r), 0.f, skyColor.r);
	skyColor.g = std::clamp(sunlight - (1.f - skyColor.g), 0.f, skyColor.g);
	skyColor.b = std::clamp(sunlight - (1.f - skyColor.b), 0.f, skyColor.b);
	skyColor.a = 1.f;

	return skyColor;
}

void GameTime::updateTpsCounter() {
	static u64 tpsTimer = GameClock::getInstance().getTicks(true);
	static u64 tpsStart = s_ticks;

	if (tpsStart > s_ticks)
		tpsStart = s_ticks;

	u64 currentClockTicks = GameClock::getInstance().getTicks(true);
	u32 elapsed = u32(currentClockTicks - tpsTimer);
	if (elapsed >= 1000) {
		s_ticksPerSecond = u16(((s_ticks - tpsStart) * 1000 + elapsed / 2) / elapsed);
		tpsTimer = currentClockTicks;
		tpsStart = s_ticks;
	}
}

