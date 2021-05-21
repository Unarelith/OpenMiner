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
#include <gk/core/Debug.hpp>
#include <gk/core/GameClock.hpp>

#include "ClientProfiler.hpp"

ClientProfiler *ClientProfiler::s_instance = nullptr;

void ClientProfiler::onBeginTick() {
	m_ticks.emplace_back();
	m_ticks.back().begin = gk::GameClock::getInstance().getTicks(true);
}

void ClientProfiler::onEndTick() {
	m_ticks.back().end = gk::GameClock::getInstance().getTicks(true);
}

void ClientProfiler::startAction(const std::string &name) {
	m_ticks.back().actions[name].durations.emplace_back();
	m_ticks.back().actions[name].durations.back().first = gk::GameClock::getInstance().getTicks(true);
}

void ClientProfiler::endAction(const std::string &name) {
	m_ticks.back().actions[name].durations.back().second = gk::GameClock::getInstance().getTicks(true);
}

void ClientProfiler::dump(u64 tickDurationMin) {
	std::unordered_map<std::string, std::pair<u64, u64>> maxActionDuration;

	u64 i = 0;
	for (auto &tick : m_ticks) {
		u64 tickDuration = tick.end - tick.begin;
		if (tickDuration < tickDurationMin) continue;

		gkDebug() << "Tick" << i++ << "took" << tickDuration << "ms";

		if (tickDuration > 0) {
			for (auto &[actionName, action] : tick.actions) {
				u64 j = 0;
				for (auto &[begin, end] : action.durations) {
					u64 actionDuration = end - begin;

					if (maxActionDuration.find(actionName) == maxActionDuration.end())
						maxActionDuration.emplace(actionName, std::make_pair(actionDuration, 0));

					maxActionDuration[actionName].first = std::max(maxActionDuration[actionName].first, actionDuration);

					float actionPercentTotal = actionDuration / (float)tickDuration * 100.f;

					gkDebug() << "Action" << actionName << j++ << "took" << actionDuration << "ms -" << actionPercentTotal << "% of tick";
				}

				maxActionDuration[actionName].second = std::max(maxActionDuration[actionName].second, j);
			}
		}
	}

	for (auto &[actionName, pair] : maxActionDuration) {
		gkDebug() << "Max duration for action" << actionName << ":" << pair.first << "ms (" << pair.second << ")";
	}
}

