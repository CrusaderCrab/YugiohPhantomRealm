#pragma once
#include <GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game\Cards\CardData.h>
#include <Game\Duel\GamePlayer.h>
#include <Game\Duel\EnemyPlayer.h>
#include <Game\ModelLoader.h>
#include <Game\Duel\Parts\FieldWidget.h>
#include <Game\Duel\Parts\LifepointsWidget.h>
#include <Game\Duel\Parts\UnderlayWidget.h>
#include <Game\Duel\Parts\BattleCursor.h>
#include <Game\Duel\EndUnit.h>
#include <string>
#define YUG_BOARD_CARDS_PER_ROW 5
#define YUG_BOARD_CARDS_PER_COL 4
#define YUG_BOARD_PLAYER_MON_ROW 1
#define YUG_BOARD_PLAYER_MAG_ROW 0
#define YUG_BOARD_ENEMY_MON_ROW 2
#define YUG_BOARD_ENEMY_MAG_ROW 3

#define YUG_BOARD_CH_VIEWONLY 0
#define YUG_BOARD_CH_CHOOSINGPOSITION 1
#define YUG_BOARD_CH_PLAYERTURN 2
#define YUG_BOARD_CH_BATTLEUNIT 3
#define YUG_BOARD_CH_ENDTURN 4
#define YUG_BOARD_CH_MAGICEFFECT 5
#define YUG_BOARD_CH_START 6
#define YUG_BOARD_CH_FINISH 7
#define YUG_BOARD_CH_IDLE 8
#define YUG_BOARD_CH_MAGICHCHOOSE 9
#define YUG_BOARD_CH_EQUIPCHOOSE 10
#define YUG_BOARD_CH_EQUIPUNIT 11
#define YUG_BOARD_CH_CHOOSEENEMY 12
#define YUG_BOARD_CH_TURNOVER1 13
#define YUG_BOARD_CH_TURNOVER1HALF 16
#define YUG_BOARD_CH_TURNOVER2 14
#define YUG_BOARD_CH_TURNOVER2HALF 17
#define YUG_BOARD_CH_TURNOVER3 15
#define YUG_BOARD_CH_PANIN 20
#define YUG_BOARD_CH_STARTNORM 21

#define YUG_GP_HUMAN 0
#define YUG_GP_AI 1

namespace Duel{


	class Board : public Game::YugiohUnit
	{
		void setupModels();
	public:

		static Card::CardData board[YUG_BOARD_CARDS_PER_ROW][YUG_BOARD_CARDS_PER_COL];
		GamePlayer player;
		EnemyPlayer enemy;
		GamePlayer* currentPlayer;
		Game::ModelLoader boardModel;
		FieldWidget field;
		LifepointsWidget LP;
		UnderlayWidget underlay;
		BattleCursor battleCursor;
		int currentField;
		static int cP[2];
		static int aP[2];
		int chain;
		bool closed;
		bool inBattle;
		bool halfView;
		bool magicBeforeParticles;
		bool trapBeforeParticles;
		std::string leadInTrack;
		std::string mainTrack;
		float leadInLength;

		bool initialize();
		bool shutdown();

		bool startup();
		bool startup(char miscInf);
		bool cleanup();

		char miscInfo;
		Card::CardData displayCard;
		bool doRender;
		void render();
		void input();
		void update();
		void toThisScreen();

		void beginGame();


		void normalInputs();
		void viewOnlyInputs();
		void choosePositionInputs();
		void playerTurnInputs();
		void chooseEnemyInputs();
		void magicChooseInputs();
		void equipChooseInputs();
		void confirmMagicCard();
		bool lockRowMovement;

		void endTurnUpdate();
		void magicEffectUpdate();
		void startUp();
		void finishUpdate();
		void battleUnitUpdate();
		void equipUnitUpdate();


		void changeCardMode();
		void interpolateCursor();
		void cameraToBoardCursor();
		void cameraToPlayer();
		void moveCursorToAttackRow();
		void flipCard(int col, int row);

		//General
		void newTurn();


		//Main



		//Animation
		void renderUnderlay();
		void renderPlayedCards();
		void renderField();
		void renderLifepoints();
		void renderFusionUnit();
		void renderBattleUnit();
		void renderEffectCard();

		void currentPlayerView(float speed);
		void currentBoardView(float speed);
		glm::vec3 calcCameraPos();
		void moveCamera();
		void moveHandCardsIfNeccessary();
		void playerTurnStartPos();
		void pullCardToBoard(int col, int row);
		void pullCardToBoard(int col, int row, float duration);
		void rotateCardToBoard(int col, int row);
		void rotateCardToBoard(int col, int row, float duration);
		glm::mat4 getCorrectCardRotation(int col, int row);
		glm::vec4 getCurrentOverallAmtran();
		glm::vec4 getCurrentOverallDarkenedAmtran();

		//animation camera turnover
		void turnover1Update();
		void turnover1HalfUpdate();
		void turnover2Update();
		void turnover3Update();

		void battleCursorReaction();
		
		//magics && equips
		int preMagicPosition;
		void chooseEquipCard();
		int equipRow;
		int equipColumn;
		int fusionPlaceRow;
		int fusionPlaceColumn;
		void boardFusion();

		//Utility
		bool enemyHasNoCards();
		void wait(float duration);
		void continueWaiting();
		bool playerControlling();
		void flipCardIfNecessary();
		void flipAttackerIfNecessary();
		bool onPlayerSide();
		bool overMonsterCard();
		bool overPlayerCard();
		bool currentCardGood();
		bool currentAtkCardGood();
		Card::CardData* getCurrentCard();
		Card::CardData* getCurrentAtkCard();
		glm::vec3 getBoardPosition(int col, int row);
		bool cardVisibleToPlayer();
		void updateGuardWarnings();
		void prepareCardsForNewTurn();
		int getFirstEmptyColumn(int row);
		void pressNextEnemyInput();
		void interpolateBoardAndCardAmtran(glm::vec4 newAmtran, float duration);
		bool isMagicCard(Card::CardData* c);

		//start functions
		void prepareBoardModel();
		void panInUpdate();
		void panFinishUpdate();

		float waitDuration;
		float currentlyWaited;
		bool isWaiting;

		//MISC
		static Board boardInstance;
	};
#define theBoard Duel::Board::boardInstance
#define currentCard board[theBoard.cP[0]][theBoard.cP[1]]
}