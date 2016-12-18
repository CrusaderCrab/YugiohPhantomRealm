#pragma once
#include <vector>
#include <Game\Cards\Trap\TrapSource.h>
#include <Game\WaitUnit.h>
#include <Game\Cards\CardData.h>
#include <Game\ModelLoader.h>

namespace Card{

	class House : public TrapSource, public Game::WaitUnit
	{
	public:
		House(int a);
		int amount;
		int chain;
		Game::ModelLoader crosshair;
		glm::vec4 color;
		std::vector<glm::vec3>posis;
		float time;
		float speed;
		float zOff;
		bool blink1;
		bool blink2;
		bool blink3;
		bool blink4;

		void startup();
		void cleanup();
		void update();
		void render();
		bool listen(int info);

		void startUpdate();
		void crosshairUpdate();
		void cardDestroyUpdate();
		void finishUpdate();

		void getPositions();
		void destroyCards();
		
	};

}