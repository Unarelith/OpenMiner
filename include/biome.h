#ifndef BIOME_H
#define BIOME_H

class Biome {
	public:
		Biome(int x, int y, int z, GLuint texture);
		~Biome();
		
		void draw();
		
		int x() const { return m_x; }
		int y() const { return m_y; }
		int z() const { return m_z; }
		
	private:
		int m_x;
		int m_y;
		int m_z;
		
		std::vector<Cube*> m_cubes;
};

#endif // BIOME_H
