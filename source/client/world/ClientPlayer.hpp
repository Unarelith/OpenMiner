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
		void turnViewV(float angle);

		void updateCamera();

		void move(float direction);

		void processInputs();
		void updatePosition(const ClientWorld &world);

		void checkCollisions(const ClientWorld &world);

		float dirTargetedX() const { return m_forwardDir.x; }
		float dirTargetedY() const { return m_forwardDir.y; }
		float dirTargetedZ() const { return m_forwardDir.z; }

		static ClientPlayer &getInstance() { return *s_instance; }
		static void setInstance(ClientPlayer *instance) { s_instance = instance; }

		void setPosition(double x, double y, double z);
		void setCameraRoll(float angle) { m_viewAngleRoll = angle; updateCamera(); };

		gk::Camera &camera() { return m_camera; }

	private:
		void testPoint(const ClientWorld &world, double x, double y, double z, glm::vec3 &vel);

		static ClientPlayer *s_instance;

		gk::Camera &m_camera;

		gk::Vector3f m_forwardDir;

		gk::Vector3f m_cameraLocalPos;

		glm::vec3 m_velocity{0.f};
		bool m_isJumping = false;

		const float m_jumpSpeed = 0.06f;

		float m_inertia = 0;

		float m_oldInertia = 0;
};

#endif // CLIENTPLAYER_HPP_
