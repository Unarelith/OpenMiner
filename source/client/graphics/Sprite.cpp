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
#include <gk/core/Exception.hpp>

#include "Sprite.hpp"

Sprite::Sprite(const std::string &textureName, u16 frameWidth, u16 frameHeight, bool isAnimated) : Image() {
	load(textureName, frameWidth, frameHeight, isAnimated);
}

void Sprite::load(const Sprite &sprite) {
	Image::load(sprite);

	m_animations = sprite.m_animations;

	m_currentAnimation = sprite.m_currentAnimation;
	m_previousAnimation = sprite.m_previousAnimation;

	m_frameWidth = sprite.m_frameWidth;
	m_frameHeight = sprite.m_frameHeight;

	m_isAnimated = sprite.m_isAnimated;

	setCurrentFrame(sprite.m_currentFrame);
}

void Sprite::load(const std::string &textureName, u16 frameWidth, u16 frameHeight, bool isAnimated) {
	Image::load(textureName);

	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;

	setPosRect(0, 0, frameWidth, frameHeight);

	setCurrentFrame(0);

	m_isAnimated = isAnimated;
}

void Sprite::updateAnimations() {
	if (m_animations.size() > 0 && m_isAnimated) {
		if(m_currentAnimation >= m_animations.size()) {
			throw EXCEPTION("Trying to play inexistant animation:", m_currentAnimation, "| Animations:", m_animations.size());
		}

		m_animations[m_currentAnimation].play();

		setCurrentFrame(m_animations[m_currentAnimation].currentFrame());
	}
}

void Sprite::setCurrentFrame(u16 currentFrame) {
	u16 frameX = (currentFrame % (Image::width() / m_frameWidth)) * m_frameWidth;
	u16 frameY = (currentFrame / (Image::width() / m_frameWidth)) * m_frameHeight;

	setClipRect(frameX, frameY, m_frameWidth, m_frameHeight);

	m_currentFrame = currentFrame;
}

void Sprite::setCurrentAnimation(u16 currentAnimation) {
	if (m_previousAnimation != currentAnimation)
		m_animations[m_currentAnimation].reset();

	m_previousAnimation = m_currentAnimation;
	m_currentAnimation = currentAnimation;
}
