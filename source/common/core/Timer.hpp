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
#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "common/core/IntTypes.hpp"

class Timer {
	public:
		Timer(bool useRealTime = false);

		void stop();
		void start();
		void reset();

		u32 time() const;
		void setTime(u32 time);

		bool isStarted() const { return m_isStarted; }

	private:
		bool m_useRealTime = false;

		bool m_isStarted = false;

		u32 m_t = 0;
		u32 m_tick = 0;
};

#endif // TIMER_HPP_
