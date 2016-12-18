#include <stdlib.h>     /* srand, rand */
#include <fstream>
#include <string>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Duel\Deck.h>
#include <Utility\ErrorHandler.h>
#include <chrono>
#include <iostream>

namespace Duel{

	void Deck::fillList(std::vector< std::vector<unsigned int>>& v, std::string path){
		std::ifstream in(path);
		char ch = 'A';
		int outerIndex = 0;

		while(true){
			ch = toNextSymbol(in);
			if(ch == '%') break;

			if(ch == '#'){
				std::vector<unsigned int> t;
				v.push_back(t);
				toNextSymbol(in);
				unsigned int no = 9999;
				while(true){
					in>>no;
					if(no == 0)break;
					if(no<100 || no>843)
						std::cout<<"DeckGenerator: bad card number given: "<<no<<std::endl;
					v[outerIndex].push_back(no);
				}
				//std::cout<<"fill: out = "<<outerIndex<<" in = "<<v[outerIndex].size()<<std::endl;
				outerIndex++;
			}
		}

	}

	void Deck::addMagicCards(std::vector<unsigned int>& newDeck){
		unsigned int fields[] = {338,839,840,841,842,843};

		unsigned int equips[] = {339,340,341,342,345,347,
			348,349,778,783,784,785,786,787,788,789,790,791,792,
			793,794,795,796,798};

		unsigned int misc[] = {803,818,823,829,832,834};

		newDeck.push_back(805);
		int i = rand()%6;
		newDeck.push_back(fields[i]);
		i = rand()%24;
		newDeck.push_back(equips[i]);
		i = rand()%6;
		newDeck.push_back(misc[i]);
	}

	void Deck::addOneOfEachCardType(std::vector< std::vector<unsigned int>>& v, std::vector<unsigned int>& newDeck){
		for(unsigned int i = 0; i < v.size(); i++){
			switch(i){
			case 4: break;
			case 0: if(rand()%2 == 0) addCardFrom(v[i], newDeck); break;
			case 10: if(rand()%2 == 0) addCardFrom(v[i], newDeck); break;
			case 11: if(rand()%2 == 0) addCardFrom(v[i], newDeck); break;
			case 12: if(rand()%2 == 0) addCardFrom(v[i], newDeck); break;
			case 15: if(rand()%2 == 0) addCardFrom(v[i], newDeck); break;
			default: addCardFrom(v[i], newDeck);
			}
			
		}
	}

	void Deck::addCardFrom(std::vector<unsigned int>& cardlist, std::vector<unsigned int>& newDeck){
		if(cardlist.size() <= 0) return;
		int i = rand()%cardlist.size();
		newDeck.push_back(cardlist[i]);
		cardlist.erase(cardlist.begin()+i);
	}

	void Deck::generateNewDeck(){
		std::vector<unsigned int> newDeck;
		std::vector< std::vector<unsigned int> >lowList;
		std::vector< std::vector<unsigned int> >highList;
		std::string lowPath = "GameData/cardData/deck/lowDeckGeneratorList.txt";
		std::string highPath = "GameData/cardData/deck/highDeckGeneratorList.txt";
		fillList(lowList, lowPath);
		fillList(highList, highPath);
		
		srand(time(NULL));
		addMagicCards(newDeck);
		addOneOfEachCardType(lowList, newDeck);
		int typeCounts[] = {
			1,1,1,1,1,
			1,1,1,1,1,
			1,1,1,1,1,
			1,1,1,1,1};

		//low deck
		while(newDeck.size() <= 38){
			int cardType = rand()%1000;
			if(cardType < 45 && typeCounts[00] < 5){//drag 11,0
				addCardFrom(lowList[0], newDeck); typeCounts[0]++;
			}else if(cardType < 130 && typeCounts[1] < 5){//spell 1
				addCardFrom(lowList[1], newDeck); typeCounts[1]++;
			}else if(cardType < 215 && typeCounts[2] < 5){//zom 2
				addCardFrom(lowList[2], newDeck); typeCounts[2]++;
			}else if(cardType < 300 && typeCounts[3] < 5){//war 3
				addCardFrom(lowList[3], newDeck); typeCounts[3]++;
			}else if(cardType < 316 && typeCounts[4] < 5){//bea-war 4
				addCardFrom(lowList[4], newDeck); typeCounts[4]++;
			}else if(cardType < 401 && typeCounts[5] < 5){//bea 5
				addCardFrom(lowList[5], newDeck); typeCounts[5]++;
			}else if(cardType < 446 && typeCounts[6] < 5){//wing 6
				addCardFrom(lowList[6], newDeck); typeCounts[6]++;
			}else if(cardType < 531 && typeCounts[7] < 5){//fie 7
				addCardFrom(lowList[7], newDeck); typeCounts[7]++;
			}else if(cardType < 616 && typeCounts[8] < 5){//fai 8
				addCardFrom(lowList[8], newDeck); typeCounts[8]++;
			}else if(cardType < 661 && typeCounts[9] < 5){//in 9
				addCardFrom(lowList[9], newDeck); typeCounts[9]++;
			}else if(cardType < 678 && typeCounts[10] < 5){//din 10
				addCardFrom(lowList[10], newDeck); typeCounts[10]++;
			}else if(cardType < 694 && typeCounts[11] < 5){//rep 11
				addCardFrom(lowList[11], newDeck); typeCounts[11]++;
			}else if(cardType < 710 && typeCounts[12] < 5){//fis 12
				addCardFrom(lowList[12], newDeck); typeCounts[12]++;
			}else if(cardType < 771 && typeCounts[14] < 5){//mac 14
				addCardFrom(lowList[14], newDeck); typeCounts[14]++;
			}else if(cardType < 787 && typeCounts[15] < 5){//thu 15
				addCardFrom(lowList[15], newDeck); typeCounts[15]++;
			}else if(cardType < 872 && typeCounts[16] < 5){//aq 16
				addCardFrom(lowList[16], newDeck); typeCounts[16]++;
			}else if(cardType < 917 && typeCounts[17] < 5){//pyr 17
				addCardFrom(lowList[17], newDeck); typeCounts[17]++;
			}else if(cardType < 962 && typeCounts[18] < 5){//ro 18
				addCardFrom(lowList[18], newDeck); typeCounts[18]++;
			}else if(typeCounts[19] < 5){//pla 19
				addCardFrom(lowList[19], newDeck); typeCounts[19]++;
			}
		}

		//high deck
		while(newDeck.size() < 40){
			int cardType = rand()%1000;
			if(cardType < 45 && typeCounts[0] < 5){//drag 11,0
				addCardFrom(highList[0], newDeck); typeCounts[0]++;
			}else if(cardType < 130 && typeCounts[1] < 5){//spell 1
				addCardFrom(highList[1], newDeck); typeCounts[1]++;
			}else if(cardType < 215 && typeCounts[2] < 5){//zom 2
				addCardFrom(highList[2], newDeck); typeCounts[2]++;
			}else if(cardType < 300 && typeCounts[3] < 5){//war 3
				addCardFrom(highList[3], newDeck); typeCounts[3]++;
			}else if(cardType < 316 && typeCounts[4] < 5){//bea-war 4
				addCardFrom(highList[4], newDeck); typeCounts[4]++;
			}else if(cardType < 401 && typeCounts[5] < 5){//bea 5
				addCardFrom(highList[5], newDeck); typeCounts[5]++;
			}else if(cardType < 446 && typeCounts[6] < 5){//wing 6
				addCardFrom(highList[6], newDeck); typeCounts[6]++;
			}else if(cardType < 531 && typeCounts[7] < 5){//fie 7
				addCardFrom(highList[7], newDeck); typeCounts[7]++;
			}else if(cardType < 616 && typeCounts[8] < 5){//fai 8
				addCardFrom(highList[8], newDeck); typeCounts[8]++;
			}else if(cardType < 661 && typeCounts[9] < 5){//in 9
				addCardFrom(highList[9], newDeck); typeCounts[9]++;
			}else if(cardType < 678 && typeCounts[10] < 5){//din 10
				addCardFrom(highList[10], newDeck); typeCounts[10]++;
			}else if(cardType < 694 && typeCounts[11] < 5){//rep 11
				addCardFrom(highList[11], newDeck); typeCounts[11]++;
			}else if(cardType < 710 && typeCounts[12] < 5){//fis 12
				addCardFrom(highList[12], newDeck); typeCounts[12]++;
			}else if(cardType < 771 && typeCounts[14] < 5){//mac 14
				addCardFrom(highList[14], newDeck); typeCounts[14]++;
			}else if(cardType < 787 && typeCounts[15] < 5){//thu 15
				addCardFrom(highList[15], newDeck); typeCounts[15]++;
			}else if(cardType < 872 && typeCounts[16] < 5){//aq 16
				addCardFrom(highList[16], newDeck); typeCounts[16]++;
			}else if(cardType < 917 && typeCounts[17] < 5){//pyr 17
				addCardFrom(highList[17], newDeck); typeCounts[17]++;
			}else if(cardType < 962 && typeCounts[18] < 5){//ro 18
				addCardFrom(highList[18], newDeck); typeCounts[18]++;
			}else if( typeCounts[19] < 5){//pla 19
				addCardFrom(highList[19], newDeck); typeCounts[19]++;
			}
		}

		internalDeck.clear();
		for(unsigned int i = 0; i < newDeck.size(); i++){
			internalDeck.push_back(newDeck[i]);
		}

	}

	char Deck::toNextSymbol(std::ifstream& in){
		char ch;
		while(true){
			ch = in.get();
			if(ch == '#')
				return ch;
			if( ch == '=')
				return ch;
			if( ch == '%')
				return ch;
		}
	}

}