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
#ifndef SPRITE_HPP_
#define SPRITE_HPP_

#include "Image.hpp"
#include "SpriteAnimation.hpp"

class Sprite : public Image {
	public:
		Sprite() : Image() {}
		Sprite(const std::string &textureName, u16 frameWidth, u16 frameHeight, bool isAnimated = false);

		void load(const Sprite &sprite);
		void load(const std::string &textureName, u16 frameWidth, u16 frameHeight, bool isAnimated = false);

		void updateAnimations();

		void addAnimation(const SpriteAnimation &animation) { m_animations.emplace_back(animation); }

		u16 currentFrame() const { return m_currentFrame; }

		u16 frameWidth() const { return m_frameWidth; }
		u16 frameHeight() const { return m_frameHeight; }

		bool hasAnimations() { return m_animations.size() != 0; }

		SpriteAnimation &currentAnimation() { return m_animations.at(m_currentAnimation); }
		const SpriteAnimation &currentAnimation() const { return m_animations.at(m_currentAnimation); }

		SpriteAnimation &getAnimation(std::size_t i) { return m_animations.at(i); }
		const SpriteAnimation &getAnimation(std::size_t i) const { return m_animations.at(i); }

		void setCurrentFrame(u16 currentFrame);
		void setCurrentAnimation(u16 currentAnimation);

		bool isAnimated() const { return m_isAnimated; }
		void setAnimated(bool isAnimated) { m_isAnimated = isAnimated; }

	private:
		std::vector<SpriteAnimation> m_animations;

		u16 m_currentFrame = 0;
		u16 m_currentAnimation = 0;
		u16 m_previousAnimation = 0;

		u16 m_frameWidth = 0;
		u16 m_frameHeight = 0;

		bool m_isAnimated = false;
};

#endif // SPRITE_HPP_
