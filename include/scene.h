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
		
		static float intersectionLinePlane(vect3D normal, vect3D planePoint, vect3D lineOrigPoint, vect3D directionVector);
		static float intersectionLineCube(int cubeX, int cubeY, int cubeZ, vect3D lineOrigPoint, vect3D directionVector);
		static void testCubes(std::vector<Cube*> cubes);
		
		static Player *player;
		static Biome *biome;
		
	private:
		typedef std::map<std::string, GLuint> Textures;
		
		bool m_cont;
		Textures m_textures;
};

#endif // SCENE_H
