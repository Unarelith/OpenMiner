/*
 * =====================================================================================
 *
 *       Filename:  SpriteAnimation.cpp
 *
 *    Description:
 *
 *        Created:  25/01/2018 04:07:30
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "SpriteAnimation.hpp"

void SpriteAnimation::reset(u16 frameID) {
	m_timer.reset();
	m_timer.setTime(frameID * m_delay);
}

void SpriteAnimation::start() {
	m_timer.start();

	// if(!Sprite::pause) m_isPaused = false;
}

void SpriteAnimation::stop() {
	m_timer.stop();

	// if(!Sprite::pause) m_isPaused = true;
}

void SpriteAnimation::play() {
	if(/* Sprite::pause ||  */m_isPaused) {
		stop();
	} else {
		start();
	}

	if(isFinished()) {
		if(m_isRepeated) {
			reset();
			start();
		} else {
			stop();
		}
	}
}

u16 SpriteAnimation::getFrame(u16 frameID) const {
	return m_frames[frameID];
}

u16 SpriteAnimation::currentFrame() const {
	u16 frameID = displayedFramesAmount();
	if(frameID >= m_frames.size()) {
		return getFrame(m_frames.size() - 1);
	} else {
		return getFrame(frameID);
	}
}

u16 SpriteAnimation::displayedFramesAmount() const {
	if (m_delay == 0)
		return 0;

	return m_timer.time() / m_delay;
}

