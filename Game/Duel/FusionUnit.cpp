#include <Game\Duel\FusionUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\Clock.h>
#include <Game\Duel\Board.h>
#include <iostream>
#include <Utility\BlankUnit.h>
#include <Utility\StateUnit.h>
#include <Game\Duel\PositionUnit.h>
#include <fstream>
#include <Game\VectorUnit.h>
#define YUG_FUS_WAIT 0.6f
namespace Duel{

	FusionUnit FusionUnit::FusionUnitInstance;

	bool FusionUnit::initialize(){
		firstCardPosition = glm::vec3(-0.97f, 0.2f, 1.7f);
		cardXYoffset = glm::vec2(0.5f, 0.1f);
		throwCardPosition = glm::vec3(-5.0f, -5.0f, 0.0f);
		chain = YUG_FUS_CH_FINISHED;
		initializeCentreGlow();
		initializeCards();
		createFusionLists();
		equiping = false;
		boardFusion = false;
		return true;
	}
	bool FusionUnit::shutdown(){
		return false;
	}

	void FusionUnit::render(){
		for(unsigned int i = 0; i<cards.size(); i++){
			//std::cout<<"fusion Render 1\n";
			if(!cards[i].blankCard()){
				cards[i].smallRender.render();
			}
		}
		if(renderNewCard){
			//std::cout<<"FUS: is render new\n";
			newCard.bigRender.render();
			newCard.smallRender.render();
		}
		if(renderMassiveCard){
			massiveCard.render();
		}
		if(equiping){
			cards[equipMonster].bigRender.render();
		}
		centreGlow.render();
		
	}

	void FusionUnit::update(){
		for(unsigned int i = 0; i < cards.size(); i++){
			if(!cards[i].blankCard())
				cards[i].smallRender.update();
		}
		newCard.smallRender.update();
		newCard.bigRender.update();
		if(equiping){
			cards[equipMonster].bigRender.update();
		}
		centreGlow.update();
		massiveCard.update();
		if(!isWaiting){
			//std::cout<<"Fusion non-wait update\n";
			if(chain == YUG_FUS_CH_POSITIONING){
				//std::cout<<"Fusion pos update\n";
				chain = YUG_FUS_CH_FUSEDOWN;
				wasDowning = true;
				outsideIndex = 0;
				wait(YUG_FUS_WAIT);
			}else if(chain == YUG_FUS_CH_FUSEDOWN){
				//std::cout<<"Fusion down update\n";
				fuseDowns();
			}else if(chain == YUG_FUS_CH_FUSEUP){
				//std::cout<<"Fusion up update\n";
				fuseUps();
			}else if(chain == YUG_FUS_CH_SECONDANIMATION){
				//std::cout<<"Fusion 2nd ani update\n";
				secondaryAnimationUpdate();
			}else if(chain == YUG_FUS_CH_FINALFUSEANIMATION){
				//std::cout<<"Fusion final ani update\n";
				finalFusePart();
			}else if(chain == YUG_FUS_CH_PASSOVER){
				cardPassover();
				
			}else if(chain == YUG_FUS_CH_FINISHED){
				//std::cout<<"Fusion fin update\n";
			}else{
				animationUpdates();
			}

		}else{
			continueWaiting();
		}
	}

	void FusionUnit::wait(float duration){
		//if(isWaiting)
		//	std::cout<<"Game Player: one wait time overriding another\n";
		totalWait = duration;
		currentlyWaited = 0;
		isWaiting = true;
	}
	void FusionUnit::continueWaiting(){
		currentlyWaited += gameClock.lastLoopTime();
		if(currentlyWaited > totalWait){
			isWaiting = false;
		}
	}

	void FusionUnit::takingHandCards(){
		
	}
	void FusionUnit::handFunction(int player){
		//std::cout<<"hand: "<<player<<" "<<(theBoard.currentPlayer==&(theBoard.enemy))<<" "<<!theBoard.playerControlling()<<" "<<!(theBoard.currentPlayer->human)<<std::endl;
		//std::cout<<"fuse hand: b cP "<<theBoard.cP[0]<<" "<<theBoard.cP[1]<<std::endl;
		
		Duel::GamePlayer* p;
		if(player == YUG_GAME_PLAYER_PLAYER){
			p = &(theBoard.player);
		}else{
			p = &(theBoard.enemy);
		}
		for(int markPos = 1; markPos <= 5; markPos++){
			for(int i = 0; i < 5; i++){
				if(!p->hand[i].blankCard()){
					if(p->fusionMarks[i].position==markPos){

						cards.push_back((p->hand[i]));
						cardCreator.setupParents(&(cards[cards.size()-1]));

						markings.push_back(p->fusionMarks[i]);
						markings[markings.size()-1].position = 
							p->fusionMarks[i].position;
						markings[markings.size()-1].upDown = 
							p->fusionMarks[i].upDown;

						p->hand[i] = cardCreator.blankCard();
						p->fusionMarks[i].position = 0;
						p->fusionMarks[i].upDown = YUG_MARKINGS_MIDDLE;

						//std::cout<<"Fusion : new Card\n";
						//std::cout<<cards[cards.size()-1].name.data()<<"\n";
						//std::cout<<markings[markings.size()-1].position<<" "
						//	<<markings[markings.size()-1].upDown<<std::endl;
						//std::cout<<"\n|"<<cards[cards.size()-1].smallRender.parentCard->name.data()<<"|\n"<<std::endl;


					}
				}//if blank
			}//throught cards
		}//through markings
		for(unsigned int i = 0; i < cards.size(); i++){
			cardCreator.setupParents( &(cards[i]) );
			cards[i].hidden = false;
		}
		interpolateCardsToStartPositions();
		rotateCardsToStartPosition();
		blankUnit.holdInputForNow();
		wait(0.5f);
		chain = YUG_FUS_CH_POSITIONING;
		//std::cout<<"finished hand function\n"<<cards.size()<<std::endl;
	}

	void FusionUnit::interpolateCards(){
		/*for(unsigned int i = 0; i < cards.size(); i++){
			cards[i].smallRender.interpolate(glm::vec3(
				firstCardPosition.x + cardXYoffset.x*i,
				firstCardPosition.y + (cardXYoffset.y*markings[i].upDown),
				firstCardPosition.z),
				YUG_WAIT_SMALL_WAIT);
		}*/
	}

	void FusionUnit::fuseDowns(){
		//std::cout<<"fuse: FuseDown called out = "<<outsideIndex<<" \n";
		if(outsideIndex < (cards.size()-1)){
			downRecursion(outsideIndex+1);
			outsideIndex++;
			for(unsigned int t = 0; t< cards.size(); t++){
				//if(!cards[t].blankCard())
					//std::cout<<t<<cards[t].name.data()<<std::endl;
				//else
					//std::cout<<t<<" blank\n";
			}
		}else{//down finished
			//std::cout<<"fuse: FuseDown finished \n";
			outsideIndex = 0;
			chain = YUG_FUS_CH_FUSEUP;
			wasDowning = false;
			wait(YUG_FUS_WAIT);
		}
	}

	void FusionUnit::downRecursion(int insideIndex){
		//std::cout<<"downRec: out "<<outsideIndex<<" in "<<insideIndex<<std::endl;
		if(markings[outsideIndex].upDown == YUG_MARKINGS_UP ||
			cards[outsideIndex].blankCard()){
			//std::cout<<"fuse down: out blank or an up card\n";
			return;
		}
		if(markings[outsideIndex].upDown == YUG_MARKINGS_DOWN &&
			cards[outsideIndex].blankCard()){
			//std::cout<<"fuse down: out down but in blank\n";
			if(insideIndex+1 < cards.size()){
				downRecursion(insideIndex+1);
			}
		}
		if(markings[outsideIndex].upDown == YUG_MARKINGS_DOWN && 
			markings[insideIndex].upDown == YUG_MARKINGS_UP){
			//std::cout<<"fuse down: out down but in up\n";
			markings[outsideIndex].upDown = YUG_MARKINGS_UP;
			return;
		}
		if(markings[outsideIndex].upDown == YUG_MARKINGS_DOWN &&
			markings[insideIndex].upDown == YUG_MARKINGS_DOWN){
				//std::cout<<"fuse down: fusing pri = "<<outsideIndex<<" sec= "<<insideIndex<<"\n";
				priCard = outsideIndex;
				secCard = insideIndex;
				newCard = tryFusion();
				goodFusion = (!newCard.blankCard());
				if(goodFusion){
					newCard.startup();//QWERTY!!!! COULD BREAK ALL
					theBoard.field.applyField(&newCard);
					chain = YUG_FUS_CH_GOOD_START;
				}else{
					chain = YUG_FUS_CH_BAD_START;
				}
				wait(pos.wait[2]);
				/*std::cout<<"Good fusion: "<<goodFusion<<std::endl;
				fusionInterpolate();
				wait(YUG_FUS_WAIT);
				chain = YUG_FUS_CH_SECONDANIMATION;*/
				return;
		}
	}

	void FusionUnit::fusionInterpolate(){
		cards[secCard].smallRender.interpolate(
			cards[priCard].smallRender.position,
			YUG_FUS_WAIT);
	}

	void FusionUnit::fuseUps(){
		//std::cout<<"fuse: Fuseup called out = "<<outsideIndex<<" \n";
		if(outsideIndex < (cards.size()-1)){
			upRecursion(outsideIndex+1);
			outsideIndex++;
			for(unsigned int t = 0; t< cards.size(); t++){
				//if(!cards[t].blankCard())
					//std::cout<<t<<cards[t].name.data()<<std::endl;
				//else
					//std::cout<<t<<" blank\n";
			}
		}else{//down finished
			//std::cout<<"fuse: Fuseup finished \n";
			outsideIndex = 0;
			chain = YUG_FUS_CH_PASSOVER;
			wait(YUG_FUS_WAIT);
		}
	}

	void FusionUnit::upRecursion(int insideIndex){
		if(cards[outsideIndex].blankCard()){
			//std::cout<<"Fuse up, out card is null\n";
			return;
		}
		if(cards[insideIndex].blankCard()){
			//std::cout<<"Fuse up, in card is null\n";
			if(insideIndex+1 < cards.size()){
			upRecursion( insideIndex+1);
			}
			return;
		}
		//std::cout<<"fuse up: fusing pri = "<<outsideIndex<<" sec= "<<insideIndex<<"\n";
		priCard = outsideIndex;
		secCard = insideIndex;
		if(regularFusion()){
			newCard = tryFusion();
			goodFusion = (!newCard.blankCard());
			//std::cout<<"Good fusion: "<<goodFusion<<std::endl;
			if(goodFusion){
				newCard.startup();//QWERTY!!!! COULD BREAK ALL
				theBoard.field.applyField(&newCard);
				chain = YUG_FUS_CH_GOOD_START;
			}else{
				chain = YUG_FUS_CH_BAD_START;
			}
		}else{//equip card
			if(tryEquip()){
				chain = YUG_FUS_CH_EQUIP_START;
			}else{
				chain = YUG_FUS_CH_BAD_START;
			}
		}
		wait(pos.wait[2]);
		return;
	}

	bool FusionUnit::regularFusion(){
		return((cards[priCard].monMagTrap != YUG_EQUIP_CARD &&
			cards[secCard].monMagTrap != YUG_EQUIP_CARD )|| specialEquips(priCard,secCard));
	}

	bool FusionUnit::specialEquips(int p, int s){
		bool ret = false;
		if(cards[p].cardNumber == 827){cards[p].origAttack = cards[s].origAttack; ret = true;}
		if(cards[s].cardNumber == 827){cards[s].origAttack = cards[p].origAttack; ret = true;}
		if(cards[p].cardNumber == 828){
			cards[p].cardNumber = cards[s].cardNumber;
			cards[p].origAttack = cards[s].origAttack;
			for(int i = 0; i<cards[s].fusionTypes.size();i++){
				cards[p].addFusionType(cards[s].fusionTypes[i]);
			}
			ret = true;
		}
		if(cards[s].cardNumber == 828){
			cards[s].cardNumber = cards[p].cardNumber;
			cards[s].origAttack = cards[p].origAttack;
			for(int i = 0; i<cards[p].fusionTypes.size();i++){
				cards[s].addFusionType(cards[p].fusionTypes[i]);
			}
			ret = true;
		}
		return ret;
	}

	void FusionUnit::secondaryAnimationUpdate(){
		if(goodFusion){
			cards[priCard].smallRender.rotate(
				glm::vec3(0.0f,1.0f,0.0f), YUG_HALF_PI, 
				YUG_FUS_WAIT);
			cards[secCard].smallRender.rotate(
				glm::vec3(0.0f,1.0f,0.0f), -YUG_HALF_PI, 
				YUG_FUS_WAIT);
		}else{
			if(cards[secCard].monMagTrap != YUG_MONSTER_CARD){
				cards[secCard].smallRender.interpolate(
					throwCardPosition, YUG_FUS_WAIT);
			}else{
				cards[priCard].smallRender.interpolate(
					throwCardPosition, YUG_FUS_WAIT);
			}
		}
		chain = YUG_FUS_CH_FINALFUSEANIMATION;
		wait(YUG_FUS_WAIT);
	}
	void FusionUnit::finalFusePart(){
		if(goodFusion){
			cards[priCard].cleanup();
			cards[secCard].cleanup();
			cards[secCard] = newCard;
			cardCreator.setupParents(&(cards[secCard]));
			cards[secCard].smallRender.startup();
			cards[secCard].smallRender.position = 
				cards[priCard].smallRender.position;
			cards[priCard] = cardCreator.blankCard();
			//std::cout<<"new Card name: "<<cards[secCard].name.data()<<std::endl;
		}else{
			if(cards[secCard].monMagTrap != YUG_MONSTER_CARD){
				cards[secCard].cleanup();
				cards[secCard] = cards[priCard];
				cardCreator.setupParents(&(cards[secCard]));
				cards[priCard] = cardCreator.blankCard();
			}else{
				cards[priCard].cleanup();
				cards[priCard] = cardCreator.blankCard();
			}
		}
		if(wasDowning){
			markings[secCard].upDown = YUG_MARKINGS_DOWN;
			chain = YUG_FUS_CH_FUSEDOWN;
		}else{
			markings[secCard].upDown = YUG_MARKINGS_UP;
			chain = YUG_FUS_CH_FUSEUP;
		}
		wait(YUG_FUS_WAIT);
	}
	void FusionUnit::pass2Positioner(){

	}

	
	Card::CardData FusionUnit::tryFusion(){
		//std::cout<<"trying fusion pri sec "<<priCard<<" "<<secCard<<std::endl;
		int cardNo = readFusionList();
		gameClock.newLoop();
		if(cardNo != 0){
			std::cout<<"Fusion: "<<cards[priCard].name.data()<<" + "<<cards[secCard].name.data()<<" = "<<cardCreator.createCard(cardNo).name.data()<<std::endl;
			return cardCreator.createCard(cardNo);
		}else{
			return cardCreator.blankCard();
		}
		//std::cout<<"Reading fusion data done\n";
	}


	int FusionUnit::readFusionList(){
		int output = indiviualFusion(priCard, secCard);
		if(output != 0)
			return output;
		//std::cout<<"completed first inidiv fuse read\n";
		output = indiviualFusion(secCard, priCard);
		if(output != 0)
			return output;
		//std::cout<<"completed second inidiv fuse read\n";
		output = generalFusion(priCard, secCard);
		if(output != 0)
			return output;
		//std::cout<<"completed first general fuse read\n";
		/*output = generalFusion(secCard, priCard);
		if(output != 0)
			return output;*/
		//std::cout<<"completed second general fuse read\n";
		return 0;
	}

	int FusionUnit::indiviualFusion(int pri, int sec){
		std::ifstream in("GameData/fusion/UniqueFusionsList.txt");
		int primaryCardNo = cards[pri].cardNumber;
		int primaryPosition = 0;
		int atkRequirement = 0;
		int atkMaximum = 0;
		int outputCard = 0;
		int otherCardNo;
		bool atEnd = false;
		//specific fusion
		do{
			if(waitForSymbol(&in) == '&')
				break;
			(in)>>primaryPosition;
			if(primaryCardNo == primaryPosition){
				do{
					in>>otherCardNo;
					in>>atkRequirement;
					in>>atkMaximum;
					in>>outputCard;
					if(otherCardNo==atkRequirement && atkRequirement==outputCard && outputCard==0){
						atEnd = true;
					}else{
						if(isGeneral(otherCardNo)){
							if(cards[sec].hasThisAttribute(otherCardNo) &&
								cards[sec].origAttack >= atkRequirement &&
								cards[sec].origAttack < atkMaximum){
									return outputCard;
							}
						}else{//two specific cards
							if(cards[sec].cardNumber == otherCardNo){
								return outputCard;
							}
						}
					}//not end marking
				}while(!atEnd);//going through all fusions with this specific card
			}//if right car number end

		}while(!atEnd);
		return 0;
	}

	int FusionUnit::generalFusion(int pri, int sec){
		int out = tryFuse(cards[pri], cards[sec]);
		if(out!=0)return out;
		return 0;
	}

	/*int FusionUnit::generalFusion(int pri, int sec){
		//std::cout<<"into general fusion\n";
		for(unsigned int priFuseIndex = 0; priFuseIndex < cards[pri].fusionTypes.size(); priFuseIndex++)
		{
			for(unsigned int secFuseIndex = 0; secFuseIndex < cards[sec].fusionTypes.size(); secFuseIndex++)
			{
				int primaryPosition = 0;
				int atkRequirement = 0;
				int atkMaximum = 0;
				int outputCard = 0;
				int otherCardNo;
				bool atEnd = false;
				bool littleEnd = false;
				std::ifstream in("GameData/fusion/GeneralFusionList.txt");
				do{
					if(waitForSymbol(&in) == '&'){
						atEnd = true;
						break;
					}
					in>>primaryPosition;
					if(cards[pri].hasThisAttribute(primaryPosition, priFuseIndex)){
						do{
							in>>otherCardNo;
							in>>atkRequirement;
							in>>atkMaximum;
							in>>outputCard;
							if(outputCard==0){
								littleEnd = true;
								//atEnd = true;
							}else{
								if(cards[sec].hasThisAttribute(otherCardNo, secFuseIndex) &&
									cards[sec].origAttack >= atkRequirement &&
									cards[pri].origAttack >= atkRequirement &&
									cards[sec].origAttack < atkMaximum &&
									cards[pri].origAttack < atkMaximum){
										return outputCard;
								}
							}//not at end of this attrib list
						}while(!littleEnd);
					}//match with priCard attrib
				}while(!atEnd);
			}
		}

		return 0;

	}*/

	bool FusionUnit::isGeneral(int number){
		return (number < 100);

	}

	bool FusionUnit::tryEquip(){
		return readEquipList();
	}
	bool FusionUnit::readEquipList(){
		return (readingEquipList(true)||readingEquipList(false));
	}

	bool FusionUnit::readingEquipList(bool back){
		int mCard = (back?secCard:priCard);
		int eCard = (back?priCard:secCard);
		if(cards[mCard].monMagTrap!=YUG_MONSTER_CARD && cards[eCard].monMagTrap!=YUG_MONSTER_CARD){
			return false;
		}
		int takenValue;
		int antiValue = 0;
		int andValue = 0;
		char lastSeenchar;
		bool atEnd = false;
		bool littleEnd = false;
		std::ifstream in("GameData/fusion/EquipList.txt");
		do{
			lastSeenchar = waitForSymbol(&in);
			if(lastSeenchar == '&' || in.eof()){
				atEnd = true;
				break;
			}
			if(lastSeenchar == '#'){//found next equip card listing
				in>>takenValue;
				if(takenValue == cards[eCard].cardNumber){//correct listing for this equip card
					do{
						lastSeenchar = waitForSymbol(&in);
						if(lastSeenchar == '~'){
							littleEnd = true;
							break;
						}
						if(lastSeenchar == '^'){
							in>>antiValue;
						}
						if(lastSeenchar == '%'){
							in>>andValue;
						}
						if(lastSeenchar == '='){
							in>>takenValue;
							if(cards[mCard].canUseEquip(takenValue)){
								if(antiValue==0 || !(cards[mCard].canUseEquip(antiValue))){
									if(andValue==0 ||cards[mCard].canUseEquip(andValue)){
										equipMonster = mCard;
										equipCard = eCard;
										return true;
									}
								}
							}
						}
					}while(!littleEnd);
				}
			}
		}while(!atEnd);
		return false;
	}

	bool FusionUnit::readingEquipList(Card::CardData& eqCard, Card::CardData& eqMon){
		if(eqMon.monMagTrap!=YUG_MONSTER_CARD && eqCard.monMagTrap!=YUG_MONSTER_CARD){
			return false;
		}
		int takenValue;
		int antiValue = 0;
		int andValue = 0;
		char lastSeenchar;
		bool atEnd = false;
		bool littleEnd = false;
		std::ifstream in("GameData/fusion/EquipList.txt");
		do{
			lastSeenchar = waitForSymbol(&in);
			if(lastSeenchar == '&' || in.eof()){
				atEnd = true;
				break;
			}
			if(lastSeenchar == '#'){//found next equip card listing
				in>>takenValue;
				if(takenValue == eqCard.cardNumber){//correct listing for this equip card
					do{
						lastSeenchar = waitForSymbol(&in);
						if(lastSeenchar == '~'){
							littleEnd = true;
							break;
						}
						if(lastSeenchar == '^'){
							in>>antiValue;
						}
						if(lastSeenchar == '%'){
							in>>andValue;
						}
						if(lastSeenchar == '='){
							in>>takenValue;
							if(eqMon.canUseEquip(takenValue)){
								if(antiValue==0 || !(eqMon.canUseEquip(antiValue))){
									if(andValue==0 ||eqMon.canUseEquip(andValue)){
										return true;
									}
								}
							}
						}
					}while(!littleEnd);
				}
			}
		}while(!atEnd);
		return false;
	}

	char FusionUnit::waitForSymbol(std::ifstream* in){
		char ch;
		while(true){
			ch = in->get();
			if(ch == '#')
				return '#';
			if(ch == '&')
				return '&';
			if(ch == '^')
				return '^';
			if(ch == '%')
				return '%';
			if(ch == '~')
				return '~';
			if(ch == '=')
				return '=';
		}

	}

	int FusionUnit::indiviualFusion(Card::CardData& c1, Card::CardData& c2){
		std::ifstream in("GameData/fusion/UniqueFusionsList.txt");
		int primaryCardNo = c1.cardNumber;
		int primaryPosition = 0;
		int atkRequirement = 0;
		int atkMaximum = 0;
		int outputCard = 0;
		int otherCardNo;
		bool atEnd = false;
		//specific fusion
		do{
			if(waitForSymbol(&in) == '&')
				break;
			(in)>>primaryPosition;
			if(primaryCardNo == primaryPosition){
				do{
					in>>otherCardNo;
					in>>atkRequirement;
					in>>atkMaximum;
					in>>outputCard;
					if(otherCardNo==atkRequirement && atkRequirement==outputCard && outputCard==0){
						atEnd = true;
					}else{
						if(isGeneral(otherCardNo)){
							if(c2.hasThisAttribute(otherCardNo) &&
								c2.origAttack >= atkRequirement &&
								c2.origAttack < atkMaximum){
									return outputCard;
							}
						}else{//two specific cards
							if(c2.cardNumber == otherCardNo){
								return outputCard;
							}
						}
					}//not end marking
				}while(!atEnd);//going through all fusions with this specific card
			}//if right car number end

		}while(!atEnd);
		return 0;
	}
	
	int FusionUnit::generalFusion(Card::CardData& c1, Card::CardData& c2){
		int out = tryFuse(c1, c2);
		if(out!=0)return out;
		return 0;
	}

	/*int FusionUnit::generalFusion(Card::CardData& c1, Card::CardData& c2){
		//std::cout<<"into general fusion\n";
		for(unsigned int priFuseIndex = 0; priFuseIndex < c1.fusionTypes.size(); priFuseIndex++)
		{
			for(unsigned int secFuseIndex = 0; secFuseIndex < c2.fusionTypes.size(); secFuseIndex++)
			{
				int primaryPosition = 0;
				int atkRequirement = 0;
				int atkMaximum = 0;
				int outputCard = 0;
				int otherCardNo;
				bool atEnd = false;
				bool littleEnd = false;
				std::ifstream in("GameData/fusion/GeneralFusionList.txt");
				do{
					if(waitForSymbol(&in) == '&'){
						atEnd = true;
						break;
					}
					in>>primaryPosition;
					if(c1.hasThisAttribute(primaryPosition, priFuseIndex)){
						do{
							in>>otherCardNo;
							in>>atkRequirement;
							in>>atkMaximum;
							in>>outputCard;
							if(outputCard==0){
								littleEnd = true;
								//atEnd = true;
							}else{
								if(c2.hasThisAttribute(otherCardNo, secFuseIndex) &&
									c2.origAttack >= atkRequirement &&
									c1.origAttack >= atkRequirement &&
									c2.origAttack < atkMaximum &&
									c1.origAttack < atkMaximum){
										return outputCard;
								}
							}//not at end of this attrib list
						}while(!littleEnd);
					}//match with priCard attrib
				}while(!atEnd);
			}
		}

		return 0;

	}*/


	void FusionUnit::createFusionLists(){
		std::ifstream in("GameData/fusion/GeneralFusionList.txt");
		int holderInt = 0;
		int lowAttack = 0;
		int highAttack = 0;
		int secondType = 0;
		int outputCard = 0;
		while(true){
			char sym = waitForSymbol(&in);
			if(sym== '&')
				break;
			in>>holderInt;
			std::vector<FusionInfo> newList;
			while(true){
				in>>secondType; in>>lowAttack; in>>highAttack; in>>outputCard;
				if(secondType==lowAttack && outputCard==0)
					break;
				newList.push_back(FusionInfo(secondType, lowAttack, highAttack, outputCard));
			}
			//std::cout<<"FusionUnit::createFusionLists: newList.size = "<<newList.size()<<std::endl;
			generalLists.push_back(newList);
		}
		//std::cout<<"FusionUnit::createFusionLists: generalList.size = "<<generalLists.size()<<std::endl;
	}

	int FusionUnit::tryFuse(Card::CardData& c1, Card::CardData& c2){
		int i = tryFuse2(c1, c2);
		if(i != 0) return i;
		return tryFuse2(c2, c1);
	}
	int FusionUnit::tryFuse2(Card::CardData& c1, Card::CardData& c2){
		int indexCount = 0;
		//std::cout<<"FusionUnit::tryFuse2: c1.size= "<<c1.fusionTypes.size()<<" c2.size= "<<c2.fusionTypes.size()<<std::endl;
		while(true){
			if(indexCount >= c1.fusionTypes.size())//index hasn't overrun first card
				return leftOver(c2, c1, indexCount);
			if(indexCount >= c2.fusionTypes.size())
				return leftOver(c1, c2, indexCount);
			for(int n = 0; n <= indexCount-1; n++){//have used fusion list catch up to new index
				//std::cout<<"FusionUnit::tryFuse2: first for: n= :"<<n<<"looking at: ("<<n<<", "<<indexCount<<")\n";
				int out = tryFuse3(n, indexCount, c1, c2);
				if(out!=0) return out;
			}
			for(int n = 0; n <= indexCount; n++){//have new c1 fusion part try with up-to-index c2 fusion list parts
				//std::cout<<"FusionUnit::tryFuse2: second for: n= :"<<n<<"looking at: ("<<indexCount<<", "<<n<<")\n";
				int out = tryFuse3(indexCount, n, c1, c2);
				if(out!=0) return out;
			}
			indexCount++;
		}
		return 0;
	}
	int FusionUnit::tryFuse3(int primaryIndex, int secondIndex, Card::CardData& primaryCard, Card::CardData& secondCard){
		int primaryAttrib = primaryCard.fusionTypes[primaryIndex]-10;
		if(primaryAttrib<0 || primaryAttrib >=generalLists.size()){
			//std::cout<<"tryFuse3: out of bounds primaryAttrib = "<<primaryAttrib<<std::endl;
			//std::cout<<"Card is: "<<primaryCard.cardNumber<<std::endl;
			return 0;
		}
		//std::cout<<"FusionUnit::tryFuse3: priAtb:"<<primaryAttrib<<std::endl;
		for(unsigned int index = 0; index<fusionUnit.generalLists[primaryAttrib].size(); index++){
			FusionInfo fi = fusionUnit.generalLists[primaryAttrib][index];
			//std::cout<<"FusionUnit::tryFuse3: scdAtb= "<<secondCard.fusionTypes[secondIndex]<<" secondType="<<fi.secondType<<std::endl;
			//std::cout<<"priAtk= "<<primaryCard.origAttack<<" secAtk= "<<secondCard.origAttack<<std::endl;
			//std::cout<<"atk Range= "<<fi.lowerAttack<<" : "<<fi.upperAttack<<std::endl;
			if(fi.secondType == secondCard.fusionTypes[secondIndex] && fi.checkAtkRange(primaryCard.origAttack, secondCard.origAttack)){
				return fi.outputCard;
			}
		}
		return 0;
	}
	int FusionUnit::leftOver(Card::CardData& longCard, Card::CardData& shortCard, int indexCount){
		for(unsigned int index = indexCount; index < longCard.fusionTypes.size(); index++){
			for(unsigned int index2 = 0; index2 < shortCard.fusionTypes.size(); index2++){
				//std::cout<<"FusionUnit::leftover: looking at: ("<<index<<", "<<index2<<")\n";
				int out = tryFuse3(index, index2, longCard, shortCard);
				if(out!=0) return out;
			}
		}
		return 0;
	}
}