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

#include <deque>
#include <string>
#include <unordered_map>

#include <gk/core/IntTypes.hpp>

struct ClientAction {
	std::deque<std::pair<u64, u64>> durations;
};

struct ClientTick {
	u64 begin;
	u64 end;
	std::unordered_map<std::string, ClientAction> actions;
};

class ClientProfiler {
	public:
		void onBeginTick();
		void onEndTick();

		void dump(u64 tickDurationMin = 0);

		void startAction(const std::string &name);
		void endAction(const std::string &name);

		static ClientProfiler &getInstance() { return *s_instance; }
		static void setInstance(ClientProfiler *instance) { s_instance = instance; }

	private:
		static ClientProfiler *s_instance;

		std::deque<ClientTick> m_ticks;
};

#endif // CLIENTPROFILER_HPP_
