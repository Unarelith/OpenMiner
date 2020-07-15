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
#ifndef CELESTIALOBJECT_HPP_
#define CELESTIALOBJECT_HPP_

#include <gk/gl/Drawable.hpp>
#include <gk/gl/Transformable.hpp>
#include <gk/gl/VertexBuffer.hpp>
#include <gk/graphics/Color.hpp>

class CelestialObject : public gk::Drawable, public gk::Transformable  {
	public:
		CelestialObject();

		float width() const { return m_width; }
		float height() const { return m_height; }

		void setColor(const gk::Color &color) { m_color = color; m_isUpdateNeeded = true; }
		void setSize(float width, float height) { m_width = width; m_height = height; m_isUpdateNeeded = true; }
		void setTexture(const std::string &textureName);
		void setPhaseCount(u16 phaseCount, u16 phaseSize) { m_phaseCount = phaseCount; m_phaseSize = phaseSize; m_isUpdateNeeded = true; }
		void setCurrentPhase(u16 currentPhase) { m_currentPhase = currentPhase; m_isUpdateNeeded = true; }
		void setRotationOffset(u16 rotationOffset) { m_rotationOffset = rotationOffset; }
		void setRotationAxis(const gk::Vector3f &rotationAxis) { m_rotationAxis = rotationAxis; }

	private:
		void updateVertexBuffer() const;

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::VertexBuffer m_vbo;

		gk::Color m_color = gk::Color::White;

		float m_width = 0.f;
		float m_height = 0.f;

		const gk::Texture *m_texture = nullptr;

		mutable bool m_isUpdateNeeded = true;

		u16 m_phaseCount = 0;
		u16 m_phaseSize = 0;
		u16 m_currentPhase = 0;

		u16 m_rotationOffset = 0;
		gk::Vector3f m_rotationAxis{0, 1, 0};
};

#endif // CELESTIALOBJECT_HPP_
