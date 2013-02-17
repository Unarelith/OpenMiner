#ifndef CUBE_H
#define CUBE_H

class Cube {
	public:
		Cube(int x, int y, int z, GLuint texture, Map *map);
		~Cube() {}
		
		void draw();
		
		void setSelected(bool selected) { m_selected = selected; };
		
		int x() const { return m_x; }
		int y() const { return m_y; }
		int z() const { return m_z; }
		
	private:
		int m_x;
		int m_y;
		int m_z;
		
		GLuint m_texture;
		
		Map *m_map;
		
		bool m_selected;
};

#endif // CUBE_H
