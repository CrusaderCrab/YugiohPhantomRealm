#pragma once
#include <GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game\YugiohUnit.h>
#include <Game\Cards\BigCardRender.h>
#include <Game\ModelLoader.h>

#define YUG_BAT_CH_START 0
#define YUG_BAT_CH_POSITIONING 1
#define YUG_BAT_CH_ATTACK1 2
#define YUG_BAT_CH_BURNUP 3
#define YUG_BAT_CH_REBOUND 4
#define YUG_BAT_CH_CLEANUP 5
#define YUG_BAT_CH_END 6
#define YUG_BAT_CH_IDLE 7
#define YUG_BAT_CH_STARTSET 8
#define YUG_BAT_CH_LIFTOFF 9
#define YUG_BAT_CH_CAMBACK 10
#define YUG_BAT_CH_REVEAL1 11
#define YUG_BAT_CH_REVEAL2 12
#define YUG_BAT_CH_GUARD1 13
#define YUG_BAT_CH_GUARD2 14
#define YUG_BAT_CH_GUARDEND 15
#define YUG_BAT_CH_STARTSLASH 16
#define YUG_BAT_CH_WHITEFLASH 17
#define YUG_BAT_CH_FADEOUT 18
#define YUG_BAT_CH_SUCCESSANI 19
#define YUG_BAT_CH_FAILANI 20
#define YUG_BAT_CH_SUCCESSANI2 28
#define YUG_BAT_CH_FAILANI2 29
#define YUG_BAT_CH_WAITBEAT 21
#define YUG_BAT_CH_REBOUNDSTART 22
#define YUG_BAT_CH_BURNUPSET 23
#define YUG_BAT_CH_FADEOUTCARD 24
#define YUG_BAT_CH_RETURN 25
#define YUG_BAT_CH_ENDSET 26

#define ZYUG_SHAKE_NONE 0 

namespace Duel{
	class BattleUnit : public Game::YugiohUnit
	{
	public:
		int chain;
		
		bool printDamage;
		bool printOnDefender;
		

		bool doRender;
		bool doUpdate;

		Card::BigCardRender atkCard;
		Card::BigCardRender defCard;


		bool initialize();
		bool shutdown();

		void startBattle(bool directAtk);
		void positionCards();
		void setupCons();
		void update();
		void render();

		void positionUpdate();
		void attack1Update();
		void burnupUpdate();
		void reboundUpdate();
		void cleanupUpdate();
		void endUpdate();

		int attackCalculation();
		void successfulAttack();
		void drawAttack();
		void unsuccessfulAttack();

		void lowerDefendersLifepoints();
		void lowerAttackersLifepoints();

		void wait(float duration);
		void continueWaiting();
		float waitDuration;
		float currentlyWaited;
		bool isWaiting;

		glm::vec3 atkCardOffPosition;
		glm::vec3 atkCardOnPosition;
		glm::vec3 defCardOffPosition;
		glm::vec3 defCardOnPosition;
		glm::vec3 textOffset;
		glm::vec3 cardScale;
		//calc parts
		bool willRebound;//fillin//
		bool onRebound;//fillin//
		bool defenderDefending;//start//
		bool directAttack;//start//
		bool atkCardDead;//fill//
		bool defCardDead;//fill//
		int damage;//fill//
		bool atkGuardianBoost;//set//
		bool defGuardianBoost;//set//
		bool draw;//fill//

		//calc functions
		void initializeGlow();
		void initializeSlash();
		void initializeFlash();
		void shutdownModels();

		//calc utility
		void fillInParts();
		void destroyCards();
		void inflictDamage();
		int getDefenderValue();

		//ANIMATION

		//parts
		Game::ModelLoader glow;
		Game::ModelLoader slash;
		Game::ModelLoader flash;
		bool callFadeUnit;
		glm::vec3 originalCardPosition;
		int shakeCardVar;
		bool textFading;
		float fadeTextSpeed;
		float fadeTextDuration;
		float textStarttran;
		float textDestinationTran;
		float textCurrentTran;
		bool defCardBurnt;

		//functions
		void animationUpdate();
		void battleStartUpdate();
		void liftoffUpdate();
		void interpolateSmallCardsToPosition();
		void rotateSmallCardsToPosition();
		void cambackUpdate();
		void cardReveal1Update();
		void flipCardsCorrectDirection();
		void cardReveal2Update();
		void guard1Update();
		void guard2Update();
		void startSlashUpdate();
		void whiteFlashUpdate();
		void startFlash();
		void endFlash();
		void fadeOutUpdate();
		void successUpdate();
		void shakeCard();
		void fadeTextIn();
		void attackGlow();
		void fadeTextOut();
		void success2Update();
		void waitBeatUpdate();
		void failUpdate();
		void fail2Update();
		void burnupSetUpdate();
		void atkCardBurnup();
		void defCardBurnup();
		void fadeCardOutUpdate();
		void prepareBoard();
		void returnUpdate();
		void endSetUpdate();

		//utility
		void setUpEmphasises();
		void setUpBigAttacker();
		void setUpBigDefender();
		void shakeCardUpdate();
		void fadeTextUpdate();
		void renderfadeText();
		void fadeCardUpdate();


		static BattleUnit battleUnitInstance;
	};
#define battleUnit Duel::BattleUnit::battleUnitInstance
#define attacker board[theBoard.aP[0]][theBoard.aP[1]]
}