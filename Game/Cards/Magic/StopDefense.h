#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Utility\Clock.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>

namespace Card{

	class StopDefense : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		
		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void waitUpdate();
		void finishUpdate();
		void flipCards();

	};

}