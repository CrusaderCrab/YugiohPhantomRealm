#pragma once
#include <fstream>
#include <Game\YugiohUnit.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>
#include <Game\Cards\CardData.h>

#define YUG_END_CONTINUE_DUEL 0
#define YUG_END_PLAYER_WIN 1
#define YUG_END_PLAYER_LOST 2

#define YUG_END_S 4
#define YUG_END_A 3
#define YUG_END_B 2
#define YUG_END_C 1
#define YUG_END_D 0
#define YUG_END_X -1

namespace Duel{

	class EndUnit : public Game::YugiohUnit, Game::WaitUnit
	{
	public:
		bool doRender;
		bool doUpdate;
		int chain;
		bool won;

		//rendering values
		bool callLocalFade;
		Game::ModelLoader rotationThinker;
		Game::ModelLoader bgSheet;
		Game::ModelLoader resultSheet;
		Card::CardData wonCard;
		bool drawLetters;
		std::vector<int>rewardCards; 

		//rating calc values
		int noOfMagicsPlayed;//
		int noOfMagicsUsed;//
		int noOfAttacks;//
		int noOfSuccessfulAttacks;//
		int noOfSuccessfulDefenses;//
		int noOfTurns;//
		int enemyCardsDestroyed;//
		int noOfTrapsPlayed;//
		int trapsUsed;//
		int totalAttackValue;//
		int totalDefenseValue;//
		int noOfMonstersPlayed;//
		int magicCardDamage;
		int magicCardHealing;
		int averAtk;
		int averDef;

		int powValue;
		int tecValue;
		bool pow;
		int resultValue;
		//
		void startup();
		void cleanup();
		void playerWin();
		void playerLost();
		int checkLifepoints();
		void playerOutOfCards();
		void takeInput();

		void setupRatingValues();
		void setUpSheet();
		void setUpCard();
		void setupRotationThinker();
		void setupResultSheet();
		std::string getResultTexturePath();

		void startUpdate();
		void lettersUpdate();
		void changeFadeUpdate();
		void fadeoutUpdate();
		void preEndUpdate();
		void endUpdate();

		void calculateRating();
		void renderRatings();
		void getWonCard();
		int countDeckAmount(int cardNo);
		char getMarkingChar();
		void getRewardCardList();
		void giveCardToPlayer();
		void waitForSymbol(char mark, std::ifstream* in);

		void update();
		void render();
		void input();

		static EndUnit EndUnitInstance;
	};
#define endUnit Duel::EndUnit::EndUnitInstance
}

//pass Input back to container
//fade Screen to black
//clear everything off screen
//pass render back to container
//pass update back to container
//give result to container
//use EndAnaltyics to get new card
//display analytics
//clean up board and all its parts