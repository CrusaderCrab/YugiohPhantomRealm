#pragma once
#include <vector>
#include <fstream>
#include <Game\Duel\Deck.h>
#include <definesAndTypedefs.h>
#include <Game\AI\AIUnit.h>
namespace Game{

	class EnemyData
	{
		char waitForSymbol(std::ifstream* input);

		EnemyData() {ai = nullptr;}
		EnemyData(const EnemyData&);
		EnemyData& operator=(const EnemyData&);

	public:
		Duel::Deck deck;
	
		//drops
		//name
		AI::AIUnit* ai;
		int enemyNumber;
		int aiNumber;
		int low; int high; int dang, disNum;

		void startup();
		void cleanup();
		void loadCorrespondingAI();
		void loadData(std::string path);
		void loadData(const char* aiFilePath);
		static EnemyData enemyDataInstance;
	};
#define enemyData Game::EnemyData::enemyDataInstance

}
