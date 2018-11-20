/*
 * =====================================================================================
 *
 *       Filename:  Camera.hpp
 *
 *    Description:
 *
 *        Created:  16/12/2014 12:21:03
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <cmath>

#include <glm/glm.hpp>

#undef M_PI
#define M_PI 3.14159265358979323846
#ifndef RADIANS_PER_DEGREES
#define RADIANS_PER_DEGREES (M_PI / 180.0f)
#endif

class World;

class Camera {
	public:
		Camera();

		void turnH(float angle);
		void turnV(float angle);

		void move(float direction);

		void processInputs();
		glm::mat4 updatePosition(const World &world);

		void checkCollisions(const World &world);
		void updateViewMatrix();

		float pointTargetedX() const { return m_x + cos(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedY() const { return m_y + sin(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedZ() const { return m_z + sin(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES) - 0.00001; }

		static Camera &getInstance() {
			static Camera instance;
			return instance;
		}

		float x() const { return m_x; }
		float y() const { return m_y; }
		float z() const { return m_z; }

	private:
		void testPoint(const World &world, glm::vec3 pos, glm::vec3 &speed);

		glm::mat4 m_viewMatrix;

		float m_x;
		float m_y;
		float m_z;

		float m_angleH;
		float m_angleV;

		glm::vec3 m_velocity{0};
		bool m_isJumping = false;

		const float m_gravity = 0.001;
		const float m_jumpSpeed = 0.07f;
};

#endif // CAMERA_HPP_
