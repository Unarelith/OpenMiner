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

float GameTime::getCurrentTime(float offset, float speed) {
	return std::fmod(gk::GameClock::getInstance().getTicks() * daySpeed * speed / 1000.f + offset, 360.f) / 360.f;
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

