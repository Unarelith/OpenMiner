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

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "config.h"
#include "types.h"
#include "mapManager.h"
#include "player.h"

using namespace std;

Player::Player(float x, float y, float z, float angle) {
	m_x = x;
	m_y = y;
	
	m_eyeheight = z + 0.1;
	
	m_angleH = angle;
	m_angleV = 0.0;
	
	m_jumpSpeed = 0.0;
	m_isJumping = false;
}

void Player::move(float distance, float direction) {
	direction += m_angleH;
	
	float vx = distance * cos(direction * M_PI / 180.0);
	if((passable(m_x + ((vx > 0) ? vx + 0.2 : vx - 0.2) , m_y, m_eyeheight)) &&
	   (passable(m_x + ((vx > 0) ? vx + 0.2 : vx - 0.2) , m_y, m_eyeheight - PLAYER_HEIGHT + 0.3))) {
		m_x += vx;
	}
	
	float vy = distance * sin(direction * M_PI / 180.0);
	if((passable(m_x, m_y + ((vy > 0) ? vy + 0.2 : vy - 0.2), m_eyeheight)) &&
	   (passable(m_x, m_y + ((vy > 0) ? vy + 0.2 : vy - 0.2), m_eyeheight - PLAYER_HEIGHT + 0.3))) {
		m_y += vy;
	}
}

void Player::jump() {
	if(m_isJumping) {
		m_eyeheight += m_jumpSpeed / 4;
		
		m_jumpSpeed -= GRAVITY;
		
		if((m_jumpSpeed < 0) &&
		   ((!passable(m_x, m_y, m_eyeheight - PLAYER_HEIGHT - m_jumpSpeed / 4 - 0.1)) /* ||
			(!passable(m_x + 0.25, m_y, m_eyeheight - PLAYER_HEIGHT - m_jumpSpeed / 4 - 0.3)) ||
			(!passable(m_x, m_y + 0.25, m_eyeheight - PLAYER_HEIGHT - m_jumpSpeed / 4 - 0.3)) ||
		    (!passable(m_x + 0.25, m_y + 0.25, m_eyeheight - PLAYER_HEIGHT - m_jumpSpeed / 4 - 0.3)) */ )) {
			m_jumpSpeed = 0.0;
			m_isJumping = false;
			return;
		}
		
		if((m_jumpSpeed >= 0) &&
		   ((!passable(m_x, m_y, m_eyeheight + m_jumpSpeed / 4 + 0.5)) /* ||
		    (!passable(m_x + 0.25, m_y, m_eyeheight + m_jumpSpeed / 4 + 0.3)) ||
		    (!passable(m_x, m_y + 0.25, m_eyeheight + m_jumpSpeed / 4 + 0.3)) ||
		    (!passable(m_x + 0.25, m_y + 0.25, m_eyeheight + m_jumpSpeed / 4 + 0.3)) */ )) {
			m_jumpSpeed = 0.0;
		}
	}
	else if((passable(m_x, m_y, m_eyeheight - PLAYER_HEIGHT - m_jumpSpeed / 4 - 0.1)) &&
			(passable(m_x, m_y, m_eyeheight - PLAYER_HEIGHT - m_jumpSpeed / 4 - 0.1))) {
		m_jumpSpeed = 0.0;
		m_isJumping = true;
	}
	
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

void Player::fly() {
	if(m_eyeheight < 256) m_eyeheight += FLY_SPEED;
}

void Player::land() {
	m_eyeheight -= FLY_SPEED;
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
	
	//cout << "Eyepos: (" << int(m_x) << " ; " << int(m_y) << " ; " << int(m_eyeheight) << ")" << endl;
	//cout << "Pt targeted: (" << pointTargetedx() << " ; " << pointTargetedy() << " ; " << pointTargetedz() << ")" << endl;
}

