#include <GL\glew.h>//for GLuints 
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Cards\Magic\MagicCard.h>
#include <Game\Cards\Magic\ChangeOfHeart.h>
#include <iostream>

namespace Card{

	CardCreatorUnit CardCreatorUnit::cardCreatorUnitInstance;

	CardData CardCreatorUnit::createCard(int cardNo){
		if((337 < cardNo && cardNo < 350)||(777 < cardNo && cardNo < 900)){
			createFileName(cardNo,"GameData/cardData/magics/",25);
		}else{
			createFileName(cardNo,"GameData/cardData/monsters/",27);
		}
		//std::cout<<fileName.data()<<std::endl;
		std::ifstream input(fileName.data());
		if(!input){
			std::cout<<"Card Creator: can't open file with card number: "<<cardNo<<"\n";
			return blankCard();
		}
		//std::cout<<"Card Creator: loading Card\n";
		CardData card;
		//card Number
		untilLBracket(&input); input>>card.cardNumber;
		//card name
		untilLBracket(&input); do{
			card.name.push_back(input.get());
		}while(card.name[card.name.size()-1] != ']');
		card.name[card.name.size()-1] = '\0';
		//render path
		untilLBracket(&input); do{
			card.renderFileName.push_back(input.get());
		}while(card.renderFileName[card.renderFileName.size()-1] != ']');
		card.renderFileName[card.renderFileName.size()-1] = '\0';
		//blurb
		untilLBracket(&input); do{
			card.blurb.push_back(input.get());
		}while(card.blurb[card.blurb.size()-1] != ']');
		card.blurb[card.blurb.size()-1] = '\0';
		untilLBracket(&input); input>>card.monMagTrap;
		untilLBracket(&input); input>>card.attack; card.altAttack = card.attack; card.origAttack = card.attack;
		untilLBracket(&input); input>>card.defense; card.altDefense = card.defense; card.origDefense = card.defense;
		untilLBracket(&input); input>>card.starchips;
		untilLBracket(&input); input>>card.actualType;
		untilLBracket(&input);
		int fType = 0;
		do{
			input>>fType;
			card.fusionTypes.push_back(fType);
		}while(getNextDelim(&input) != ']');
		/*
		untilHash(&input); input>>card.fusionTypes[0];
		untilHash(&input); input>>card.fusionTypes[1];
		untilHash(&input); input>>card.fusionTypes[2];
		*/
		untilLBracket(&input); input>>card.element;
		untilLBracket(&input); input>>card.constellations[0];
		untilLBracket(&input); input>>card.constellations[1];
		card.fieldlessAttack = card.attack;
		card.fieldlessDefense = card.defense;
		card.faceUp = true;
		card.hasAttacked = false;
		card.currentConstellation = YUG_CARD_NO_CURRENT_CON;
		card.pictureTBO = YUG_UNBIND;
		return card;
	}

	void CardCreatorUnit::untilLBracket(std::ifstream* input){
		char ch;
		while(true){
			ch = input->get();
			if(ch == '[')
				return;
		}

	}
	char CardCreatorUnit::getNextDelim(std::ifstream* input){
		char ch;
		while(true){
			ch = input->get();
			if(ch == '[')
				return ch;
			if( ch == ']')
				return ch;
		}

	}

	void CardCreatorUnit::setupParents(CardData* parent){
		parent->bigRender.parentCard = parent;
		parent->smallRender.parentCard = parent;
	}

	CardData CardCreatorUnit::blankCard(){
		CardData card;
		card.cardNumber = YUG_NO_CARD;
		return card;
	}

	void CardCreatorUnit::createFileName(int cardNo, const char* prefix, int length){
		//----//std::cout<<"Creating card "<<cardNo<<std::endl;
		const char* directory = prefix;
		//int directoryLength = (sizeof(directory)/sizeof(char))-1;
		int directoryLength = length;
		fileName = std::vector<char>();
		int index = 0;
		for( ;index < directoryLength; index++)
			fileName.push_back( directory[index] );
		
		int hDigit = cardNo / 100;
		int tDigit = ((cardNo/10)%10);
		int uDigit = cardNo%10;
		fileName.push_back(hDigit + '0');//get last digit
		fileName.push_back(tDigit + '0');
		fileName.push_back(uDigit + '0');
		fileName.push_back('.');
		fileName.push_back('t');
		fileName.push_back('x');
		fileName.push_back('t');
		fileName.push_back('\0');
		//std::cout<<"\nCard Creator: created Filename: "<<fileName.data()<<std::endl;
	}
	void CardCreatorUnit::setupStream(std::ifstream* input){
		
	}

}
