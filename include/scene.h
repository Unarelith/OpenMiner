#ifndef SCENE_H
#define SCENE_H

typedef struct {
	float x;
	float y;
	float z;
} vect3D;

class Scene {
	public:
		Scene();
		~Scene();
		
		void exec();
		
		void manageEvents();
		void animate();
		void draw();
		void display();
		
		void loadTextures();
		void drawField();
		
		void lockMouse();
		void unlockMouse();
		
		static bool intersectionLinesSphere(vect3D center, float radius, vect3D linePoint, vect3D directionVector);
		static void testCubes(std::vector<Cube*> cubes);
		
		static Player *player;
		
	private:
		typedef std::map<std::string, GLuint> Textures;
		
		bool m_cont;
		Textures m_textures;
		
		Biome *m_biome;
};

#endif // SCENE_H
