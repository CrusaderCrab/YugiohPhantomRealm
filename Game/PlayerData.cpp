#include <Utility\ErrorHandler.h>
#include <Game\PlayerData.h>
#include <iostream>

namespace Game{
	char* fileLocals[] = {"GameData/saves/1.txt","GameData/saves/2.txt","GameData/saves/3.txt"};
	PlayerData PlayerData::playerDataInstance;
	PlayerDataDuelTriplet PlayerData::unlockedDuelists[YUG_NO_OF_FREE_DUELISTS]; 
	bool PlayerData::plotUnlockedDuelists[YUG_NO_OF_FREE_DUELISTS];
	short PlayerData::trunk[YUG_MAX_NO_OF_CARDS];

	void PlayerData::initialize(){
		currentSaveFile =  YUG_PLAYER_DATA_NO_ACTIVE_SAVE_FILE;
	}

	void PlayerData::loadGame(int fileNo){
		//std::cout<<"Loading Game\n";
		std::ifstream input(fileLocals[fileNo-1]);
		currentSaveFile = fileNo;
		waitForHashTilde(&input);
		//player name
		playerName.clear();
		do{
			playerName.push_back(input.get());
		}while(playerName[playerName.size()-1] != '#');
		playerName[playerName.size()-1] = '\0';
		//std::cout<<"Loaded Name: "<<playerName.data()<<'\n';
		//plot point
		waitForHashTilde(&input); input>>currentPlotPoint;
		//unlocked duelists
		int freeDuelistIndex = 0;
		while(waitForHashTilde(&input) 
			&& freeDuelistIndex < YUG_NO_OF_FREE_DUELISTS)
		{
			input>>unlockedDuelists[freeDuelistIndex].hasUnlocked;
			input>>unlockedDuelists[freeDuelistIndex].winNumber;
			input>>unlockedDuelists[freeDuelistIndex].loseNumber;
			freeDuelistIndex++;
		}
		if(freeDuelistIndex <  YUG_NO_OF_FREE_DUELISTS){
			//std::cout<<freeDuelistIndex<<" ";
			//errorHandler.printError("Loading Player : Unlocked duelists ended prematurely, giving default values");
		}
		for( ; freeDuelistIndex <  (YUG_NO_OF_FREE_DUELISTS);  
			freeDuelistIndex++)
		{
			unlockedDuelists[freeDuelistIndex].hasUnlocked = false;
		}

		//plot unlocked
		freeDuelistIndex = 0;
		while(waitForHashTilde(&input) 
			&& freeDuelistIndex < YUG_NO_OF_FREE_DUELISTS)
		{
			input>>plotUnlockedDuelists[freeDuelistIndex];
			freeDuelistIndex++;
		}
		if(freeDuelistIndex <  YUG_NO_OF_FREE_DUELISTS){
			//std::cout<<freeDuelistIndex<<" ";
			//errorHandler.printError("Loading Player : plot duelists ended prematurely, giving default values");
		}
		for( ; freeDuelistIndex <  (YUG_NO_OF_FREE_DUELISTS);  
			freeDuelistIndex++)
		{
			plotUnlockedDuelists[freeDuelistIndex] = false;
		}

		//player deck
		deck.internalDeck.clear();
		int deckIndex = 0;
		int cardNo;
		while(deckIndex < YUG_DECK_MAX_CARDS && 
			waitForHashTilde(&input) )
		{
			input>>cardNo;
			deck.internalDeck.push_back(cardNo);
			deckIndex++;
		}
		//std::cout<<"end deck"<<std::endl;
		//player trunk
		int trunkIndex = 0;
		while(waitForHashTilde(&input)
			&& trunkIndex < YUG_MAX_NO_OF_CARDS)
		{
			input>>trunk[trunkIndex];
			trunkIndex++;
		}
		if(trunkIndex < YUG_MAX_NO_OF_CARDS){
			//std::cout<<trunkIndex<<" ";
			//	errorHandler.printError("Loading Player : trunk ended prematurely, giving default values");

		}
		for( ;trunkIndex < YUG_MAX_NO_OF_CARDS;
			trunkIndex++)
		{
			if(trunkIndex < 744)
				trunk[trunkIndex] = 0;
			else
				trunk[trunkIndex] = 0;

		}
		name = std::string(playerName.data());
	}
	void PlayerData::saveGame(int fileNo){
		//std::cout<<"Saving Game\n";
		std::ofstream out(fileLocals[fileNo-1]);
		currentSaveFile = fileNo;
		out<<'#'<<playerName.data()<<"#\n";
		out<<'#'<<currentPlotPoint<<"\n";

		for(unsigned int i = 0; i < YUG_NO_OF_FREE_DUELISTS; i++){
			out<<'#'<<unlockedDuelists[i].hasUnlocked
				<<' '<<unlockedDuelists[i].winNumber
				<<' '<<unlockedDuelists[i].loseNumber<<' ';
		}
		out<<"~\n\n";

		for(unsigned int i = 0; i < YUG_NO_OF_FREE_DUELISTS; i++){
			out<<'#'<<plotUnlockedDuelists[i]<<' ';
		}
		out<<"~\n\n";

		for(unsigned int i = 0; i < 40; i++){
			out<<'#'<<deck.getNoAt(i)<<' ';
			if(i%20==0 && i!=0) out<<'\n';
		}
		out<<"\n";

		for(unsigned int i = 0; i < YUG_MAX_NO_OF_CARDS; i++){
			out<<'#'<<trunk[i]<<' ';
			if(i%20==0  && i!=0) out<<'\n';
		}
		out<<"~\n";
	}

	std::string PlayerData::saveDescription(int fileNo){
		std::vector<char> pName; 
		std::ifstream input(fileLocals[fileNo-1]);
		waitForHashTilde(&input);
		//player name
		do{
			pName.push_back(input.get());
		}while(pName[pName.size()-1] != '#');
		pName[pName.size()-1] = ' ';
		pName.push_back('\0');
		if(pName[0]=='~')return std::string("EMPTY");
		//std::cout<<"PlayerData: description Name: "<<playerName.data()<<'\n';
		int plotPoint=0;
		waitForHashTilde(&input); input>>plotPoint;
		int percen = 0;
		for(int i = 0; i < YUG_NO_OF_FREE_DUELISTS; i++){
			percen += plotUnlockedDuelists[i];
		}
		percen *= 3; 
		std::string s = std::string(pName.data());//+std::to_string(percen);
		//std::cout<<"descript file |"<<s<<"|\n";
		return s;
	}

	bool PlayerData::isEmpty(int fileNo){
		std::ifstream input(fileLocals[fileNo-1]);
		waitForHashTilde(&input);
		return(input.get()=='~');
	}


	bool PlayerData::waitForHashTilde(std::ifstream* input){
		char ch;
		while(true){
			ch = input->get();
			if(ch == '#')
				return true;
			if(ch == '~' || ch == '&')
				return false;
		}

	}



}