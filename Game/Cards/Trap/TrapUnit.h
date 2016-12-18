#pragma once
#include <Game\Cards\Trap\TrapSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>

#define YUG_TRAP_CH_START 0
#define YUG_TRAP_CH_CAM_PULL 1
#define YUG_TRAP_CH_SPARKLE 2
#define YUG_TRAP_CH_CAM_BACK 3
#define YUG_TRAP_CH_SPECIFIC 4
#define YUG_TRAP_CH_SPECIFIC_END 5
#define YUG_TRAP_CH_RETURN 6
#define YUG_TRAP_CH_END 7
#define YUG_TRAP_CH_IDLE 8
#define YUG_TRAP_CH_MAG_RETURN 13
#define YUG_TRAP_CH_MAG_END 14

#define YUG_TRAP_ATTACK 9
#define YUG_TRAP_MON_PLAYED 12
#define YUG_TRAP_STANCE_CHANGE 10
#define YUG_TRAP_MAGIC_ACTIVATE 11

#define YUG_TRAP_BAD_COLOR glm::vec4(0.8f,0.5f,0.5f,1.0f)
#define YUG_TRAP_GOOD_COLOR glm::vec4(0.5f,0.8f,0.5f,1.0f)

namespace Card{

	class TrapUnit :  public Game::YugiohUnit, public Game::WaitUnit
	{
	public:

		bool hasCard;
		CardData* trapCard;
		int cardRow;
		int cardCol;
		int currentSourceCol;
		int currentSourceRow;
		int returnPoint;
		bool cardToCollect;
		int cardToCollectCol;

		void startup();
		void cleanup();

		void render();
		void update();

		TrapSource* traps[5][2];
		TrapSource* spareTrap;
		TrapSource* currentTrap;

		int chain;
		int cardNumber;
		int currentTrapCol;
		bool listen(int info);
		bool enemyListen(int info);

		void addTrap(int col, bool player);
		TrapSource* getTrapPointer();
		void removeTrap(int col, bool player);
		void removeAllTraps();

		//updates
		void start(int info);
		void camPullUpdate();
		void sparkleUpdate();
		void camBackUpdate();
		void specificUpdate();
		void deleteUpdate();
		void returnUpdate();
		void endUpdate();

		bool magicEnd;
		void magicReturnUpdate();
		void magicEndUpdate();

		//utility
		void cameraToTrapView(float duration);
		void setUpFromBoard();
		void setUpFromMagic();
		void setUpFromPlay();
		void printOutTrapSources();

		static TrapUnit trapUnitInstance;
	};
#define trapUnit Card::TrapUnit::trapUnitInstance

}
