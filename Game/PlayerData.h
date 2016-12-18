#pragma once
#include <vector>
#include <fstream>
#include <Game\Duel\Deck.h>
#include <string>
#include <definesAndTypedefs.h>
#define YUG_PLAYER_DATA_NO_ACTIVE_SAVE_FILE -1
#define YUG_PLOT_POINT_START 0
#define YUG_PLOT_POINT_NIGHT_BEFORE 10
#define YUG_PLOT_POINT_FIRST_DAY 11
#define YUG_PLOT_POINT_TOURNEY 12
#define YUG_PLOT_POINT_POST_TOURNEY 30
#define YUG_PLOT_POINT_POST_ESCAPE 31
#define YUG_PLOT_POINT_HAVENT_SEEN_MAGES 32
#define YUG_PLOT_POINT_SEEN_MAGES 33
#define YUG_PLOT_POINT_MAGES 34
#define YUG_PLOT_POINT_POST_MAGES 50
namespace Game{
	class PlayerDataDuelTriplet{
	public:
		bool hasUnlocked;
		int winNumber;
		int loseNumber;
		PlayerDataDuelTriplet(bool ul=false, int w=0, int l=0)
			: hasUnlocked(ul), winNumber(w), loseNumber(l){}
	};

	class PlayerData
	{
		bool waitForHashTilde(std::ifstream* input);

		PlayerData() {}
		PlayerData(const PlayerData&);
		PlayerData& operator=(const PlayerData&);
	public:
		Duel::Deck deck;
		static short trunk[YUG_MAX_NO_OF_CARDS];
		static PlayerDataDuelTriplet unlockedDuelists[YUG_NO_OF_FREE_DUELISTS]; 
		static bool plotUnlockedDuelists[YUG_NO_OF_FREE_DUELISTS]; 
		std::vector<char> playerName;
		int currentPlotPoint;
		int currentSaveFile;

		void initialize();
		void loadGame(int fileNo);
		void saveGame(int fileNo);
		std::string saveDescription(int fileNo);
		bool isEmpty(int fileNo);
		std::string name;

		static PlayerData playerDataInstance;
	};
	#define playerData Game::PlayerData::playerDataInstance
}
