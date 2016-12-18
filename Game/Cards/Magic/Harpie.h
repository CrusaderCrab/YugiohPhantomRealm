#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>
namespace Card{

	class Harpie  : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		Game::ModelLoader feather;
		float time;
		float speed;
		float rotCount;
		bool player;
		bool ticker;
		bool m1,m2,m3,m4,m5;
		bool featherMove;
		int correctRow;

		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void sweepingUpdate();
		void upwardsUpdate();
		void finishUpdate();

		void vanishMagicCard(int col);
		void removeMagicCard(int col);
	};

}