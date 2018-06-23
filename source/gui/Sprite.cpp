/*
 * =====================================================================================
 *
 *       Filename:  Sprite.cpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:48:08
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Exception.hpp"
#include "Sprite.hpp"

Sprite::Sprite(const std::string &textureName, u16 frameWidth, u16 frameHeight, bool isAnimated) : Image(textureName) {
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;

	// setPosRect(0, 0, frameWidth, frameHeight);

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

