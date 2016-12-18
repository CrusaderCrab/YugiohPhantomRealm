#pragma once
#include <vector>
#include <Game\Cards\Trap\TrapSource.h>
#include <Game\WaitUnit.h>
#include <Game\Cards\CardData.h>
#include <Game\ModelLoader.h>

namespace Card{

	class Reverse : public TrapSource, public Game::WaitUnit
	{
	public:
		int chain;
		std::vector<CardData*> cards;
		float cardTime;
		float cardSpeed;
		bool risen;

		void startup();
		void cleanup();
		void update();
		void render();
		bool listen(int info);

		void startUpdate();
		void fadeInUpdate();
		void particleUpdate();
		void fadeOutUpdate();
		void finishUpdate();

		void alterCardStats();
	};

}