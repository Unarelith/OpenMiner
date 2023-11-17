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
#include <map>
#include <sstream>

#include "client/hud/DebugProfilerWindow.hpp"

DebugProfilerWindow::DebugProfilerWindow() {
#ifndef OM_PROFILER_ENABLED
	m_text.setString("Profiler disabled.\nPlease recompile with -DOM_PROFILER_ENABLED to enable it.");
#endif
}

void DebugProfilerWindow::onEvent(const SDL_Event &event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
			case SDLK_a: m_mode = DebugProfilerWindowMode::Total; break;
			case SDLK_p: m_mode = DebugProfilerWindowMode::Prev; break;
			case SDLK_s: m_mode = DebugProfilerWindowMode::Min; break;
			case SDLK_l: m_mode = DebugProfilerWindowMode::Max; break;
		}
	}
}

void DebugProfilerWindow::update() {
#ifdef OM_PROFILER_ENABLED
	ClientProfiler &profiler = ClientProfiler::getInstance();
	std::size_t ticks = profiler.getTickCount();

	// FIXME: Some information may be lost here
	const ClientTick &tick = profiler.getLastTick();
	if (&tick == m_lastTick) return;
	m_lastTick = &tick;

	m_snapshot.addTick(tick);

	constexpr std::size_t updateDisplayFrequency = 60;
	if (ticks - m_lastUpdate >= updateDisplayFrequency) {
		m_lastUpdate = ticks;

		std::stringstream stream;
		stream << "In the last " << updateDisplayFrequency << " ticks:\n";
		stream << "Tick count: " << m_snapshot.tickCount() << "\n";
		stream << "- [A] Total: " << (float)m_snapshot.totalTicksDuration() / 1000.f << " ms ("
			<< (float)m_snapshot.totalTicksDuration() / 1000.0f / updateDisplayFrequency << " ms/t)\n";

		stream << "- [P] Prev: " << (float)m_snapshot.previousTickDuration() / 1000.f << " ms\n";
		stream << "- [S] Min: " << (float)m_snapshot.shortestPreviousTickDuration() / 1000.f << " ms\n";
		stream << "- [L] Max: " << (float)m_snapshot.longestPreviousTickDuration() / 1000.f << " ms\n";

		if (m_mode == DebugProfilerWindowMode::Total)
			stream << "Total actions during those ticks:\n";
		else if (m_mode == DebugProfilerWindowMode::Prev)
			stream << "Actions for last tick:\n";
		else if (m_mode == DebugProfilerWindowMode::Min)
			stream << "Actions for shortest tick since last update:\n";
		else if (m_mode == DebugProfilerWindowMode::Max)
			stream << "Actions for longest tick since last update:\n";

		std::map<u64, std::pair<std::string, const DebugProfilerSnapshotValue &>> snapshotData;
		for (auto &[valueName, value] : m_snapshot.values()) {
			if (value.hasDataForTick(&tick) || m_mode == DebugProfilerWindowMode::Total) {
				std::pair<std::string, const DebugProfilerSnapshotValue &> pair{valueName, value};
				if (m_mode == DebugProfilerWindowMode::Total)
					snapshotData.emplace(value.totalTime(nullptr), pair);
				else
					snapshotData.emplace(value.totalTime(&tick), pair);
			}
		}

		for (auto &[displayedValue, value] : snapshotData) {
			stream << "- " << value.first << " (";
			if (m_mode == DebugProfilerWindowMode::Total)
				stream << value.second.wholeSnapshotCallCount();
			else
				stream << value.second.lastTickCallCount();
			stream << "): " << (float)displayedValue / 1000.f << " ms ("
				<< (float)value.second.totalTime(nullptr) / 1000.f / updateDisplayFrequency << " ms/t)\n";
		}

		m_text.setString(stream.str());

		m_snapshot.reset();
	}
#endif
}

void DebugProfilerWindow::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_text, states);
}
