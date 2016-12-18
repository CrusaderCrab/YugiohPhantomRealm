#pragma once
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>

namespace Card{

	class ChangeOfHeart : public MagicSource, public Game::WaitUnit
	{ 
	public:
		int chain;
		int oldRow;
		int oldColumn;
		int newRow;
		int newColumn;
		Game::ModelLoader bg;
		CardData* card;
		void startup();
		void cleanup();
		void update();
		void render();

		bool copyCardOver();
		void interpolateCardUp();
		void startUpdate();
		void pullDownUpdate();
		void thirdUpdate();
		void finishedUpdate();
		int findNewColumn();
		int getStrongestCol(int col);
		void changeOverData();
	};


}
