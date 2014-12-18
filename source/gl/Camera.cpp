/*
 * =====================================================================================
 *
 *       Filename:  Camera.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/12/2014 12:21:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include <SFML/Window/Keyboard.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

Camera::Camera() {
	m_x = 0;
	m_y = 32;
	m_z = 0;
	
	m_angleH = 45.0;
	m_angleV = -20.0;
	
	m_vx = 0;
	m_vz = 0;
	
	m_viewMatrix = update();
}

Camera::~Camera() {
}

void Camera::turnH(float angle) {
	m_angleH += angle;
	
	while(m_angleH >= 180.0) {
		m_angleH -= 360.0;
	}
	while(m_angleH < -180.0) {
		m_angleH += 360.0;
	}
}

void Camera::turnV(float angle) {
	m_angleV += angle;
	
	if(89.9 < m_angleV) {
		m_angleV = 89.9;
	}
	else if(-89.9 > m_angleV) {
		m_angleV = -89.9;
	}
}

void Camera::move(float direction) {
	direction += m_angleH;
	
	m_vx = 0.04f * cos(direction * M_PI / 180.0);
	m_vz = 0.04f * sin(direction * M_PI / 180.0);
	
	m_x += m_vx;
	m_z += m_vz;
	
	m_vx = 0;
	m_vz = 0;
}

glm::mat4 Camera::processInputs() {
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		turnV(1);
		m_viewMatrix = update();
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		turnV(-1);
		m_viewMatrix = update();
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		turnH(-0.8);
		m_viewMatrix = update();
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		turnH(0.8);
		m_viewMatrix = update();
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
		m_y -= 0.1;
		m_viewMatrix = update();
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
		m_y += 0.1;
		m_viewMatrix = update();
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		move(0.0f);
		m_viewMatrix = update();
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		move(180.0f);
		m_viewMatrix = update();
	}
	
	return m_viewMatrix;
}

glm::mat4 Camera::update() {
	return glm::lookAt(glm::vec3(m_x, m_y, m_z),
					   glm::vec3(pointTargetedX(), pointTargetedY(), pointTargetedZ()),
					   glm::vec3(0, 1, 0));
}

