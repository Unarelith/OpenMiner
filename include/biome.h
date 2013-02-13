#ifndef BIOME_H
#define BIOME_H

class Biome {
	public:
		Biome(int x, int y, int z, GLuint texture);
		~Biome();
		
		void draw();
		
	private:
		int m_x;
		int m_y;
		int m_z;
		
		std::vector<Cube*> m_cubes;
};

#endif // BIOME_H
