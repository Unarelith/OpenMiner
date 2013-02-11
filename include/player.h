#ifndef PLAYER_H
#define PLAYER_H

class Player {
	public:
		Player(float x, float y, float z, float angle);
		
		void move(float distance, float direction);
		void turnH(float angle);
		void turnV(float angle);
		void fly();
		void land();
		
		void watch();
		
	private:
		float m_x;
		float m_y;
		float m_z;
		
		float m_angleH;
		float m_angleV;
		
		float m_eyeheight;
};

#endif // PLAYER_H
