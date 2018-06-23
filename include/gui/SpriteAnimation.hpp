/*
 * =====================================================================================
 *
 *       Filename:  SpriteAnimation.hpp
 *
 *    Description:
 *
 *        Created:  25/01/2018 04:07:17
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SPRITEANIMATION_HPP_
#define SPRITEANIMATION_HPP_

#include <vector>

#include "Timer.hpp"

class SpriteAnimation {
	public:
		SpriteAnimation(u16 delay, bool isRepeated = true)
			: m_delay(delay), m_isRepeated(isRepeated) {}

		void reset(u16 frameID = 0);

		void start();
		void stop();

		void play();

		void addFrame(u16 frameID) { m_frames.emplace_back(frameID); }
		u16 getFrame(u16 frameID) const;
		u16 currentFrame() const;
		u16 displayedFramesAmount() const;

		bool isPlaying() const { return m_timer.isStarted() && !m_isPaused; }
		bool isFinished() const { return displayedFramesAmount() >= m_frames.size(); }

		u32 size() const { return m_frames.size(); }

		u16 delay() const { return m_delay; }

		void setRepeated(bool isRepeated) { m_isRepeated = isRepeated; }

	private:
		std::vector<u16> m_frames;

		Timer m_timer;

		u16 m_delay = 0;

		bool m_isPaused = false;
		bool m_isRepeated = true;
};

#endif // SPRITEANIMATION_HPP_
