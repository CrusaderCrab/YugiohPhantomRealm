#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>
namespace Card{
	class DarkLight : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		Game::ModelLoader centreGlow;
		float time;
		float speed;

		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void glowUpdate();
		void flashUpdate();
		void fadeUpdate();
		void finishUpdate();
	};
}
