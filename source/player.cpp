#include <string>
#include <cmath>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "init.h"
#include "types.h"
#include "player.h"

Player::Player(float x, float y, float z, float angle) {
	m_x = x;
	m_y = y;
	m_z = z;
	m_angleH = angle;
	m_angleV = 0.0f;
	m_eyeheight = 0.8;
}

void Player::move(float distance, float direction) {
	direction += m_angleH;
	
	m_y -= distance * sin(direction * M_PI / 180.0);
	m_x -= distance * cos(direction * M_PI / 180.0);
}

void Player::turnH(float angle) {
	m_angleH += angle;
	
	while(m_angleH >= 180.0f) {
		m_angleH -= 360.0f;
	}
	while(m_angleH < -180.0f) {
		m_angleH += 360.0f;
	}
}

void Player::turnV(float angle) {
	m_angleV += angle;
	
	if(45.0f < m_angleV) {
		m_angleV = 45.0f;
	}
	else if(-45.0f > m_angleV) {
		m_angleV = -45.0f;
	}
}

void Player::fly() {
	if(m_eyeheight < 20) m_eyeheight += 0.05;
}

void Player::land() {
	if(m_eyeheight > 0.8) m_eyeheight -= 0.05;
}

void Player::watch() {
	#define RADIANS_PER_DEGREES 0.0174532925199
	
	gluLookAt(
			// Eye position
			m_x, m_y, m_eyeheight,
			
			// Point targeted
			m_x - cos(-m_angleH * RADIANS_PER_DEGREES),
			m_y + sin(-m_angleH * RADIANS_PER_DEGREES),
			m_eyeheight + tan(m_angleV * RADIANS_PER_DEGREES),
			
			// z is the vertical
			0, 0, 1);
}

