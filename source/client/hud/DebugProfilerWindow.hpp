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
#ifndef DEBUGPROFILERWINDOW_HPP_
#define DEBUGPROFILERWINDOW_HPP_

#include <chrono>
#include <limits>

#include <gk/core/SDLHeaders.hpp>

#include "ClientProfiler.hpp"
#include "Text.hpp"

enum class DebugProfilerWindowMode {
	Total,
	Prev,
	Min,
	Max
};

class DebugProfilerSnapshotValue {
	public:
		void addDuration(const ClientTick *tick, const TimePoint &start, const TimePoint &end) {
			m_lastValue = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			m_totalTime += m_lastValue;

			m_minValue = std::min(m_minValue, m_lastValue);
			m_maxValue = std::max(m_maxValue, m_lastValue);

			m_values[tick].emplace_back(m_lastValue);
			m_lastTick = tick;
		}

		void clear() {
			m_lastValue = 0;
			m_totalTime = 0;

			m_minValue = std::numeric_limits<u64>::max();
			m_maxValue = std::numeric_limits<u64>::min();

			m_lastTick = nullptr;
			m_values.clear();
		}

		bool hasDataForTick(const ClientTick *tick) const { return m_values.find(tick) != m_values.end(); }

		u64 lastValue() const { return m_lastValue; }
		u64 totalTime(const ClientTick *tick) const {
			if (tick) {
				u64 sum = 0;
				for (auto &it : m_values.at(tick))
					sum += it;
				return sum;
			}

			return m_totalTime;
		}

		u64 minValue() const { return m_minValue; }
		u64 maxValue() const { return m_maxValue; }

		u64 lastTickCallCount() const { return m_lastTick ? m_values.at(m_lastTick).size() : 0; }
		u64 wholeSnapshotCallCount() const {
			u64 total = 0;
			for (auto &it : m_values)
				total += it.second.size();
			return total;
		}

	private:
		u64 m_lastValue = 0;
		u64 m_totalTime = 0;

		u64 m_minValue = std::numeric_limits<u64>::max();
		u64 m_maxValue = std::numeric_limits<u64>::min();

		const ClientTick *m_lastTick = nullptr;
		std::unordered_map<const ClientTick *, std::vector<u64>> m_values;
};

class DebugProfilerSnapshot {
	public:
		void reset() {
			m_tickDuration.clear();
			m_values.clear();
		}

		void addTick(const ClientTick &tick) {
			m_tickDuration.addDuration(&tick, tick.begin, tick.end);

			for (auto &[actionName, action] : tick.actions)
				for (auto &[start, end] : action.durations)
					m_values[actionName].addDuration(&tick, start, end);
		}

		const std::unordered_map<std::string, DebugProfilerSnapshotValue> &values() const { return m_values; }

		u64 tickCount() const { return m_tickDuration.wholeSnapshotCallCount(); }
		u64 totalTicksDuration() const { return m_tickDuration.totalTime(nullptr); }
		u64 previousTickDuration() const { return m_tickDuration.lastValue(); }
		u64 shortestPreviousTickDuration() const { return m_tickDuration.minValue(); }
		u64 longestPreviousTickDuration() const { return m_tickDuration.maxValue(); }

	private:
		DebugProfilerSnapshotValue m_tickDuration;

		std::unordered_map<std::string, DebugProfilerSnapshotValue> m_values;
};

class DebugProfilerWindow : public gk::Transformable, public gk::Drawable {
	public:
		DebugProfilerWindow();

		void onEvent(const SDL_Event &event);

		void update();

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		Text m_text;

		std::size_t m_lastUpdate = 0;

		DebugProfilerWindowMode m_mode = DebugProfilerWindowMode::Prev;

		DebugProfilerSnapshot m_snapshot;

		const ClientTick *m_lastTick = nullptr;
};

#endif // DEBUGPROFILERWINDOW_HPP_
