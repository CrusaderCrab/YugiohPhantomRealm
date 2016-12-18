#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>

namespace Card{
	class TypeDestroyer : public MagicSource, public Game::WaitUnit
	{
	public:
		TypeDestroyer(int type, glm::vec4 colour);

		int chain;
		Game::ModelLoader crosshair;
		glm::vec4 color;
		std::vector<glm::vec3>posis;
		float time;
		float speed;
		float zOff;
		int deadType;
		bool blink1;
		bool blink2;
		bool blink3;
		bool blink4;

		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void crosshairUpdate();
		void cardDestroyUpdate();
		void finishUpdate();

		void getPositions();
	};

}