/*
 * =====================================================================================
 *
 *       Filename:  Player.hpp
 *
 *    Description:
 *
 *        Created:  27/06/2018 04:56:19
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <cmath>

#include <glm/glm.hpp>

#include <gk/gl/Camera.hpp>

#include "Inventory.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef RADIANS_PER_DEGREES
#define RADIANS_PER_DEGREES (M_PI / 180.0f)
#endif

class ClientWorld;

class Player {
	public:
		Player(gk::Camera &camera);

		void turnH(float angle);
		void turnV(float angle);

		void move(float direction);

		void processInputs();
		void updatePosition(const ClientWorld &world);

		void checkCollisions(const ClientWorld &world);

		float pointTargetedX() const { return m_x + cos(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedY() const { return m_y + sin(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedZ() const { return m_z + sin(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES) - 0.00001; }

		static Player &getInstance() { return *s_instance; }
		static void setInstance(Player *instance) { s_instance = instance; }

		float x() const { return m_x; }
		float y() const { return m_y; }
		float z() const { return m_z; }

		Inventory &inventory() { return m_inventory; }

		const gk::Camera &camera() { return m_camera; }

	private:
		void testPoint(const ClientWorld &world, glm::vec3 pos, glm::vec3 &speed);

		static Player *s_instance;

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

		Inventory m_inventory{9, 4};
};

#endif // PLAYER_HPP_
