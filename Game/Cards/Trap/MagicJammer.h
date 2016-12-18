#pragma once
#include <Game\Cards\Trap\TrapSource.h>
#include <Game\WaitUnit.h>
#include <Game\Cards\CardData.h>

namespace Card{

	class MagicJammer : public TrapSource, public Game::WaitUnit
	{
	public:
		int chain;

		void startup();
		void cleanup();
		void update();
		void render();
		bool listen(int info);
	};

}

