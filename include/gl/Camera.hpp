/*
 * =====================================================================================
 *
 *       Filename:  Camera.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/12/2014 12:21:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <cmath>

#include <glm/glm.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define RADIANS_PER_DEGREES 0.0174532925199

class Camera {
	public:
		Camera();
		~Camera();
		
		void turnH(float angle);
		void turnV(float angle);
		
		void move(float direction);
		
		glm::mat4 processInputs();
		glm::mat4 update();
		
		float pointTargetedX() { return m_x + cos(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedY() { return m_y + sin(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedZ() { return m_z + sin(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES) - 0.00001; }
		
	private:
		glm::mat4 m_viewMatrix;
		
		float m_x;
		float m_y;
		float m_z;
		
		float m_angleH;
		float m_angleV;
		
		float m_vx;
		float m_vz;
};

#endif // CAMERA_HPP_
