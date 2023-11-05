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
		return getFrame((u16)m_frames.size() - 1);
	} else {
		return getFrame(frameID);
	}
}

u16 SpriteAnimation::displayedFramesAmount() const {
	if (m_delay == 0)
		return 0;

	return u16(m_timer.time() / m_delay);
}
