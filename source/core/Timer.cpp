/*
 * =====================================================================================
 *
 *       Filename:  Timer.cpp
 *
 *    Description:
 *
 *        Created:  15/09/2014 19:31:35
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#include "GameClock.hpp"
#include "Timer.hpp"

Timer::Timer(bool useRealTime) {
	m_useRealTime = useRealTime;
	m_t = GameClock::getTicks(m_useRealTime);
}

void Timer::stop() {
	if(m_isStarted) {
		m_isStarted = false;
		m_tick = GameClock::getTicks(m_useRealTime) - m_t;
	}
}

void Timer::start() {
	if(!m_isStarted) {
		m_isStarted = true;
		m_t = GameClock::getTicks(m_useRealTime) - m_tick;
	}
}

void Timer::reset() {
	m_isStarted = false;
	m_t = GameClock::getTicks(m_useRealTime);
	m_tick = 0;
}

u32 Timer::time() const {
	if(m_isStarted) {
		return GameClock::getTicks(m_useRealTime) - m_t;
	} else {
		return m_tick;
	}
}

void Timer::setTime(u32 time) {
	if(m_isStarted) {
		m_t = GameClock::getTicks(m_useRealTime) - time;
	} else {
		m_tick = time;
	}
}

