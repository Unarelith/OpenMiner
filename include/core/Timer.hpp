/*
 * =====================================================================================
 *
 *       Filename:  Timer.hpp
 *
 *    Description:
 *
 *        Created:  15/09/2014 19:31:28
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "IntTypes.hpp"

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
