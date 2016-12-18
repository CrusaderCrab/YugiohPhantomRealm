#include <stdlib.h>     /* srand, rand */
#include <chrono>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Duel\Deck.h>
#include <Utility\ErrorHandler.h>

namespace Duel{

	bool Deck::startup(){
		return true;
	}
	bool Deck::cleanup(){
		return true;
	}
	Card::CardData Deck::drawCard(){
		Card::CardData card = cardCreator.createCard(internalDeck[cardsDrawn]);
		cardsDrawn++;
		cardsLeft--;
		return card;
	}
	void Deck::newDuel(){
		shuffleDeck();
		cardsDrawn = 0;
		cardsLeft = 40;
	}
	bool Deck::deckEmpty(){
		return cardsLeft <= 0;
	}
	int Deck::amountInDeck(unsigned int cardNo){
		int count = 0;
		for(unsigned int index = 0; index <internalDeck.size(); index++){
			if(internalDeck[index] == cardNo)
				count++;
		}
		return count;
	}
	bool Deck::tripleInDeck(unsigned int cardNo){
		return (amountInDeck(cardNo) >= 3); 
	}
	bool Deck::deckNumberCorrect(){
		return internalDeck.size() == YUG_DECK_MAX_CARDS;
	}
	bool Deck::maxCardsInDeck(){
		return internalDeck.size() >= YUG_DECK_MAX_CARDS;
	}
	bool Deck::addCard(unsigned int cardNo){
		if(!tripleInDeck(cardNo) && !maxCardsInDeck()){
			internalDeck.push_back(cardNo);
			return true;
		}
		return false;
	}
	void Deck::removeFromDeck(int deckPlacement){
		internalDeck.erase(internalDeck.begin() + deckPlacement);
	}
	void Deck::shuffleDeck(){
		int temp;
		std::srand(time(NULL));
		for(unsigned int c = 0 ; c < 3; c++){
			for(unsigned int i = 0; i <internalDeck.size(); i++){
				temp = internalDeck[i];
				int rand = std::rand() % internalDeck.size();
				internalDeck[i] = internalDeck[rand];
				internalDeck[rand] = temp;
			}
		}
	}
	void Deck::sortByNumber(){
		//errorHandler.printError("Deck.sort() has yet to be written. shuffling instead");
		shuffleDeck();
	}

	int Deck::getNoAt(int index){
		return internalDeck[index];
	}










}
