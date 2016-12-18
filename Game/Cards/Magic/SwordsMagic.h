#pragma once
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
namespace Card{

	class SwordsMagic : public MagicSource, public Game::WaitUnit
	{
	public:
		bool returnNow;
		void startup();
		void cleanup();
		void update();
		void render();
	};

}
