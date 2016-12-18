#pragma once
#include <vector>
#include <Game\ModelLoader.h>
#include <Game\Cards\CardData.h>
#include <Game\YugiohUnit.h>
#include <Game\WaitUnit.h>
#include <Screens\BaseScreen.h>

namespace Screen{

	class TrunkCard{
	public:
		Card::CardData c;
		int noInTrunk;
		int noInDeck;
		TrunkCard();
		TrunkCard(int tN, int dN);
		~TrunkCard();
	};

	//class Trunk : public Game::YugiohUnit, Game::WaitUnit
	class Trunk : public BaseScreen
	{
	public:
		Game::ModelLoader unusedSheet;
		Game::ModelLoader deckSheet;
		Game::ModelLoader scrollBarModel;
		Game::ModelLoader sortCursorModel;
		Game::ModelLoader mainCursorModel;

		std::vector<TrunkCard> trunkCards;
		int totalCardsInTrunk;
		std::vector<TrunkCard> deckCards;
		int chain;
		bool cardDisplayOver;
		bool closed;
		char miscInfo;
		int fieldType;

		int trunkSortType;
		int trunkCursorPos;
		int trunkDistanceFromTop;
		int trunkDistanceFromBottom;

		int deckSortType;
		int deckCursorPos;
		int deckDistanceFromTop;
		int deckDistanceFromBottom;

		int viewing;

		bool initialize();
		bool shutdown();
		void startup();
		void startup(const char* fileLocal);///
		void cleanup();
		void render();
		void update();
		void input();
		void toThisScreen();

		void setupSheets();
		void setupScrollBar();
		void setupSortCursor();
		void setupMainCursor();

		void loadTrunkCards();
		void loadDeck();
		void addDeckCardsToTrunk();
		void addTrunkNoToDeck();

		void writeTrunkCards();
		void writeDeckCards();

		void startUpdate();
		void switchUpdate();
		void toTrunkUpdate();
		void toDeckUpdate();
		void endUpdate();
		void finalUpdate();
		void cardDisplayOverUpdate();

		bool lessThanName(TrunkCard& cardA, TrunkCard& CardB);
		
		void renderDeckCards();
		void renderTrunkCards();
		void renderMiscNumbers();
		void renderCard(TrunkCard& t, glm::vec3 startPos);

		void currentCursorUp();
		void currentCursorDown();
		void viewCurrentCard();
		void switchView();
		void switchCurrentSort();
		void removeCurrentCard();
		void addCurrentCard();
		void exitTrunk();
		void sprintDown();
		void sprintUp();

		void moveMainCursor();
		void moveScrollBar();
		void moveSortCursor();
		void sortCards(std::vector<TrunkCard>& cards, int sortNo);

		static Trunk trunkInstance;
	};
#define trunkUnit Screen::Trunk::trunkInstance
}

