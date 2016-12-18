#pragma once
#include <Game\ModelLoader.h>
#include <Game\WaitUnit.h>

namespace Duel{

	class SwordUnit : Game::WaitUnit
	{
	public:
		int chain;
		float time; float speed;
		int playerWait;
		int enemyWait;
		bool doRender;
		bool doUpdate;
		bool playerView;
		bool playerReset;
		Game::ModelLoader swords[14];
		static SwordUnit swordUnitInstance;

		void startup();
		void cleanup();
		void update();
		void render();

		void fallingUpdate();
		void fallingUpdate2();
		void fadeUpdate();
		void resetUpdate();

		void setupSwords();
		int currentPlayerCountLeft();
		void currentPlayerTurnEnd();
		bool currentPlayerCanAttack();
		void activateSwords();
	};
#define swordUnit Duel::SwordUnit::swordUnitInstance
}
