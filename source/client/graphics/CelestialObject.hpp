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

#include "Color.hpp"
#include "Drawable.hpp"
#include "Transformable.hpp"
#include "VertexBuffer.hpp"

class CelestialObject : public Drawable, public Transformable  {
	public:
		CelestialObject();

		float width() const { return m_width; }
		float height() const { return m_height; }

		void setColor(const Color &color) { m_color = color; m_isUpdateNeeded = true; }
		void setSize(float width, float height) { m_width = width; m_height = height; m_isUpdateNeeded = true; }
		void setTexture(const std::string &textureName);
		void setPhaseCount(u16 phaseCount, u16 phaseSize) { m_phaseCount = phaseCount; m_phaseSize = phaseSize; m_isUpdateNeeded = true; }
		void setCurrentPhase(u16 currentPhase) const { if (m_currentPhase != currentPhase) { m_currentPhase = currentPhase; m_isUpdateNeeded = true; } }
		void setRotationOffset(u16 rotationOffset) { m_rotationOffset = rotationOffset; }
		void setRotationSpeed(float rotationSpeed) { m_rotationSpeed = rotationSpeed; }
		void setRotationAxis(const Vector3f &rotationAxis) { m_rotationAxis = rotationAxis; m_axisXfNeedsUpdate = true; }

	private:
		void updateVertexBuffer() const;
		void updateAxisTransform() const;

		void draw(RenderTarget &target, RenderStates states) const override;

		mutable VertexBuffer m_vbo;

		Color m_color = Color::White;

		float m_width = 0.f;
		float m_height = 0.f;

		const Texture *m_texture = nullptr;

		mutable bool m_isUpdateNeeded = true;
		mutable bool m_axisXfNeedsUpdate = true;

		u16 m_phaseCount = 0;
		u16 m_phaseSize = 0;
		mutable u16 m_currentPhase = 0;

		u16 m_rotationOffset = 0;
		float m_rotationSpeed = 1.f;
		Vector3f m_rotationAxis{0, 1, 0};
		mutable glm::mat4 m_rotAxisTransform;
};

#endif // CELESTIALOBJECT_HPP_
