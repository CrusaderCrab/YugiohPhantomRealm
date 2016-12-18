#pragma once
#include <Game\Cards\Trap\TrapSource.h>
#include <Game\WaitUnit.h>
#include <Game\Cards\CardData.h>

namespace Card{

	class Fake : public TrapSource, public Game::WaitUnit
	{
	public:
		CardData* attackCard;
		CardData* defender;

		void startup();
		void cleanup();

		void update();
		void render();

		int chain;
		void startUpdate();
		void fadeInUpdate();
		void burnUpdate();
		void fadeCardUpdate();
		void fadeOutUpdate();
		void returnUpdate();
		void endUpdate();

		bool renderingFade;
		bool hasCards;
		void setupBigCards();

		bool listen(int info);
	};


}

