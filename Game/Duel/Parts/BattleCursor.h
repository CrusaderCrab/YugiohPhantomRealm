#pragma once
#include <Game\FlickerModelLoader.h>

namespace Duel{

	class BattleCursor : public Game::FlickerModelLoader
	{
	public:

		std::vector<GLuint> emptyTextures;
		std::vector<GLuint> fullTextures;
		bool emptyMode;

		void startup();
		void cleanup();
		void render();
		void getNextTexture();

		void move();
		void fadeIn();
		void fadeOut();
		void newTurnRotation();
		void turnon();
		void turnoff();
	};

}