#pragma once
#include <GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game/Duel/Board.h>
#include <Game/Duel/FusionUnit.h>
#include <Game/Duel/GamePlayer.h>
#include <Game/Cards/CardData.h>
#include <Game\FlickerModelLoader.h>

#define YUG_POSITION_CH_IDLE 0
#define YUG_POSITION_CH_FACE 1
#define YUG_POSITION_CH_CONS 2
#define YUG_POSITION_CH_MAGIC_CONS 5
#define YUG_POSITION_CH_PLACING 3
#define YUG_POSITION_CH_FINISH 4

namespace Duel{

	class PositionUnit : public Game::YugiohUnit
	{
	public:
		bool doRender;
		int chain;
		Card::CardData card;
		bool hasFocus;
		bool humanPlayer;
		bool multiCards;
		bool placeInMagicRow;
		int cP;

		bool renderBothArrows;
		Game::FlickerModelLoader leftArrow;
		Game::FlickerModelLoader rightArrow;

		bool renderConParts;
		Game::ModelLoader conBG;
		Game::ModelLoader conCursor;

		void initializeArrows();
		void initializeConParts();
		//first stage
		void cleanOutSingleCard();
		bool multiShouldPlaceInMagicRow();
		bool singleShouldPlaceInMagicRow();
		void choosePosition();
		void chooseFace();
		void returnToChooseFace();
		//face
		void interpolateToFacePosition();
		void rotateToFacePosition();
		void darkenOutOtherCards();
		void brightenOutOtherCards();
		void renderArrows();
		void stopRenderArrows();
		void flipCard();
		void keepCorrectBoardFace();
		void keepCorrectUpFace();
		//fusion
		void addBoardCardToFusion();
		void addHandCardToFusion();
		//con
		void passCardToPositionUnit();
		bool isMonster();
		void conPosition();
		void interpolateCardToConPosition();
		void rotateCardToConPosition();
		void startRenderCons();
		void moveConCursor();
		void interpolateCursorToPosition();
		//magics
		void playMagicFaceDown();

		//end
		void giveCardtoBoard();
		void toPlayerTurn();

		bool initialize();
		bool shutdown();
		void playingMagic();

		void input();
		void render();
		void update();

		void renderCons();

		void faceInput();
		void consInput();

		int conPos;

		void magicConsUpdate();
		void placingUpdate();
		void finishUpdate();
		void start();

		void addCardToFusion();
		bool isSpaceEmpty();
		bool wantFusion();
		void singleCardsFusion();
		bool allMagics();
		bool isMagic();
		bool boardFuse;

		void returnCardToHand();
		bool noMarkUps();
		void markHandCardForFusion();
		bool fusionNeeded();
		void interpolateCursor();
		void interpolateFinalCard();
		void interpolateUpCard();
		bool fusion_Needed;

		void wait(float duration);
		void continueWaiting();
		float waitDuration;
		float currentlyWaited;
		bool isWaiting;
		static PositionUnit positionUnitInstance;
	};
#define positionUnit Duel::PositionUnit::positionUnitInstance
}