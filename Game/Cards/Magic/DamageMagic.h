#pragma once
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>
namespace Card{

	class DamageMagic : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		int amount;
		bool reversed;
		bool player;
		Game::ModelLoader sp1;
		Game::ModelLoader sp2;
		Game::ModelLoader sp3;
		Game::ModelLoader fg;
		bool b1,b2,b3,b4,b5;
		float time;
		float speed;

		DamageMagic(int amount);
		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void textFadeInUpdate();
		void glimmerUpdate();
		void fadeOutUpdate();
		void finishUpdate();

		void setUpModels();
		
	};

}