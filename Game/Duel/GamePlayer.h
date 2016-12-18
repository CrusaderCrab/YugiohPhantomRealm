#pragma once
#include <vector>
#include <Game\YugiohUnit.h>
#include <Game\Cards\CardData.h>
#include <Game\ModelLoader.h>
#include <Game\Duel\Deck.h>
#include <Game\Duel\markingStruct.h>

#define YUG_GAME_PLAYER_PLAYER 0
#define YUG_GAME_PLAYER_ENEMY 1
#define YUG_GAME_PLAYER_CHAIN_SWEEPING_IN 8
#define YUG_GAME_PLAYER_CHAIN_DRAWING_CARDS 1
#define YUG_GAME_PLAYER_CHAIN_TAKING_INPUTS 2
#define YUG_GAME_PLAYER_CHAIN_FINISHED 3
#define YUG_GAME_PLAYER_CHAIN_XXX 4
#define YUG_GAME_PLAYER_CHAIN_IDLE 5
#define YUG_NEW_TURN_WAIT 0.5f

#define YUG_BOARD_PLAYER_CAMERA_POSITION glm::vec3(0.0f, 1.1f, 5.0f)//glm::vec3(0, 1, 5)
#define YUG_BOARD_PLAYER_CAMERA_ANGLE glm::vec3(0.0f,-0.225f,-1.0f)//glm::vec3(0.0f,-0.2f,-1.0f)

#define YUG_BOARD_ENEMY_CAMERA_POSITION glm::vec3(0, 1, -5)
#define YUG_BOARD_ENEMY_CAMERA_ANGLE glm::vec3(0.0f,-0.2f,1.0f)

#define YUG_MAX_CARDS_IN_HAND 5
namespace Duel{

	class GamePlayer : public Game::YugiohUnit
	{
		bool drawCards();
		bool drawCard(int position);
		void markCardUp(int position);
		void markCardNone(int position);
		void markCardDown(int position);
		void downKeyPress();
		void upKeyPress();
		void leftKeyPress();
		void rightKeyPress();
		void confirmKeyPress();
		void viewCardKeyPress();
		void viewBoardKeyPress();

		

		void sweepingInUpdate();
		void drawingCardsUpdate();
		void takingInputsUpdate();

		void rollBackFusionCount(int removedPos);
		void drawMarkingOn(int position, int markNumber);
		
	public:
		glm::vec3 firstCardPosition;
		glm::vec3 cardOffset;
		glm::vec3 cardOffScreenPosition;
		glm::vec3 markingOffset;
		glm::vec3 markingScale;

		glm::vec3 cursorFirstPosition;
		glm::vec3 cursorOffset;

		Duel::Deck deck;
		std::vector<Card::CardData> hand;
		std::vector<FusionMarkings> fusionMarks;
		int noOfCardsMarked;
		int lifepoints;

		int currentChainPosition;
		int cursorPosition;

		virtual bool startup(int info);
		bool cleanup();

		void render();
		void update();
		virtual void input();

		void wait(float duration);
		void continueWaiting();
		float waitDuration;
		float currentlyWaited;
		bool isWaiting;

		void hideCards();
		void showCards();
		void interpolateCursor();
		virtual void newTurn();
		void forceCardPositions();
		bool human;
		bool currentCardGood();
		Card::CardData* getCurrentCard(); 
		void moveCardsToViewPosition();
		void moveCardsToHandPosition();
		void moveCardsToHandPosition(float duration);

		void toBoardView();
		void toHandView();
		void interpolateToBoardPosition(int i);
		void interpolateToHandPosition(int i);
		void interpolateToHandPosition(int i, float duration);
		float getCorrectBoardZ(int i);
		void boardYInterpolate();
		void moveCardsToHiddenPosition();
		
		bool handView;
		glm::vec3 markingBoardOffset;
		glm::vec3 firstCardBoardPosition;
		glm::vec3 cardBoardOffset;
		glm::vec3 cardBoardJumpOffset;

		void demarkCard();
		void hideHand();
		void removeLP(int amount);
	};

}