/*---------------------------------------------------------------------------------
	
	KubKraft
	Copyright (C) 2012 Quent42340 <quent42340@gmail.com>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
---------------------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <map>
#include <unordered_map>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "config.h"
#include "types.h"
#include "mapManager.h"
#include "cube.h"
#include "chunk.h"
#include "map.h"
#include "player.h"
#include "game.h"

using namespace std;

Player::Player(float x, float y, float z, float angle) {
	m_x = x;
	m_y = y;
	
	m_eyeheight = z + PLAYER_HEIGHT - 1;
	
	m_angleH = angle;
	m_angleV = 0.0;
	
	m_isJumping = false;
	
	m_speed = vect3D(0, 0, 0);
}

void Player::testPoint(vect3D pos, vect3D *speed, bool playerFoots) {
	if(!passable(pos.x + speed->x, pos.y, pos.z)) speed->x = 0;
	if(!passable(pos.x, pos.y + speed->y, pos.z)) speed->y = 0;
	if(!passable(pos.x, pos.y, pos.z + speed->z)) {
		if(speed->z < 0 && m_isJumping) m_isJumping = false;
		speed->z = 0;
	}
}

void Player::move(float distance, float direction) {
	direction += m_angleH;
	
	m_speed.x = distance * cos(direction * M_PI / 180.0);
	m_speed.y = distance * sin(direction * M_PI / 180.0);
}

void Player::update() {
	testPoint(vect3D(m_x - 0.2, m_y - 0.2, m_eyeheight - PLAYER_HEIGHT + 0.2), &m_speed, true);
	testPoint(vect3D(m_x + 0.2, m_y - 0.2, m_eyeheight - PLAYER_HEIGHT + 0.2), &m_speed, true);
	testPoint(vect3D(m_x - 0.2, m_y + 0.2, m_eyeheight - PLAYER_HEIGHT + 0.2), &m_speed, true);
	testPoint(vect3D(m_x + 0.2, m_y + 0.2, m_eyeheight - PLAYER_HEIGHT + 0.2), &m_speed, true);
	testPoint(vect3D(m_x - 0.2, m_y - 0.2, m_eyeheight + (2 - PLAYER_HEIGHT - 0.01)), &m_speed);
	testPoint(vect3D(m_x + 0.2, m_y - 0.2, m_eyeheight + (2 - PLAYER_HEIGHT - 0.01)), &m_speed);
	testPoint(vect3D(m_x - 0.2, m_y + 0.2, m_eyeheight + (2 - PLAYER_HEIGHT - 0.01)), &m_speed);
	testPoint(vect3D(m_x + 0.2, m_y + 0.2, m_eyeheight + (2 - PLAYER_HEIGHT - 0.01)), &m_speed);
	
	m_x += m_speed.x;
	m_y += m_speed.y;
	m_eyeheight += m_speed.z;
	
	m_speed.x = 0;
	m_speed.y = 0;
	
	//m_speed.z = 0;
}

void Player::jump() {
	m_speed.z -= GRAVITY;
}

void Player::fly() {
	m_speed.z = FLY_SPEED;
}

void Player::land() {
	m_speed.z = -FLY_SPEED;
}

void Player::turnH(float angle) {
	m_angleH += angle;
	
	while(m_angleH >= 180.0) {
		m_angleH -= 360.0;
	}
	while(m_angleH < -180.0) {
		m_angleH += 360.0;
	}
}

void Player::turnV(float angle) {
	m_angleV += angle;
	
	if(89.9 < m_angleV) {
		m_angleV = 89.9;
	}
	else if(-89.9 > m_angleV) {
		m_angleV = -89.9;
	}
}

void Player::watch() {
	gluLookAt(
		// Eye position
		m_x, m_y, m_eyeheight,
		
		// Point targeted
		pointTargetedx(),
		pointTargetedy(),
		pointTargetedz(),
		
		// z is the vertical
		0, 0, 1);
}

