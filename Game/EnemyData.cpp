#include <Game\EnemyData.h>
#include <Game\AI\RegularAIUnit.h>
#include <Game\AI\DefensiveAIUnit.h>
#include <Game\AI\DebugAIUnit.h>
#include <Utility\ErrorHandler.h>
#include <iostream>
#define YUG_NEW_ENTRY '#'
#define YUG_END_FILE '&'
#define YUG_END_ENTRY '~'


namespace Game{
	EnemyData EnemyData::enemyDataInstance;

	
	void EnemyData::loadData(std::string path){
			//	std::cout<<"Loading Enemy\n";
		std::ifstream input(path);
		waitForSymbol(&input);
		input>>enemyNumber;
		waitForSymbol(&input);
		input>>aiNumber;
		waitForSymbol(&input);
		input>>low>>high>>dang>>disNum;
		//player deck
		deck.internalDeck.clear();
		int deckIndex = 0;
		int cardNo;
		while(waitForSymbol(&input) != YUG_END_FILE 
			&& deckIndex < YUG_DECK_MAX_CARDS)
		{
			input>>cardNo;
			deck.internalDeck.push_back(cardNo);
			deckIndex++;
		}
		//if(deckIndex <  YUG_DECK_MAX_CARDS)
			//errorHandler.printError("Loading Enemy : deck ended prematurely, giving default values");
		//std::cout<<deckIndex<<std::endl;
		for( ; deckIndex <  YUG_DECK_MAX_CARDS;  
			deckIndex++)
		{
			deck.internalDeck.push_back(101);
		}
		//std::cout<<"end deck"<<std::endl;
		loadCorrespondingAI();
	}

	void EnemyData::loadCorrespondingAI(){
		if(ai!=nullptr){
			//std::cout<<"Trying to delete ai\n";
			//std::cout<<"AI delete: "<<ai->moveList.size()<<std::endl;
			//std::cout<<"AI _Myend: "<<ai->moveList._Myend<<std::endl;
			delete ai;
			ai = nullptr;
		}
		switch(aiNumber){
		case 0:
			ai = new AI::RegularAIUnit(YUG_AI_LOWEST_AB, low, high, dang, disNum);
			break;
		case 1:
			ai = new AI::RegularAIUnit(YUG_AI_LOW_AB, low, high, dang, disNum);
			break;
		case 2:
			ai = new AI::RegularAIUnit(YUG_AI_MID_AB, low, high, dang, disNum);
			break;
		case 3:
			ai = new AI::RegularAIUnit(YUG_AI_HIGH_AB, low, high, dang, disNum);
			break;
		case 4:
			ai = new AI::RegularAIUnit(YUG_AI_HIGHEST_AB, low, high, dang, disNum);
			break;
		case 5:
			ai = new AI::DefensiveAIUnit(YUG_AI_LOWEST_AB, low, high, dang, disNum);
			break;
		case 6:
			ai = new AI::DefensiveAIUnit(YUG_AI_LOW_AB, low, high, dang, disNum);
			break;
		case 7:
			ai = new AI::DefensiveAIUnit(YUG_AI_MID_AB, low, high, dang, disNum);
			break;
		case 8:
			ai = new AI::DefensiveAIUnit(YUG_AI_HIGH_AB, low, high, dang, disNum);
			break;
		case 9:
			ai = new AI::DefensiveAIUnit(YUG_AI_HIGHEST_AB, low, high, dang, disNum);
			break;
		case 10:
			ai = new AI::RegularAIUnit(YUG_AI_NO_FUSION, low, high, dang, disNum);
			break;
		default:
			ai = new AI::RegularAIUnit(YUG_AI_HIGHEST_AB, low, high, dang, disNum);
			break;
		}
		ai->startup();
	}

	void EnemyData::startup(){
		ai = nullptr;
	}
	void EnemyData::cleanup(){
		if(ai!=nullptr)
			delete ai;
	}


	char EnemyData::waitForSymbol(std::ifstream* input){
		char ch;
		while(true){
			ch = input->get();
			if(ch == YUG_NEW_ENTRY || ch == YUG_END_FILE || ch == YUG_END_ENTRY)
				return ch;
		}

	}






}
