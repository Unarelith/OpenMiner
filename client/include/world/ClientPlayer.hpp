/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef CLIENTPLAYER_HPP_
#define CLIENTPLAYER_HPP_

#include <cmath>

#include <glm/glm.hpp>

#include <gk/gl/Camera.hpp>

#include "Player.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef RADIANS_PER_DEGREES
#define RADIANS_PER_DEGREES (M_PI / 180.0f)
#endif

class ClientWorld;

class ClientPlayer : public Player {
	public:
		ClientPlayer(gk::Camera &camera);

		void turnH(float angle);
		void turnV(float angle);

		void move(float direction);

		void processInputs();
		void updatePosition(const ClientWorld &world);

		void checkCollisions(const ClientWorld &world);

		float pointTargetedX() const { return m_x + cos(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedY() const { return m_y + sin(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES) - 0.00001; }
		float pointTargetedZ() const { return m_z + sin(m_angleV * RADIANS_PER_DEGREES); }

		static ClientPlayer &getInstance() { return *s_instance; }
		static void setInstance(ClientPlayer *instance) { s_instance = instance; }

		float x() const { return m_x; }
		float y() const { return m_y; }
		float z() const { return m_z; }

		void setPosition(float x, float y, float z);

		const gk::Camera &camera() { return m_camera; }

	private:
		void testPoint(const ClientWorld &world, float x, float y, float z, glm::vec3 &speed);

		static ClientPlayer *s_instance;

		gk::Camera &m_camera;

		float m_x;
		float m_y;
		float m_z;

		float m_angleH;
		float m_angleV;

		glm::vec3 m_velocity{0};
		bool m_isJumping = false;

		const float m_gravity = 0.001;
		const float m_jumpSpeed = 0.06f;
};

#endif // CLIENTPLAYER_HPP_
