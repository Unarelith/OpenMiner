#ifndef CUBE_H
#define CUBE_H

class Cube {
	public:
		Cube(int x, int y, int z, GLuint texture, Map *map);
		~Cube() {}
		
		void draw();
		
	private:
		int m_x;
		int m_y;
		int m_z;
		
		GLuint m_texture;
		
		Map *m_map;
};

#endif // CUBE_H
