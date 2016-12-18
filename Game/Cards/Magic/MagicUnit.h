#pragma once
#include <vector>
#include <Game\Cards\CardData.h>
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\WaitUnit.h>


#define YUG_MAG_CH_START 0
#define YUG_MAG_CH_SMALLUP 1
#define YUG_MAG_CH_SMALLROT 2
#define YUG_MAG_CH_BIGROT 3
#define YUG_MAG_CH_PHASEFOR 4
#define YUG_MAG_CH_VANISH 5
#define YUG_MAG_CH_SPECIFIC 6
#define YUG_MAG_CH_SPECIFC_FINISHED 7
#define YUG_MAG_CH_RETURN 8
#define YUG_MAG_CH_END 9
#define YUG_MAG_CH_IDLE 10
#define YUG_MAG_CH_TRAP1 11
#define YUG_MAG_CH_TRAP2 12

#define YUG_MAG_BAD_COLOR glm::vec4(0.8f,0.5f,0.5f,1.0f)
#define YUG_MAG_GOOD_COLOR glm::vec4(0.5f,0.8f,0.5f,1.0f)

#define YUG_MAG_BOARD 11
#define YUG_MAG_POSITION 12
#define YUG_MAG_HAND 13

namespace Card{

	class MagicUnit : public Game::YugiohUnit, Game::WaitUnit
	{
	public:
		bool hasCard;
		CardData* magicCard;
		MagicSource* magicSource;
		int cardNo;
		int chain;
		int oldRow;
		int oldColumn;
		int source;

		void startup();
		void cleanup();

		void render();
		void update();
		void input();

		void callCorrectStartup();
		void callCorrectRender();
		void callCorrectUpdate();
		void callCorrectInput();
		void callCorrectCleanup();
		void getCorrectMagicSource();

		//basic animation stuff
		void interpolateSelfUp(float duration = 0.2f);
		void rotateSelfUp(float duration = 0.2f);
		void setUpBigCard();
		void cardReveal1(float duration = 0.2f);
		void cardReveal2(float duration = 0.2f);
		void phaseForwards(float duration = 0.2f);
		void dontRenderEitherParts();
		//basic update stuff
		void startUpdate(int info);
		void smallUpUpdate();
		void smallRotUpdate();
		void bigRotUpdate();
		void phaseForUpdate();
		void deleteUpdate();
		void returnUpdate();
		void endUpdate();

		//trap Reaction
		void trapUpdate1();
		void trapUpdate2();

		//text showcase
		bool amtranUpdating;
		float currentAmtranDuration;
		float amtranSpeed;
		glm::vec4 startingAmtran;
		glm::vec4 endingAmtran;
		glm::vec4 currentAmtran;


		void interpTextAmtran(glm::vec4 endAmtran, float duration);
		void continueInterpTextAmtran();
		std::vector<CardData*> returnAllPlayerMonsters();
		std::vector<CardData*> returnAllEnemyMonsters();

		//utility
		void copyCardFromBoard();
		void copyCardFromPosition();
		void copyCardFromHand();
		void removeMagicCard();
		glm::vec3 getCorrectFusCentre();
	
		static MagicUnit magicUnitInstance;


		//all the magic Cards
	};
#define magicUnit Card::MagicUnit::magicUnitInstance
}
