#ifndef SCENE_H
#define SCENE_H

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
		
	private:
		typedef std::map<std::string, GLuint> Textures;
		
		bool m_cont;
		Textures m_textures;
		
		Player *m_player;
		
		Biome *m_biome;
};

#endif // SCENE_H
