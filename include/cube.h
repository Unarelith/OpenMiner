#ifndef CUBE_H
#define CUBE_H

class Cube {
	public:
		Cube(float x, float y, float z, GLuint texture, Map *map);
		~Cube() {}
		
		void draw(float x, float y, float z);
		
	private:
		float m_x;
		float m_y;
		float m_z;
		
		GLuint m_texture;
		
		Map *m_map;
};

#endif // CUBE_H
