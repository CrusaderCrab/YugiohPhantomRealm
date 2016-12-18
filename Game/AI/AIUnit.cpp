#include <Game\AI\AIUnit.h>
#include <Utility\InputUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Duel\FusionUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Duel\Parts\SwordUnit.h>
#include <Utility\Clock.h>
#include <Game\EnemyData.h>
#include <iostream>

namespace AI{

	AICard::AICard() : firstCardOnBoard(false) {}
	AICard::AICard(Card::CardData* c) : firstCardOnBoard(false), card(*c) {}


	void AIUnit::startup(){
		moveIndex = 0;

		for(unsigned int i = 0; i < 5; i++)
			for(unsigned int j = 0; j < 4; j++)
				boardMarks[i][j] = YUG_AI_NO_MARK;
		bestAttack = false; canBreak = false; 
		canHold = false; attackMagics = false;
		theirHighestAtk = YUG_AI_NO_MARK; theirAtkCard = YUG_AI_NO_MARK;
		theirHighestDef = YUG_AI_NO_MARK; theirDefCard = YUG_AI_NO_MARK;
		myHighestAtk = YUG_AI_NO_MARK; myAtkCard = YUG_AI_NO_MARK;
		myHighestDef = YUG_AI_NO_MARK; myDefCard = YUG_AI_NO_MARK;
		playedCon = YUG_AI_NO_MARK;
		playFaceUp = false; isFusing = false;
		currentCursorLocal[0] = YUG_AI_NO_MARK; currentCursorLocal[1] = YUG_AI_NO_MARK;
		currentHandCursorLocal = YUG_AI_NO_MARK;
		hasBoardDecision = false; hasHandDecision = false;
		lowStat = enemyData.low; highStat = enemyData.high; dangerStat = enemyData.dang;
		setUpHighLowStats();
	}
	void AIUnit::cleanup(){

	}
	void AIUnit::newTurnSetup(){
		handPositon = theBoard.enemy.cursorPosition;
		strongestCards.clear();
		moveList.clear();
		moveIndex = 0;
		bestAttack = false; canBreak = false; 
		canHold = false; attackMagics = false;
		theirHighestAtk = YUG_AI_NO_MARK; theirAtkCard = YUG_AI_NO_MARK;
		theirHighestDef = YUG_AI_NO_MARK; theirDefCard = YUG_AI_NO_MARK;
		myHighestAtk = YUG_AI_NO_MARK; myAtkCard = YUG_AI_NO_MARK;
		myHighestDef = YUG_AI_NO_MARK; myDefCard = YUG_AI_NO_MARK;
		playedCon = YUG_AI_NO_MARK;
		playFaceUp = false; isFusing = false;
		currentCursorLocal[0] = YUG_AI_NO_MARK; currentCursorLocal[1] = YUG_AI_NO_MARK;
		currentHandCursorLocal = YUG_AI_NO_MARK;
		hasBoardDecision = false; hasHandDecision = false;

	}
	void AIUnit::calcHandMove(){
		newTurnSetup();
		calcStrongest1Cards();
		calcStrongest2Cards();
		calcStrongest3Cards();
		calcStrongest4Cards();
		removeDuplicateStrongestCards();
		sortStrongestCardsAttack();
		//printStrongestCards();
		gameClock.newLoop();
		/*moveList.push_back(YUG_KEY_X);
		moveList.push_back(YUG_KEY_LEFT);
		moveList.push_back(YUG_KEY_X);
		moveList.push_back(YUG_KEY_X);
		moveList.push_back(YUG_KEY_X);*/
		playCard(getStrongestCard().posis);
	}
	void AIUnit::calcBoardMove(){
		moveList.clear();
		moveIndex = 0;
		/*if(!theBoard.board[0][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
			moveList.push_back(YUG_KEY_X);
			moveList.push_back(YUG_KEY_X);
		}
		moveList.push_back(YUG_KEY_START);*/
		fieldRecursion();
	}
	void AIUnit::dropCurrentKey(){

	}
	void AIUnit::pressNextKey(){
		inputUnit.releaseAllComputerKeys();
		if(!inputUnit.isWaiting){
			//std::cout<<"AI Pressed"<<moveList[moveIndex]<<"\nMoves left: ";
			/*for(unsigned int i = moveIndex+1; i < moveList.size(); i++){
				std::cout<<moveList[i]<<" ";
			}*/
			//std::cout<<"\n";
			inputUnit.computerKeyPress(moveList[moveIndex++]);
			//std::cout<<"Current board pos col = "<<theBoard.cP[0]<<" row = "<<theBoard.cP[1]<<std::endl;
			inputUnit.wait(0.5f);//0.5
		}
	}
	bool AIUnit::hasMovesLeft(){
		return moveIndex < moveList.size();
	}

	void AIUnit::setHighestCards(){
		for(unsigned int i = 0; i < 5; i++){
			//player highest atk and defense
			if(!isCardVisible(i)){
				boardMarks[i][YUG_BOARD_PLAYER_MON_ROW] = YUG_AI_CARD_DOWN;
			}else if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
				if(theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].attack>theirHighestAtk){
					theirHighestAtk = theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].attack;
					theirAtkCard = i;
				}
				if(theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].defense>theirHighestDef){
					theirHighestDef = theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].defense;
					theirDefCard = i;
				}
				if(theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].attack>=highValue){
					boardMarks[i][YUG_BOARD_PLAYER_MON_ROW] = YUG_AI_DANGEROUS;
				}else if(theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].defense>=highValue){
					boardMarks[i][YUG_BOARD_PLAYER_MON_ROW] = YUG_AI_NUISANCE;
				}
			}else{
				boardMarks[i][YUG_BOARD_PLAYER_MON_ROW] = YUG_AI_NO_MARK;
			}
			//ai highest atk and defense
			if(!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard()){
				if(theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].attack>myHighestAtk){
					myHighestAtk = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].attack;
					myAtkCard = i;
				}
				if(theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].defense>myHighestDef){
					myHighestDef = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].defense;
					myDefCard = i;
				}

			}else{
				boardMarks[i][YUG_BOARD_ENEMY_MON_ROW] = YUG_AI_NO_MARK;
			}
		}
	}

	void AIUnit::boardDoIHaveBestAttack(){
		for(unsigned int i = 0 ; i <  5; i++){
			for(unsigned int j = 0 ; j < 5; j++){
				Card::CardData& myC = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
				Card::CardData& thC = theBoard.board[j][YUG_BOARD_PLAYER_MON_ROW];
				if( !myC.blankCard() && !thC.blankCard() ){
					int a = differenceIfAtkFight(myC, j);
					if(a<0 && a!=YUG_AI_CARD_DOWN){
						bestAttack = false;
						return;
					}
				}
			}
		}
		bestAttack = true;
	}

	void AIUnit::doIHaveBestAttack(){
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard() && theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].attackMode){
				int a = differenceIfAtkFight((theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW]), theirAtkCard);
				if(a >= 0 || a == YUG_AI_CARD_DOWN){
					bestAttack = true;
					return;
				}
			}
		}
		bestAttack = false;
	}
	void AIUnit::canIBreakThrough(){
		if(noOfCardsIn(YUG_BOARD_PLAYER_MON_ROW)==0){
			canBreak = true; return;
		}
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard() && !canBreakTheirCard(i)){
				canBreak = false;
				return;
			}
		}
		canBreak = true;
	}
	bool AIUnit::canBreakTheirCard(int enemyPos){
		if(!isCardVisible(enemyPos))
			return true;
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard()){
				if(0 <= differenceIfCurrentFight(theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW],enemyPos)){
					return true;
				}
			}
		}
		return false;
	}
	void AIUnit::canIHoldHim(){
		if(noOfCardsIn(YUG_BOARD_PLAYER_MON_ROW)==0){
			canBreak = true; return;
		}
		for(unsigned int i = 0; i < 5; i++){
			if( !canBreakMyCard(i) ){
				canHold = true;
				return;
			}
		}
		canHold = false;
	}
	bool AIUnit::canBreakMyCard(int myCol){
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
				int res = differenceIfBestFight(theBoard.board[myCol][YUG_BOARD_ENEMY_MON_ROW],i);

				if(res <= 0 && res != YUG_AI_CARD_DOWN ){
					return true;
				}
			}
		}
		return false;
	}
	void AIUnit::doIHaveAttackMagic(){
		for(unsigned int i = 0; i < 5; i++){
			if( isAttackMagic(i)){
				attackMagics = true;
				return;
			}
		}
		attackMagics = false;
	}

	bool AIUnit::isEnemyBoardMonsterless(){
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
				return false;
			}
		}
		return true;
	}
	bool AIUnit::isEnemyBoardMagicless(){
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MAG_ROW].blankCard()){
				return false;
			}
		}
		return true;
	}
	bool AIUnit::isEnemyBoardEmpty(){
		return (isEnemyBoardMagicless() || isEnemyBoardMonsterless());
	}
	bool AIUnit::isEnemyBoardTrapless(){
		for(unsigned int i = 0; i < 5; i++){
			if(boardMarks[i][YUG_BOARD_PLAYER_MAG_ROW] == YUG_AI_IS_TRAP){
				return false;
			}
		}
		return true;
	}
	int AIUnit::myNoOfMonsterCards(){
		int count = 0;
		for(unsigned int i = 0 ; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard()){
				count++;
			}
		}
		return count;
	}


	bool AIUnit::isAttackMagic(int myCol){
		return isAttackMagic( theBoard.board[myCol][YUG_BOARD_ENEMY_MAG_ROW]);	
	}

	int AIUnit::differenceIfAtkFight(Card::CardData& myC, int theirCol){
		if(!isCardVisible(theirCol))
			return YUG_AI_CARD_DOWN;
		int ret = 0;
		Card::CardData& theirC = theBoard.board[theirCol][YUG_BOARD_PLAYER_MON_ROW];
		int myA = myC.attack;
		int theirA = theirC.attack;
		if(myC.currentConstellation == YUG_CARD_NO_CURRENT_CON){
			if(versusConstellation(myC.constellations[0],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else if(versusConstellation(myC.constellations[1],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else{
				return myA - theirA;
			}
		}else{
			int con = myC.versusConstellation(theirC.currentConstellation);
			if(con == YUG_WEAK_AGAINST)
				return (myA-500) - theirA;
			if(con == YUG_STRONG_AGAINST)
				return (myA+500) - theirA;
			return myA - theirA;
		}
	}
	int AIUnit::differenceIfDefending(Card::CardData& myC, int theirCol){
		if(!isCardVisible(theirCol))
			return YUG_AI_CARD_DOWN;
		int ret = 0;
		Card::CardData& theirC = theBoard.board[theirCol][YUG_BOARD_PLAYER_MON_ROW];
		int myA = myC.defense;
		int theirA = theirC.attack;
		if(myC.currentConstellation == YUG_CARD_NO_CURRENT_CON){
			if(versusConstellation(myC.constellations[0],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else if(versusConstellation(myC.constellations[1],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else{
				return myA - theirA;
			}
		}else{
			int con = myC.versusConstellation(theirC.currentConstellation);
			if(con == YUG_WEAK_AGAINST)
				return (myA-500) - theirA;
			if(con == YUG_STRONG_AGAINST)
				return (myA+500) - theirA;
			return myA - theirA;
		}
	}
	int AIUnit::differenceIfDefFight(Card::CardData& myC, int theirCol){
		if(!isCardVisible(theirCol))
			return YUG_AI_CARD_DOWN;
		int ret = 0;
		Card::CardData& theirC = theBoard.board[theirCol][YUG_BOARD_PLAYER_MON_ROW];
		int myA = myC.attack;
		int theirA = theirC.defense;
		if(myC.currentConstellation == YUG_CARD_NO_CURRENT_CON){
			if(versusConstellation(myC.constellations[0],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else if(versusConstellation(myC.constellations[1],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else{
				return myA - theirA;
			}
		}else{
			int con = myC.versusConstellation(theirC.currentConstellation);
			if(con == YUG_WEAK_AGAINST)
				return (myA-500) - theirA;
			if(con == YUG_STRONG_AGAINST)
				return (myA+500) - theirA;
			return myA - theirA;
		}
	}
	int AIUnit::differenceIfBestFight(Card::CardData& myC, int theirCol){
		int atkRes = differenceIfAtkFight(myC, theirCol);
		int defRes = differenceIfDefending(myC, theirCol);
		return ( atkRes>defRes ? atkRes:defRes );
	}
	int AIUnit::differenceIfCurrentFight(Card::CardData& myC, int theirCol){
		Card::CardData& theirC = theBoard.board[theirCol][YUG_BOARD_PLAYER_MON_ROW];
		if(theirC.attackMode){
			return differenceIfAtkFight(myC, theirCol);
		}else{
			return differenceIfDefFight(myC, theirCol);
		}
	}
	int AIUnit::damageFromAtkFight(Card::CardData& myC, Card::CardData& thC){
		if(!thC.faceUp)
			return YUG_AI_CARD_DOWN;
		int ret = 0;
		Card::CardData& theirC = thC;
		int myA = myC.attack;
		int theirA = theirC.attack;
		if(myC.currentConstellation == YUG_CARD_NO_CURRENT_CON){
			if(versusConstellation(myC.constellations[0],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else if(versusConstellation(myC.constellations[1],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else{
				return myA - theirA;
			}
		}else{
			int con = myC.versusConstellation(theirC.currentConstellation);
			if(con == YUG_WEAK_AGAINST)
				return (myA-500) - theirA;
			if(con == YUG_STRONG_AGAINST)
				return (myA+500) - theirA;
			return myA - theirA;
		}
	}
	int AIUnit::differenceFromDefFight(Card::CardData& myC, Card::CardData& thC){
		if(!thC.faceUp)
			return YUG_AI_CARD_DOWN;
		int ret = 0;
		Card::CardData& theirC = thC;
		int myA = myC.attack;
		int theirA = theirC.defense;
		if(myC.currentConstellation == YUG_CARD_NO_CURRENT_CON){
			if(versusConstellation(myC.constellations[0],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else if(versusConstellation(myC.constellations[1],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else{
				return myA - theirA;
			}
		}else{
			int con = myC.versusConstellation(theirC.currentConstellation);
			if(con == YUG_WEAK_AGAINST)
				return (myA-500) - theirA;
			if(con == YUG_STRONG_AGAINST)
				return (myA+500) - theirA;
			return myA - theirA;
		}
	}
	int AIUnit::damageFromFight(Card::CardData& myC, Card::CardData& thC){
		Card::CardData& theirC = thC;
		if(theirC.attackMode){
			return damageFromAtkFight(myC, thC);
		}else{
			int i = differenceFromDefFight(myC, thC);
			if(i > 0) return 0;
			return i;
		}
	}

	void AIUnit::calculateStrongestCards(){


	}
	void AIUnit::calcStrongest1Cards(){
		for(unsigned int i = 0; i < 5; i++){
			if(theBoard.enemy.hand[i].monMagTrap == YUG_MONSTER_CARD){
				AICard a(&theBoard.enemy.hand[i]);
				a.posis.push_back(i);
				strongestCards.push_back(a);
			}
		}
	}
	void AIUnit::calcStrongest2Cards(){
		for(unsigned int i = 0; i < 5; i++){
			for(unsigned int j = 0; j < 5; j++){
				if(i!=j){
					int no = fusionUnit.indiviualFusion(theBoard.enemy.hand[i],theBoard.enemy.hand[j]);
					if(no == 0){
						no = fusionUnit.generalFusion(theBoard.enemy.hand[i],theBoard.enemy.hand[j]);
						if(no != 0){
							Card::CardData c = cardCreator.createCard(no);//put in general card
							theBoard.field.applyField(&c);
							AICard a(&c);
							a.posis.push_back(i); a.posis.push_back(j);
							strongestCards.push_back(a);
						}else{
							Card::CardData c = cardCreator.blankCard();//put in blank card
							AICard a(&c);
							a.posis.push_back(i); a.posis.push_back(j);
							strongestCards.push_back(a);
						}
					}else{
						Card::CardData c = cardCreator.createCard(no);//put in individ card
						theBoard.field.applyField(&c);
						AICard a(&c);
						a.posis.push_back(i); a.posis.push_back(j);
						strongestCards.push_back(a);
					}
				}//end if
			}//for j
		}//for i
	}
	void AIUnit::calcStrongest2BoardCards(){
		for(unsigned int i = 0; i < 5; i++){
			for(unsigned int j = 0; j < 5; j++){
				Card::CardData& boardCard = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
				if(!boardCard.blankCard()){
					int no = fusionUnit.indiviualFusion(boardCard, theBoard.enemy.hand[j]);
					if(no == 0){
						no = fusionUnit.generalFusion(boardCard, theBoard.enemy.hand[j]);
						if(no != 0){
							Card::CardData c = cardCreator.createCard(no);//put in general card
							theBoard.field.applyField(&c);
							AICard a(&c);
							a.posis.push_back(-(i+1)); a.posis.push_back(j);
							strongestCards.push_back(a);
						}else{
							Card::CardData c = cardCreator.blankCard();//put in blank card
							AICard a(&c);
							a.posis.push_back(-(i+1)); a.posis.push_back(j);
							strongestCards.push_back(a);
						}
					}else{
						Card::CardData c = cardCreator.createCard(no);//put in individ card
						theBoard.field.applyField(&c);
						AICard a(&c);
						a.posis.push_back(-(i+1)); a.posis.push_back(j);
						strongestCards.push_back(a);
					}
				}
			}//for j
		}//for i
	}

	void AIUnit::calcStrongest3Cards(){
		for(unsigned int i = 0; i < 5; i++){
			for(unsigned int j = 0; j < 5; j++){
				if(i!=j){
					for(unsigned int k = 0; k<5; k++){
						if(k!=i && k!=j){
							calc3CardFusion(i,j,k);
						}
					}
				}//if bracket
			}
		}
	}
	void AIUnit::calcStrongest3BoardCards(){
		for(unsigned int i = 0; i < 5; i++){
			for(unsigned int j = 0; j < 5; j++){
				if( !theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard() ){
					for(unsigned int k = 0; k<5; k++){
						if( k!=j ){
							calc3BoardCardFusion(-(i+1),j,k);
						}
					}
				}//if bracket
			}
		}
	}

	void AIUnit::calc3CardFusion(int i, int j, int k){
		AICard a = getAICard(i,j);
		if(a.card.blankCard()){
			a.posis.push_back(k);
			strongestCards.push_back(a);
		}else{
			int no = fusionUnit.indiviualFusion(a.card,theBoard.enemy.hand[k]);
			if(no == 0){
				no = fusionUnit.generalFusion(a.card,theBoard.enemy.hand[k]);
				if(no != 0){
					Card::CardData c = cardCreator.createCard(no);//put in general card
					theBoard.field.applyField(&c);
					a.card = c; a.posis.push_back(k);
					strongestCards.push_back(a);
				}else{
					Card::CardData c = cardCreator.blankCard();//put in blank card
					a.card = c;  a.posis.push_back(k);
					strongestCards.push_back(a);
				}
			}else{
				Card::CardData c = cardCreator.createCard(no);//put in individ card
				theBoard.field.applyField(&c);
				a.card = c; a.posis.push_back(k);
				strongestCards.push_back(a);
			}
		}
	}
	void AIUnit::calc3BoardCardFusion(int i, int j, int k){
		AICard a = getAICard(i,j);
		if(a.card.blankCard()){
			a.posis.push_back(k);
			strongestCards.push_back(a);
		}else{
			int no = fusionUnit.indiviualFusion(a.card,theBoard.enemy.hand[k]);
			if(no == 0){
				no = fusionUnit.generalFusion(a.card,theBoard.enemy.hand[k]);
				if(no != 0){
					Card::CardData c = cardCreator.createCard(no);//put in general card
					theBoard.field.applyField(&c);
					a.card = c; a.posis.push_back(k);
					strongestCards.push_back(a);
				}else{
					Card::CardData c = cardCreator.blankCard();//put in blank card
					a.card = c;  a.posis.push_back(k);
					strongestCards.push_back(a);
				}
			}else{
				Card::CardData c = cardCreator.createCard(no);//put in individ card
				theBoard.field.applyField(&c);
				a.card = c; a.posis.push_back(k);
				strongestCards.push_back(a);
			}
		}
	}

	AICard AIUnit::getAICard(int a, int b){
		for(unsigned int i = 0; i < strongestCards.size(); i++){
			if(strongestCards[i].posis.size() == 2){
				if( strongestCards[i].posis[0] == a &&
					strongestCards[i].posis[1] == b){
						return strongestCards[i];
				}
			}
		}
		AICard ac; ac.card = cardCreator.blankCard();
		return ac;
	}

	void AIUnit::calcStrongest4Cards(){
		for(unsigned int i = 0; i < 5; i++){
			for(unsigned int j = 0; j < 5; j++){
				if(i!=j){
					for(unsigned int k = 0; k<5; k++){
						if(k!=i && k!=j){
							for(unsigned int m = 0; m < 5; m++){
								if(m!=i && m!=j && m!=k){
									calc4CardFusion(i,j,k,m);
								}
							}
						}//if bracket k
					}
				}//if bracket j
			}
		}//for int i
	}

	void AIUnit::calc4CardFusion(int i, int j, int k, int m){
		AICard a = getAICard(i,j,k);
		if(a.card.blankCard()){
			a.posis.push_back(m);
			strongestCards.push_back(a);
		}else{
			int no = fusionUnit.indiviualFusion(a.card,theBoard.enemy.hand[m]);
			if(no == 0){
				no = fusionUnit.generalFusion(a.card,theBoard.enemy.hand[m]);
				if(no != 0){
					Card::CardData c = cardCreator.createCard(no);//put in general card
					theBoard.field.applyField(&c);
					a.card = c; a.posis.push_back(m);
					strongestCards.push_back(a);
				}else{
					Card::CardData c = cardCreator.blankCard();//put in blank card
					a.card = c;  a.posis.push_back(m);
					strongestCards.push_back(a);
				}
			}else{
				Card::CardData c = cardCreator.createCard(no);//put in individ card
				theBoard.field.applyField(&c);
				a.card = c; a.posis.push_back(m);
				strongestCards.push_back(a);
			}
		}
	}

	AICard AIUnit::getAICard(int a, int b, int c){
		for(unsigned int i = 0; i < strongestCards.size(); i++){
			if(strongestCards[i].posis.size() == 3){
				if( strongestCards[i].posis[0] == a &&
					strongestCards[i].posis[1] == b &&
					strongestCards[i].posis[2] == c){
						return strongestCards[i];
				}
			}
		}
		AICard ac; ac.card = cardCreator.blankCard();
		return ac;
	}

	void AIUnit::removeDuplicateStrongestCards(){
		for(unsigned int i = 0; i < strongestCards.size(); i++){
			if(! strongestCards[i].card.blankCard() ){
				for(unsigned j = i+1; j < strongestCards.size(); j++){
					if(strongestCards[j].card.cardNumber == strongestCards[i].card.cardNumber){
						if(strongestCards[i].posis.size() < strongestCards[j].posis.size()){
							strongestCards[j].card = cardCreator.blankCard();
						}else{
							strongestCards[i].card = cardCreator.blankCard();
						}
					}
				}//inner for
			}
		}//outer for
	}

	void AIUnit::sortStrongestCardsAttack(){
		for(unsigned int i = 0; i < strongestCards.size(); i++){
			if(! strongestCards[i].card.blankCard() ){
				for(unsigned int j = i+1; j < strongestCards.size(); j++){
					if( strongestCards[j].card.blankCard() ||
						strongestCards[i].card.attack > strongestCards[j].card.attack){
							AICard temp = strongestCards[j];
							strongestCards[j] = strongestCards[i];
							strongestCards[i] = temp;
					}
				}
			}
		}
	}
	void AIUnit::sortStrongestCardsDefense(){
		for(unsigned int i = 0; i < strongestCards.size(); i++){
			if(! strongestCards[i].card.blankCard() ){
				for(unsigned int j = i+1; j < strongestCards.size(); j++){
					if( strongestCards[j].card.blankCard() ||
						strongestCards[i].card.defense > strongestCards[j].card.defense){
							AICard temp = strongestCards[j];
							strongestCards[j] = strongestCards[i];
							strongestCards[i] = temp;
					}
				}
			}
		}
	}
	AICard AIUnit::getStrongestCard(){
		if(strongestCards.size()==0){
			AICard a; a.posis.push_back(0);
			a.card = cardCreator.createCard(150);
			return a;
		}
		return strongestCards[strongestCards.size()-1];
	}

	void AIUnit::printStrongestCards(){
		//std::cout<<"---AI strongest card listings---\n";
		int count = strongestCards.size()-10;
		if(count<-3)count = 0;
		for(unsigned int i = count; i < strongestCards.size(); i++){
			AICard& a = strongestCards[i];
			/*if(!a.card.blankCard()){
				std::cout<<a.card.name.data()<<": ";
			}else{
				std::cout<<"Blank Card: ";
			}
			std::cout<<"\t\t\t";
			for(unsigned int j = 0; j < a.posis.size(); j++){
				std::cout<<a.posis[j]+1<<", ";
			}
			std::cout<<std::endl;*/
		}
	}

	bool AIUnit::enemyHasTraps(){
		for(unsigned int i = 0; i < 5; i++){
			if(boardMarks[i][YUG_BOARD_PLAYER_MAG_ROW] == YUG_AI_IS_TRAP){
				return true;
			}
		}
		return false;
	}
	int AIUnit::noOfEnemyMagics(){
		int count = 0;
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MAG_ROW].blankCard()){
				++count;
			}
		}
		return count;
	}
	int AIUnit::canPlayHarpies(){
		for(unsigned int i = 0; i < 5; i++){
			if(theBoard.enemy.hand[i].cardNumber == 807){//== harpies
				return i;
			}
		}
		return YUG_AI_NO_MARK;
	}

	int AIUnit::hasSupportMagics(bool& playThisCardFaceup){
		int i = hasField();
		if(i != YUG_AI_NO_MARK && !overlappingFields(i)){
			playThisCardFaceup = true;
			return i;
		}
		i = hasMiscMagic();
		if(i != YUG_AI_NO_MARK){
			playThisCardFaceup = false;
			return i;
		}
		i = hasEquip();
		if(i != YUG_AI_NO_MARK){
			playThisCardFaceup = false;
			return i;
		}
		i = hasHealMagic();
		if(i != YUG_AI_NO_MARK){
			playThisCardFaceup = true;
			return i;
		}
		i = hasDamageMagic();
		if(i != YUG_AI_NO_MARK){
			playThisCardFaceup = true;
			return i;
		}
		return YUG_AI_NO_MARK;
	}
	bool AIUnit::overlappingFields(int handPos){
		return theBoard.field.currentField == theBoard.enemy.hand[handPos].cardNumber;
	}
	int AIUnit::hasField(){
		for(unsigned int i = 0; i < 5; i++)
			if(isFieldCard(theBoard.enemy.hand[i])) return i;
		return YUG_AI_NO_MARK;
	}
	int AIUnit::hasDamageMagic(){
		for(unsigned int i = 0; i < 5; i++)
			if(isDamageMagic(theBoard.enemy.hand[i])) return i;
		return YUG_AI_NO_MARK;
	}
	int AIUnit::hasHealMagic(){
		for(unsigned int i = 0; i < 5; i++)
			if(isHealMagic(theBoard.enemy.hand[i])) return i;
		return YUG_AI_NO_MARK;
	}
	int AIUnit::hasEquip(){
		for(unsigned int i = 0; i < 5; i++)
			if(isEquipCard(theBoard.enemy.hand[i])) return i;
		return YUG_AI_NO_MARK;
	}
	int AIUnit::hasMiscMagic(){
		for(unsigned int i = 0; i < 5; i++)
			if(isMiscMagic(theBoard.enemy.hand[i])) return i;
		return YUG_AI_NO_MARK;
	}
	int AIUnit::hasDefensiveTrap(){
		for(unsigned int i = 0; i < 5; i++)
			if(isDefensiveTrap(theBoard.enemy.hand[i])) return i;
		return YUG_AI_NO_MARK;
	}

	bool AIUnit::isFieldCard(Card::CardData& c){
		return (c.cardNumber == 338 || ( 839 <= c.cardNumber&&c.cardNumber <= 843));
	}
	bool AIUnit::isDamageMagic(Card::CardData& c){
		return (822 <= c.cardNumber && c.cardNumber <= 826);
	}
	bool AIUnit::isHealMagic(Card::CardData& c){
		return (817 <= c.cardNumber && c.cardNumber <= 821);
	}
	bool AIUnit::isEquipCard(Card::CardData& c){
		return c.monMagTrap == YUG_EQUIP_CARD;
	}
	bool AIUnit::isMiscMagic(Card::CardData& c){
		int no = c.cardNumber;
		return ( 
			   no == 801 || no == 808 || no == 781 || no == 807 
			|| no == 829 || no == 830 || no == 832 || no == 833
			|| no == 834 || no == 837);  

	}
	bool AIUnit::isDefensiveTrap(Card::CardData& c){
		int no = c.cardNumber;
		return ( 780==no || 835==no || 836==no ); 
	}
	void AIUnit::printCards(std::vector<Card::CardData>& c){
		/*std::cout<<"Printing array\n";
		for(unsigned int i = 0; i < c.size(); i++){
			std::cout<<'\t'<<c[i].name.data()<<std::endl;
		}*/
	}
	bool AIUnit::canWin(){
		//std::cout<<"AI : canWin()1\n";
		int damage = 0;
		std::vector<Card::CardData> ec = getCardsFromRow(YUG_BOARD_PLAYER_MON_ROW);
		std::vector<Card::CardData> mc = getCardsFromRow(YUG_BOARD_ENEMY_MON_ROW);
		//std::cout<<"AI : canWin()1.5\n";
		mc.push_back(getStrongestCard().card);
		//std::cout<<"AI : canWin()2\n";
		sortCardsByCurrentStat(ec);
		sortCardsByAtkStat(mc);
		printCards(ec); printCards(mc);
		if(mc.size() == 6) mc[5].attack = 0;
		//std::cout<<"AI : canWin()3\n";
		for(int i = ec.size()-1; i >= 0; i--){
			bool beatCard = false;
			for(unsigned int j = 0; j < mc.size(); j++){
				int d = damageFromFight(mc[j], ec[i]);
				//std::cout<<"Damage from: "<<mc[j].name.data()<<j<<" vs "
				//	     <<ec[i].name.data()<<i<<" = "<<d<<std::endl;
				if(d >= 0){
					mc[j].attack = 0;
					damage += d;
					beatCard = true;
					break;
				}
				if(d == YUG_AI_CARD_DOWN){
					mc[j].attack = 0;
					beatCard = true;
					break;
				}
			}
			if(!beatCard) return false;
		}
		for(unsigned int j = 0; j < mc.size(); j++){
			damage += mc[j].attack;
		}
		return (damage >= theBoard.player.lifepoints);
		
	}

	std::vector<Card::CardData> AIUnit::getCardsFromRow(int row){
		std::vector<Card::CardData> v;
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][row].blankCard())
				v.push_back(theBoard.board[i][row]);
		}
		return v;
	}
	void AIUnit::sortCardsByCurrentStat(std::vector<Card::CardData>& v){
		int sz = v.size();
		for(unsigned int i = 0; i < sz; i++){
			for(unsigned int j = i+1; j < sz; j++){
				int s1 = getCurrentStat(v[i]);
				int s2 = getCurrentStat(v[j]);
				if(s1 > s2){
					Card::CardData temp = v[j];
					v[j] = v[i];
					v[i] = temp;
				}
			}
		}
	}
	void AIUnit::sortCardsByAtkStat(std::vector<Card::CardData>& v){
		int sz = v.size();
		for(unsigned int i = 0; i < sz; i++){
			for(unsigned int j = i+1; j < sz; j++){
				int s1 = v[i].attack;
				int s2 = v[j].attack;
				if(s1 > s2){
					Card::CardData temp = v[j];
					v[j] = v[i];
					v[i] = temp;
				}
			}
		}
	}
	void AIUnit::sortCardsByDefStat(std::vector<Card::CardData>& v){
		int sz = v.size();
		for(unsigned int i = 0; i < sz; i++){
			for(unsigned int j = i+1; j < sz; j++){
				int s1 = v[i].defense;
				int s2 = v[j].defense;
				if(s1 > s2){
					Card::CardData temp = v[i];
					v[j] = v[i];
					v[i] = temp;
				}
			}
		}
	}

	int AIUnit::getCurrentStat(const Card::CardData& c){
		if(c.attackMode)
			return c.attack;
		return c.defense;
	}

	void AIUnit::playStrongestCard(){
		/*std::cout<<"Strongest card : "<<getStrongestCard().card.name.data()<<std::endl;
		for(unsigned int i = 0; i < getStrongestCard().posis.size(); i++)
			std::cout<<getStrongestCard().posis[i]<<" ";
		std::cout<<" <- numbers\n";*/
		if(strongestCards.size() > 0){
			tryToAddEquipCard( strongestCards[strongestCards.size()-1] );
		}
		playCard(getStrongestCard().posis);
	}

	void AIUnit::playMagicCard(int posi){
		cursorTo(posi);
		moveList.push_back(YUG_KEY_X);//commit card
		moveList.push_back(YUG_KEY_LEFT);//flip
		moveList.push_back(YUG_KEY_X);//activate

	}

	void AIUnit::playCard(int handPos){
		cursorTo(handPos);
		moveList.push_back(YUG_KEY_X);//commit card
			if(playFaceUp)
				moveList.push_back(YUG_KEY_LEFT);
		moveList.push_back(YUG_KEY_X);//agree to face

		//moveList.push_back(YUG_KEY_X);//play on chosen space
		if(theBoard.enemy.hand[handPos].monMagTrap == YUG_MONSTER_CARD){
			pressPlayPosition(theBoard.enemy.hand[handPos]);
		}else{
			moveList.push_back(YUG_KEY_X);
		}

		if(playedCon == YUG_AI_BOT_CON)
			moveList.push_back(YUG_KEY_DOWN);
		moveList.push_back(YUG_KEY_X);

	}
	void AIUnit::playCard(std::vector<int>cardPosis){
		if(cardPosis[0] < 0){
			playBoardCard(cardPosis);
			return;
		}
		int sz = cardPosis.size();
		for(unsigned int i = 0; i < cardPosis.size(); i++){
			cursorTo(cardPosis[i]);//picking cards
			if(sz > 1)
				moveList.push_back(YUG_KEY_UP);
		}
		moveList.push_back(YUG_KEY_X);//commit cards
		if(sz == 1){//notFusing
			if(playFaceUp)
				moveList.push_back(YUG_KEY_LEFT);
			moveList.push_back(YUG_KEY_X);
		}
		//!!! CHOSE BEST SPOT PREVIOUSLY
		//moveList.push_back(YUG_KEY_X);//play on chosen space
		pressPlayPosition(theBoard.enemy.hand[cardPosis[0]]);

		if(playedCon == YUG_AI_BOT_CON)
			moveList.push_back(YUG_KEY_DOWN);
		moveList.push_back(YUG_KEY_X);
	}
	void AIUnit::playBoardCard(std::vector<int> cardPosis){
		//std::cout<<"\tPlayBoardCard start\n";
		int sz = cardPosis.size();
		for(unsigned int i = 1; i < cardPosis.size(); i++){
			cursorTo(cardPosis[i]);//picking cards
			moveList.push_back(YUG_KEY_UP);
		}
		moveList.push_back(YUG_KEY_X);//commit cards

		if(sz==2)
			moveList.push_back(YUG_KEY_X);

		int col = ( -(cardPosis[0]+1) );
		//std::cout<<"\tBoard card col = "<<col<<" Record col "<<cardPosis[0]<<std::endl;
		cursorCol = choosePosCardPosition();
		cursorRow = YUG_BOARD_ENEMY_MON_ROW;
		//std::cout<<"\t start col = "<<cursorCol<<" Dest col = "<<col<<std::endl;
		boardCursorTo(col, YUG_BOARD_ENEMY_MON_ROW);

		moveList.push_back(YUG_KEY_X);

		if(playedCon == YUG_AI_BOT_CON)
			moveList.push_back(YUG_KEY_DOWN);
		moveList.push_back(YUG_KEY_X);
	}

	int AIUnit::choosePosCardPosition(){
		for(int i = 4; i >= 0; i-- ){
			if(theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard()){
				return i;
			}
		}
		return 4;
	}
	AICard AIUnit::tryToAddEquipCard(AICard& aiCard){
		//std::cout<<"Hand AI: trying to add equip\n";
		if(aiCard.card.attack < highStat && aiCard.card.defense < lowStat){
			return aiCard;
		}
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& eqCard = theBoard.enemy.hand[i];
			if(eqCard.monMagTrap == YUG_EQUIP_CARD){
				//std::cout<<"\tIs an equip: col= "<<i<<std::endl;
				if(fusionUnit.readingEquipList(eqCard, aiCard.card)){
					//std::cout<<"\tEquip can equip\n";
					aiCard.posis.push_back(i);
					return aiCard;
				}
			}//equip if
		}
		return aiCard;
	}

	void AIUnit::cursorTo(int dest){
		if(dest==handPositon) return;
		if(dest < handPositon){
			moveList.push_back(YUG_KEY_LEFT);
			handPositon--;
			cursorTo(dest);
		}else{
			moveList.push_back(YUG_KEY_RIGHT);
			handPositon++;
			cursorTo(dest);
		}
	}

	bool AIUnit::getStrongestEnemyDefenseCard(Card::CardData& c){
		//std::cout<<"\tstrongestEnemyDefendCard\n";
		int def = -5;
		int index = -1;
		for(unsigned int i = 0 ; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
				if(theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].faceUp){
					//std::cout<<"looking at card: "<<theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].name.data()<<std::endl;
					int d = theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].defense;
					if(d > def){
						def = d;
						index = i;
					}
				}
			}
		}
		//std::cout<<"Calced highest their def = "<<def<<" at "<<index<<std::endl;
		if(index != -1){
			c = theBoard.board[index][YUG_BOARD_PLAYER_MON_ROW];
			//std::cout<<"\nStrongest def is: "<<c.name.data()<<" def = "<<c.defense<<std::endl;
			return true;
		}
		return false;
	}
	int AIUnit::canBreakStrongestDefenseCardIndex(){
		Card::CardData defCard;
		if(getStrongestEnemyDefenseCard(defCard)){
			for(int i = strongestCards.size()-1; i >= 0; i--){
				if(0 < differenceFromDefFight(strongestCards[i].card, defCard)){
					return i;
				}
			}
			return YUG_AI_NO_MARK;
		}else{
			return strongestCards.size()-1;
		}
	}
	bool AIUnit::foundAndPlayedCardToBreakDefense(){
		int i = canBreakStrongestDefenseCardIndex();
		if( i != YUG_AI_NO_MARK){
			if(theirDefCard != YUG_AI_NO_MARK)
				setCorrectConstellation(i,theirDefCard);
			tryToAddEquipCard( strongestCards[i] );
			playCard(strongestCards[i].posis);
			return true;
		}
		return false;
	}

	bool AIUnit::foundAndPlayedAttackMagic(){
		for(unsigned int i = 0; i < 5; i++){
			if(isAttackMagic(theBoard.enemy.hand[i])){
				playMagicCard(i);
				return true;
			}
		}
		return false;
	}

	bool AIUnit::isAttackMagic(Card::CardData& c){
		int no = c.cardNumber;
		return (no == 779 || no == 782 || no == 802 ||
			no == 805 || no == 806 || no == 816 || no == 804);
	}

	bool AIUnit::imSworded(){
		return swordUnit.enemyWait != -1;
	}
	bool AIUnit::theirSworded(){
		return swordUnit.playerWait != -1;
	}

	int AIUnit::newStrongestCardIndex(){
		for(int i = strongestCards.size()-1; i >= 0; i--){
			if(0 < differenceIfCurrentFight(strongestCards[i].card,theirAtkCard)){
				return i;
			}
		}
		return YUG_AI_NO_MARK;
	}
	bool AIUnit::foundAndPlayedNewStrongestCard(){
		int index = newStrongestCardIndex();
		if(index == YUG_AI_NO_MARK){
			return false;
		}
		if(theirAtkCard != YUG_AI_NO_MARK)
			setCorrectConstellation(index, theirAtkCard);
		tryToAddEquipCard(strongestCards[index]);
		playCard(strongestCards[index].posis);
		return true;
	}
	void AIUnit::setCorrectConstellation(int stIndex, int theirCol){
		Card::CardData myC = strongestCards[stIndex].card;
		myC.currentConstellation = myC.constellations[0];
		int firstCon = differenceIfCurrentFight(myC, theirCol);
		myC.currentConstellation = myC.constellations[1];
		int secondCon = differenceIfCurrentFight(myC, theirCol);
		if(secondCon > firstCon){
			playedCon = YUG_AI_BOT_CON;
		}else{
			playedCon = YUG_AI_TOP_CON;
		}
	}

	int AIUnit::newStrongestDefenseIndex(){
		for(int i = strongestCards.size()-1; i >= 0; i--){
			if(!strongestCards[i].card.blankCard()){
				int ret = differenceIfImDefending(strongestCards[i].card,theirAtkCard); 
				if(0 <= ret){
					return i;
				}
			}
		}
		return YUG_AI_NO_MARK;
	}
	bool AIUnit::foundAndPlayedNewStrongestDefense(){
		int index = newStrongestDefenseIndex();
		if(index == YUG_AI_NO_MARK){
			return false;
		}
		if(theirAtkCard != YUG_AI_NO_MARK)
			setCorrectConstellation(index, theirAtkCard);
		tryToAddEquipCard( strongestCards[index] );
		playCard(strongestCards[index].posis);
		return true;
	}
	int AIUnit::differenceIfImDefending(Card::CardData& myC, int thCol){
		
		Card::CardData& thC = theBoard.board[thCol][YUG_BOARD_PLAYER_MON_ROW];
		if(!thC.faceUp)
			return YUG_AI_CARD_DOWN;
		int ret = 0;
		Card::CardData& theirC = thC;
		int myA = myC.defense;
		int theirA = theirC.attack;
		if(myC.currentConstellation == YUG_CARD_NO_CURRENT_CON){
			if(versusConstellation(myC.constellations[0],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else if(versusConstellation(myC.constellations[1],theirC.currentConstellation) == YUG_STRONG_AGAINST){
				return (myA+500) - theirA;
			}else{
				return myA - theirA;
			}
		}else{
			int con = myC.versusConstellation(theirC.currentConstellation);
			if(con == YUG_WEAK_AGAINST)
				return (myA-500) - theirA;
			if(con == YUG_STRONG_AGAINST)
				return (myA+500) - theirA;
			return myA - theirA;
		}
	}

	void AIUnit::playMiddlingCard(){
		strongestCards.clear();
		calcStrongest1Cards();
		sortStrongestCardsAttack();
		//printStrongestCards();
		if(strongestCards.size() != 0){
			playCard(strongestCards[0].posis);
		}else{
			playCard(0);//last attempt play
		}
	}

	int AIUnit::hasSwords(){
		for(unsigned int i = 0; i < 5; i++){
			if(theBoard.enemy.hand[i].cardNumber == 838)
				return i;
		}
		return YUG_AI_NO_MARK;
	}
	void AIUnit::discardCards(int noToDiscard){
		//std::cout<<"Hand AI : Discarding cards\n";
		if(strongestCards.size() > 1){
			if(strongestCards[strongestCards.size()-1].posis.size() > noToDiscard){
				playCard(strongestCards[strongestCards.size()-1].posis);
				return;
			}
		}
		std::vector<int>discards(noToDiscard);
		for(unsigned int i = 0; i < noToDiscard; i++){
			discards[i] = i;
		}
		playCard(discards);
	}

	int AIUnit::noOfCardsIn(int row){
		int count = 0;
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][row].blankCard())
				count++;
		}
		return count;
	}
	bool AIUnit::isFirstTurn(){
		return theBoard.enemy.deck.cardsLeft == 35;
	}

	void AIUnit::pressPlayPosition(Card::CardData& playedCard){
		if(noOfCardsIn(YUG_BOARD_ENEMY_MON_ROW)==5){
			int bp = bestPosition(playedCard);
			for(unsigned int i = 0; i < bp; i++){
				moveList.push_back(YUG_KEY_LEFT);
			}
		}
		moveList.push_back(YUG_KEY_X);
	}

	int AIUnit::bestPosition(Card::CardData& playedCard){
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& c = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			int no1 = fusionUnit.indiviualFusion(playedCard, c);
			int no2 = fusionUnit.generalFusion(playedCard, c);
			if(no1 == 0 && no2 == 0){//no fusion
				if(c.attack < highStat && c.defense < highStat)
					return 4-i;
			}
		}
		int stat = 10000; int pos = -1;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& c = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			int lowStat = c.attack>c.defense ? c.attack:c.defense ;
			if(lowStat < stat){
				pos = i;
				stat = lowStat;
			}
		}
		return 4-pos;
	}

	bool AIUnit::opponentHasStrongField(){
		if(noOfCardsIn(YUG_BOARD_PLAYER_MON_ROW)>=3)
			return true;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& c = theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW];
			if( c.attack>dangerStat || c.attack>highStat )
				return true;
		}
		return false;
	}

	bool AIUnit::isCardVisible(int theirCol){
		return theBoard.board[theirCol][YUG_BOARD_PLAYER_MON_ROW].faceUp;
	}

	int AIUnit::versusConstellation(int myCon,int enemyCon){
		switch(enemyCon){
		case YUG_MERCURY:
			if(myCon == YUG_VENUS)
				return YUG_STRONG_AGAINST;
			else{
				return constellationComparison(myCon, enemyCon);
			}
		case YUG_SUN:
			return constellationComparison(myCon, enemyCon);
		case YUG_MOON:
			return constellationComparison(myCon, enemyCon);
		case YUG_VENUS:
			if(myCon  == YUG_MERCURY)
				return YUG_WEAK_AGAINST;
			if(myCon == YUG_MARS)
				return YUG_EQUAL_AGAINST;
			else{
				return constellationComparison(myCon, enemyCon);
			}
		case YUG_MARS:
			if(myCon == YUG_NEPTUNE)
				return YUG_STRONG_AGAINST;
			if(myCon == YUG_VENUS)
				return YUG_EQUAL_AGAINST;
			else{
				return constellationComparison(myCon, enemyCon);
			}
		case YUG_JUPITER:
			return constellationComparison(myCon, enemyCon);
		case YUG_SATURN:
			return constellationComparison(myCon, enemyCon);
		case YUG_URANUS:
			return constellationComparison(myCon, enemyCon);
		case YUG_PLUTO:
			return constellationComparison(myCon, enemyCon);
		case YUG_NEPTUNE:
			if(myCon == YUG_MARS)
				return YUG_WEAK_AGAINST;
			else{
				return constellationComparison(myCon, enemyCon);
			}
		default:
			//std::cout<<"AIUnit::vesusConstellation: invalid enemy con given: "<<enemyCon<<std::endl;
			return YUG_EQUAL_AGAINST;
		}
	}

	int AIUnit::constellationComparison(int myCon, int enemyCon){
		if(myCon - enemyCon == 1)
			return YUG_WEAK_AGAINST;
		if(myCon - enemyCon == -1)
			return YUG_STRONG_AGAINST;
		return YUG_EQUAL_AGAINST;
	}

	void AIUnit::setupHandState(){}
	void AIUnit::decideHandPath(){}
	void AIUnit::breakThroughHandPath(){}
	void AIUnit::bestAttackHandPath(){}
	void AIUnit::emptyBoardHandPath(){}
	void AIUnit::canHoldHandPath(){}
	void AIUnit::badPositonHandPath(){}
	void AIUnit::iHaveNoCardsHandPath(){}
	void AIUnit::firstTurnHandPath(){}
	void AIUnit::setUpHighLowStats(){}
}










