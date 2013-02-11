#ifndef BIOME_H
#define BIOME_H

class Biome {
	public:
		Biome(float x, float y, float z, GLuint texture);
		~Biome();
		
		void draw();
		
	private:
		float m_x;
		float m_y;
		float m_z;
		
		std::vector<Cube*> m_cubes;
};

#endif // BIOME_H
