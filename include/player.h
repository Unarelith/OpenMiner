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
#ifndef PLAYER_H
#define PLAYER_H

#define RADIANS_PER_DEGREES 0.0174532925199

class Player {
	public:
		Player(float x, float y, float z, float angle);
		
		void move(float distance, float direction);
		void turnH(float angle);
		void turnV(float angle);
		void fly();
		void land();
		
		void watch();
		
		float x() const { return m_x; }
		float y() const { return m_y; }
		float z() const { return m_eyeheight; }
		
		//float pointTargetedx() const { return m_x - cos(-m_angleH * RADIANS_PER_DEGREES); }
		//float pointTargetedy() const { return m_y + sin(-m_angleH * RADIANS_PER_DEGREES); }
		//float pointTargetedz() const { return m_eyeheight + tan(m_angleV * RADIANS_PER_DEGREES); }
		
		float pointTargetedx() { return m_x + cos(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedy() { return m_y + sin(m_angleH * RADIANS_PER_DEGREES) * cos(m_angleV * RADIANS_PER_DEGREES); }
		float pointTargetedz() { return m_eyeheight + sin(m_angleV * RADIANS_PER_DEGREES); }
		
	private:
		float m_x;
		float m_y;
		
		float m_eyeheight;
		
		float m_angleH;
		float m_angleV;
};

#endif // PLAYER_H
