#pragma once
#include <Game\Cards\CardData.h>
#include <fstream>
#include <vector>
namespace Card{
	class CardCreatorUnit
	{
		std::vector<char> fileName;
		void createFileName(int cardNo, const char* prefix, int length);
		void setupStream(std::ifstream* input);
		void untilLBracket(std::ifstream* input);
		void untilRBracket(std::ifstream* input);
		char getNextDelim(std::ifstream* input);
	public:
		CardData createCard(int cardNo);
		void setupParents(CardData* parent);
		CardData blankCard();
		static CardCreatorUnit cardCreatorUnitInstance;
	};
#define cardCreator Card::CardCreatorUnit::cardCreatorUnitInstance
}
