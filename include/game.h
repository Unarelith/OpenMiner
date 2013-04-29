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
#ifndef SCENE_H
#define SCENE_H

class Game {
	public:
		Game();
		~Game();
		
		void exec();
		
		void manageEvents();
		void animate();
		void draw();
		
		void lockMouse();
		void unlockMouse();
		
		static Player *player;
		static Map *map;
		
		static unsigned int mapWidth;
		static unsigned int mapDepth;
		static unsigned int mapHeight;
		
	private:
		bool m_cont;
		bool m_paused;
};

#endif // SCENE_H
