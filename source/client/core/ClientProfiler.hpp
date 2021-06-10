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
#ifndef CLIENTPROFILER_HPP_
#define CLIENTPROFILER_HPP_

#include <chrono>
#include <deque>
#include <string>
#include <unordered_map>

#include <gk/core/IntTypes.hpp>

#ifdef OM_PROFILER_ENABLED
# define OM_PROFILE_START(name) ClientProfiler::getInstance().startAction(name);
# define OM_PROFILE_END(name) ClientProfiler::getInstance().endAction(name);

# define OM_PROFILE_BEGIN_TICK() ClientProfiler::getInstance().onBeginTick();
# define OM_PROFILE_END_TICK() ClientProfiler::getInstance().onEndTick();
#else
# define OM_PROFILE_START(name) {}
# define OM_PROFILE_END(name) {}

# define OM_PROFILE_BEGIN_TICK() {}
# define OM_PROFILE_END_TICK() {}
#endif

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

struct ClientAction {
	std::deque<std::pair<TimePoint, TimePoint>> durations;
};

struct ClientTick {
	TimePoint begin;
	TimePoint end;
	std::unordered_map<std::string, ClientAction> actions;
};

class ClientProfiler {
	public:
		void onBeginTick();
		void onEndTick();

		void dump(u64 tickDurationMin = 0);

		void startAction(const std::string &name);
		void endAction(const std::string &name);

		// FIXME: m_ticks.size() - 1 is actually the current tick
		const ClientTick &getLastTick() const { return m_ticks.at(m_ticks.size() - 2); };

		std::size_t getTickCount() const { return m_ticks.size(); }

		static ClientProfiler &getInstance() { return *s_instance; }
		static void setInstance(ClientProfiler *instance) { s_instance = instance; }

		static u64 tickCount;

	private:
		static ClientProfiler *s_instance;

		std::deque<ClientTick> m_ticks; // TODO: Use a std::vector
};

#endif // CLIENTPROFILER_HPP_
