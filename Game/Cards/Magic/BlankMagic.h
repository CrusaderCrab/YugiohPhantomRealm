#pragma once
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\ModelLoader.h>
#include <Game\WaitUnit.h>

namespace Card{

	class BlankMagic  : public MagicSource, public Game::WaitUnit
	{
	public:
		void startup();
		void cleanup();
		void update();
		void render();
	};

}

