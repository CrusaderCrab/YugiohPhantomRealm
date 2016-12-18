#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>
namespace Card{

	class Curse : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		float time;
		float speed;
		bool risen;
		std::vector<CardData*>cards;

		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void fadeInUpdate();
		void particleUpdate();
		void fadeOutUpdate();
		void finishUpdate();

		void getCards();
	};

}
