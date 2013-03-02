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

#include "init.h"
#include "types.h"
#include "player.h"

using namespace std;

Player::Player(float x, float y, float z, float angle) {
	m_x = x;
	m_y = y;
	
	m_eyeheight = 0.8 + z;
	
	m_angleH = angle;
	m_angleV = 0.0f;
}

void Player::move(float distance, float direction) {
	direction += m_angleH;
	
	m_y += distance * sin(direction * M_PI / 180.0);
	m_x += distance * cos(direction * M_PI / 180.0);
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
	
	if(89.9f < m_angleV) {
		m_angleV = 89.9f;
	}
	else if(-89.9f > m_angleV) {
		m_angleV = -89.9f;
	}
}

void Player::fly() {
	if(m_eyeheight < 256) m_eyeheight += 0.05;
}

void Player::land() {
	/*if(m_eyeheight > 0.8)*/ m_eyeheight -= 0.05;
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

