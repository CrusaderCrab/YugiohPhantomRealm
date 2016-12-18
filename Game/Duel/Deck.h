#pragma once
#include <DefinesAndTypedefs.h>
#include <Game\Cards\CardData.h>

#include <vector>

#define YUG_DECK_MAX_CARDS 40
namespace Duel{

	class Deck
	{
		
	public:
		std::vector<unsigned int> internalDeck;

		bool startup();
		bool cleanup();

		int cardsDrawn;
		int cardsLeft;

		Card::CardData drawCard();
		void newDuel();
		bool deckEmpty();
		int amountInDeck(unsigned int cardNo);
		bool tripleInDeck(unsigned int cardNo);
		bool deckNumberCorrect();
		bool maxCardsInDeck();
		bool addCard(unsigned int cardNo);
		void removeFromDeck(int deckPlacement);
		void shuffleDeck();
		void sortByNumber();
		int getNoAt(int index);

		void generateNewDeck();
		void fillList(std::vector< std::vector<unsigned int>>& v, std::string path);
		void addMagicCards(std::vector<unsigned int>& newDeck);
		void addOneOfEachCardType(std::vector< std::vector<unsigned int>>& v, std::vector<unsigned int>& newDeck);
		void addCardFrom(std::vector<unsigned int>& cardlist, std::vector<unsigned int>& newDeck);
		char toNextSymbol(std::ifstream& in);
	};

}