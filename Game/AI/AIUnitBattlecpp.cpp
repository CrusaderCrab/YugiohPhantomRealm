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

	void AIUnit::prepareBattleAI(){
		//std::cout<<"$$$prepared battle ai$$$\n";
		clearCardsDecided();
	}

	void AIUnit::clearCardsDecided(){
		for(unsigned int i = 0; i < 5; i++){
			cardsDecided[i] = false;
			//std::cout<<"Clear Card decide "<<i<<" "<<cardsDecided[i]<<std::endl;
		}
	}

	void AIUnit::clearLastMonsterMarks(){
		for(unsigned int i = 0; i <  5; i++){
			boardMarks[i][YUG_BOARD_PLAYER_MON_ROW] = YUG_AI_NO_MARK;
			boardMarks[i][YUG_BOARD_ENEMY_MON_ROW] = YUG_AI_NO_MARK;
		}
	}

	void AIUnit::removeAllTrapMarks(){
		for(unsigned int i = 0; i < 5; i++){
			boardMarks[i][YUG_BOARD_PLAYER_MAG_ROW] = YUG_AI_NO_MARK;
		}
	}

	void AIUnit::recursionBattleSetup(){
		moveList.clear(); 
		
		moveIndex = 0;
		canBreak = false; canHold = false; bestAttack = false;
		setCursorPos();
		clearLastMonsterMarks();
		//std::cout<<"\tbreak\n";
		canIBreakThrough();
		doIHaveBestAttack();
		///std::cout<<"\thold\n";
		canIHoldHim();
		markMyHighestAttack();
		markMyHighestDefense();
		//std::cout<<"\tneeded\n";
		markMyNeededCards();
		if(noOfCardsIn(YUG_BOARD_PLAYER_MAG_ROW)==0){
			removeAllTrapMarks();
		}
		//markHighestAttack();
		//markHighestDefense();
	}
	void AIUnit::markMyNeededCards(){
		if(!canBreak && canHold){
			for(unsigned int i = 0; i < 5; i++){
				if( !canBreakMyCard(i) ){
					if(boardMarks[i][YUG_BOARD_ENEMY_MON_ROW] == YUG_AI_NO_MARK){
						boardMarks[i][YUG_BOARD_ENEMY_MON_ROW] = YUG_AI_NEEDED;
					}
				}
			}//end for
		}
	}

	void AIUnit::markMyHighestAttack(){
		int pos = -1; int atk = -1;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& c = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			if(!c.blankCard()){
				if(c.attack > atk){
					atk = c.attack; pos = i;
				}
			}
		}
		if(pos!=-1)
			boardMarks[pos][YUG_BOARD_ENEMY_MON_ROW] = YUG_AI_HIGH_ATTACK;
	}
	void AIUnit::markMyHighestDefense(){
		int pos = -1; int def = -1;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& c = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			if(!c.blankCard()){
				if(c.defense > def){
					def = c.defense; pos = i;
				}
			}
		}
		if(pos != -1)
			if(boardMarks[pos][YUG_BOARD_ENEMY_MON_ROW] == YUG_AI_NO_MARK)
				boardMarks[pos][YUG_BOARD_ENEMY_MON_ROW] = YUG_AI_HIGH_DEFENSE;
	}
	void AIUnit::markHighestAttack(){
		int myAtk = -1; int myPos = -1;
		int thAtk = -1; int thPos = -1;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& myC = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			Card::CardData& thC = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			if(!myC.blankCard() && (myC.attack > myAtk)){
				myAtk = myC.attack; myPos = i;
			}
			if(!thC.blankCard() && (thC.attack > thAtk)){
				thAtk = thC.attack; thPos = i;
			}
		}
	}
	void AIUnit::markHighestDefense(){

	}
	bool AIUnit::allCardsDecided(){
		for(unsigned int i = 0; i < 5 ;i++){
			//std::cout<<"\tCard decide "<<i<<" "<<cardsDecided[i]<<std::endl;
			if((!cardsDecided[i]) && (!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard()))
				return false;
		}
		return true;
	}

	void AIUnit::fieldRecursion(){
		//std::cout<<"battle AI: start field recursion\n";
		recursionBattleSetup();
		//std::cout<<"Can break: "<<canBreak<<"\nBest Attack: "<<bestAttack
		//	<<"\nCan Hold: "<<canHold<<std::endl;
		if(allCardsDecided()){//end turn
			//std::cout<<"\tAll cards decided\n";
			moveList.push_back(YUG_KEY_START);
			return;
		}
		int sm = boardSupportMagic();
		if(sm != YUG_AI_NO_MARK){
			activateSupportMagic(sm);
			return;
		}
		int mm = boardMiscMagic();
		if(mm != YUG_AI_NO_MARK){
			activateSupportMagic(mm);
			return;
		}
		int emon = YUG_AI_NO_MARK; int emag = YUG_AI_NO_MARK;
		boardEquipCards(emag,emon);//int& pass -> alters thems
		if(emag != YUG_AI_NO_MARK){
			activateEquipCard(emag, emon);
			return;
		}
		if(!canBreak){//attack magics
			int am = boardAttackMagic();
			if(am != YUG_AI_NO_MARK){
				activateSupportMagic(am);
				return;
			}
		}
		std::vector<int>myCards = myCurrentMonstersList();
		std::vector<int>theirCards = theirCurrentMonsterList();
		sortRowList(myCards, YUG_BOARD_ENEMY_MON_ROW);
		sortRowList(theirCards, YUG_BOARD_PLAYER_MON_ROW);
		
		if(noOfCardsIn(YUG_BOARD_PLAYER_MON_ROW)==0){//empty board
			for(unsigned int j = 0; j < myCards.size(); j++){
				//std::cout<<"\tDirect attack : "<<j<<std::endl;
				if(thinkAboutAttack(myCards[j],YUG_AI_NO_MARK)){
					return;
				}
			}
		}
		for(int i = theirCards.size()-1; i >= 0; i--){
			//std::cout<<"-"<<theBoard.board[theirCards[i]][YUG_BOARD_PLAYER_MON_ROW].name.data()<<std::endl;
			for(unsigned int j = 0; j < myCards.size(); j++){
				//std::cout<<"--"<<theBoard.board[myCards[j]][YUG_BOARD_ENEMY_MON_ROW].name.data()<<std::endl;
				//std::cout<<"\tMonster attack - their: "<<theirCards[i]<<" my: "<<myCards[j]<<std::endl;
				if(thinkAboutAttack(myCards[j],theirCards[i])){
					return;
				}
			}
		}
		//no more attacks
		for(unsigned int j = 0; j < myCards.size(); j++){
			//std::cout<<"\tBest Position "<<myCards[j]<<std::endl;
			if(putInBestPosition( myCards[j] )){
				return;
			}
		}
		//std::cout<<"\t Field recursion drop through\n";
		moveList.push_back(YUG_KEY_START);
	}

	int AIUnit::boardSupportMagic(){
		for(unsigned int i = 0; i < 5; i++){
			if(isBoardSupportMagic(i))
				return i;
		}
		return YUG_AI_NO_MARK;
	}
	bool AIUnit::isBoardSupportMagic(int col){
		int no = theBoard.board[col][YUG_BOARD_ENEMY_MAG_ROW].cardNumber;
		return ( (817 <= no && no <= 826) );
	}
	void AIUnit::activateSupportMagic(int posi){
		boardCursorTo( posi , YUG_BOARD_ENEMY_MAG_ROW );
		moveList.push_back(YUG_KEY_X);//choose card
		moveList.push_back(YUG_KEY_X);//activate card
	}

	int AIUnit::boardAttackMagic(){
		for(unsigned int i = 0; i < 5; i++){
			if(isBoardAttackMagic(i))
				return i;
		}
		return YUG_AI_NO_MARK;
	}
	bool AIUnit::isBoardAttackMagic(int col){
		int no = theBoard.board[col][YUG_BOARD_ENEMY_MAG_ROW].cardNumber;
		return ((no==779 && noOfCardsIn(YUG_BOARD_ENEMY_MON_ROW)!=5) 
			|| no==781 || no==782 || no==802 || no==805 || no==806 || no==838);
	}

	int AIUnit::boardMiscMagic(){
		for(unsigned int i = 0; i < 5; i++){
			if(isBoardMiscMagic(i))
				return i;
		}
		return YUG_AI_NO_MARK;
	}
	bool AIUnit::isBoardMiscMagic(int col){
		int no = theBoard.board[col][YUG_BOARD_ENEMY_MAG_ROW].cardNumber;
		return ((no==801 && worthStopDef()) || (no==803 && worthDarkLight())
			|| (no==808 && worthCurse()) );
	}
	bool AIUnit::worthStopDef(){
		if(canBreak)
			return false;
		for(unsigned int i = 0; i < 5; i++){
			if( !boardCanBreakTheirCard(i) ){
				if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].attackMode){
					return true;
				}
			}
		}
		return false;
	}
	bool AIUnit::worthDarkLight(){//player has 2+ more monsters face down
		int w = 0;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& myC = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			Card::CardData& thC = theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW];
			if(!myC.blankCard() && !myC.faceUp)w--;
			if(!thC.blankCard() && !thC.faceUp)w++;
		}
		return w >= 2;
	}
	bool AIUnit::worthCurse(){
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& myC = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			if(!myC.blankCard() && (myC.defStatDrop>0 || myC.atkStatDrop>0) )
				return true;
		}
		return false;
	}

	void AIUnit::boardEquipCards(int& magPos, int& monPos){
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& eqCard = theBoard.board[i][YUG_BOARD_ENEMY_MAG_ROW];
			if(eqCard.monMagTrap == YUG_EQUIP_CARD){
				for(unsigned int j = 0; j < 5; j++){
					Card::CardData& eqMon = theBoard.board[j][YUG_BOARD_ENEMY_MON_ROW];
					if(fusionUnit.readingEquipList(eqCard, eqMon)){
						magPos = i;
						monPos = j;
						return;
					}
				}//inner for
			}
		}//outer for
		magPos = YUG_AI_NO_MARK;
		monPos = YUG_AI_NO_MARK;
	}
	void AIUnit::activateEquipCard(int magPos, int monPos){
		boardCursorTo(magPos, YUG_BOARD_ENEMY_MAG_ROW);
		moveList.push_back(YUG_KEY_X);
		boardCursorTo(monPos, YUG_BOARD_ENEMY_MON_ROW);
		moveList.push_back(YUG_KEY_X);
	}

	std::vector<int> AIUnit::myCurrentMonstersList(){
		std::vector<int> v;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& c = theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW];
			if(!c.blankCard())
				v.push_back(i);
		}
		return v;
	}
	std::vector<int> AIUnit::theirCurrentMonsterList(){
		std::vector<int> v;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& c = theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW];
			if(!c.blankCard())
				v.push_back(i);
		}
		return v;
	}
	//weakest to strongest atk
	void AIUnit::sortRowList(std::vector<int>& cardPos, int row){
		for(unsigned int i = 0; i<cardPos.size() ; i++){
			for(unsigned int j = i+1; j<cardPos.size(); j++){
				//std::cout<<"-Sort Row i = "<<i<<" j = "<<j<<std::endl;
				if(theBoard.board[cardPos[i]][row].attack > theBoard.board[cardPos[j]][row].attack){
					//std::cout<<"--do swap\n";
					int temp = cardPos[ j ];
					cardPos[ j ] = cardPos[ i ];
					cardPos[ i ] = temp;
				}
			}
		}
	}

	bool AIUnit::thinkAboutAttack(int myCol, int theirCol){
		//std::cout<<"\t\tThink about attack: myCol = "<<myCol<<" TheirCol = "<<theirCol<<std::endl;
		if(cardsDecided[myCol]){
			//std::cout<<"\t\tAlreadyDecided\n";
			return false;
		}
		if(theirCol==YUG_AI_NO_MARK){
			return thinkAboutDirectAttack(myCol);
		}
		Card::CardData& c = theBoard.board[myCol][YUG_BOARD_ENEMY_MON_ROW];
		int res = differenceIfCurrentFight(c, theirCol);
		if( res==YUG_AI_CARD_DOWN && c.attack<lowStat ){
			//std::cout<<"\t\tTAA: too low attack for face down atk\n";
			return false;
		}
		if((!thinkAboutDamage(res, myCol, theirCol)) && res != YUG_AI_CARD_DOWN){
			//std::cout<<"\t\tTAA: would lose fight\n";
			return false;
		}
		if(enemyHasTraps() && boardMarks[myCol][YUG_BOARD_ENEMY_MON_ROW]==YUG_AI_HIGH_ATTACK)
		{
			//std::cout<<"\t\tWont' attack with highest + traps\n";
			return false;//secondStrongestAttack(theirCol);
		}
		if(!defenseOnlyMon(myCol)){
			//std::cout<<"\t\tTAA: will attack\n";
			boardCursorTo(myCol, YUG_BOARD_ENEMY_MON_ROW);
			if(!theBoard.board[myCol][YUG_BOARD_ENEMY_MON_ROW].attackMode)
				moveList.push_back(YUG_KEY_L1);
			moveList.push_back(YUG_KEY_X);
			cursorRow = YUG_BOARD_PLAYER_MON_ROW;
			cursorCol = 0;
			boardCursorTo(theirCol, YUG_BOARD_PLAYER_MON_ROW);
			moveList.push_back(YUG_KEY_X);
			cursorRow = YUG_BOARD_ENEMY_MON_ROW;
			cardsDecided[myCol] = true;
			return true;
		}
		//std::cout<<"\t\tTAA: wont attack fall through\n";
		return false;
	}
	bool AIUnit::thinkAboutDamage(int damage, int myCol, int thCol){
		if(damage > 0)
			return true;
		if(damage < 0)
			return false;
		if(boardMarks[myCol][YUG_BOARD_ENEMY_MON_ROW] != YUG_AI_NO_MARK)
			return false;
		if(theBoard.board[thCol][YUG_BOARD_PLAYER_MON_ROW].attackMode)
			return true;
		return false;
	}
	bool AIUnit::thinkAboutDirectAttack(int myCol){
		//std::cout<<"\t\t\tTAA direct attack\n";
		if(enemyHasTraps() && boardMarks[myCol][YUG_BOARD_ENEMY_MON_ROW]==YUG_AI_HIGH_ATTACK)
		{
			return false;//secondStrongestAttack(theirCol);
		}
		if(!defenseOnlyMon(myCol)){
			boardCursorTo(myCol, YUG_BOARD_ENEMY_MON_ROW);
			if(!theBoard.board[myCol][YUG_BOARD_ENEMY_MON_ROW].attackMode)
				moveList.push_back(YUG_KEY_L1);
			moveList.push_back(YUG_KEY_X);
			moveList.push_back(YUG_KEY_X);
			cardsDecided[myCol] = true;
			return true;
		}
		return false;
	}
	bool AIUnit::defenseOnlyMon(int myCol){
		//std::cout<<"\t\t\tDefenseOnly Monster\n";
		Card::CardData& c = theBoard.board[myCol][YUG_BOARD_ENEMY_MON_ROW];
		if (c.attack < (c.defense-1000))
			return true;
		if(thisDefenseIsNeeded(myCol)){
				return true;
		}
		return false;
	}
	bool AIUnit::thisDefenseIsNeeded(int myCol){
		//std::cout<<"\t\t\tThis card is needed\n";
		if(!bestAttack && !canBreak){
			//std::cout<<"\t\t\tDefensive position\n";

			bool canBeBroken = false;
			for(unsigned int i = 0; i < 5; i++){
				if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
					int res = differenceIfBestFight(theBoard.board[myCol][YUG_BOARD_ENEMY_MON_ROW],i);
					if(res < 0 && res != YUG_AI_CARD_DOWN ){
						canBeBroken = true;
					}
				}
			}

			if(!canBeBroken){
				//std::cout<<"\t\t\tIs a Great wall of defense\n";
				return true;
			}
		}
		return false;
	}

	bool AIUnit::putInBestPosition(int col){
		//std::cout<<"\tBest position col = "<<col<<std::endl;
		Card::CardData& c = theBoard.board[col][YUG_BOARD_ENEMY_MON_ROW];
		if(!c.blankCard()&&!cardsDecided[col]){
			if(higherAttackThenThis(c) || c.attack<=c.defense){
				if(c.attackMode){
					boardCursorTo(col, YUG_BOARD_ENEMY_MON_ROW);
					moveList.push_back(YUG_KEY_R1);
					cardsDecided[col] = true;
					return true;
				}
			}else{
				if(!c.attackMode){
					boardCursorTo(col, YUG_BOARD_ENEMY_MON_ROW);
					moveList.push_back(YUG_KEY_R1);
					cardsDecided[col] = true;
					return true;
				}
			}
		}
		return false;
	}
	bool AIUnit::higherAttackThenThis(Card::CardData& c){
		//std::cout<<"\t\tHigher attack: "<<c.name.data()<<std::endl;
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
				if(0 > differenceIfAtkFight(c, i)){
					return true;
				}
			}
		}
		return false;
	}


	void AIUnit::setCursorPos(){
		cursorCol = theBoard.cP[0];
		cursorRow = theBoard.cP[1];
	}
	void AIUnit::boardCursorTo(int col, int row){
		//std::cout<<"Board Cursor wanted col ="<<col<<" row = "<<row<<std::endl;
		if(row > cursorRow){//up towards enemy
			moveList.push_back(YUG_KEY_UP);
			cursorRow++;
			boardCursorTo(col, row);
		}else if(row < cursorRow){//down towards player
			moveList.push_back(YUG_KEY_DOWN);
			cursorRow--;
			boardCursorTo(col, row);
		}else{
			if(col > cursorCol){
				moveList.push_back(YUG_KEY_RIGHT);
				cursorCol++;
				boardCursorTo(col, row);
			}else if(col < cursorCol){
				moveList.push_back(YUG_KEY_LEFT);
				cursorCol--;
				boardCursorTo(col, row);
			}
		}
		//on correct spot
	}

	bool AIUnit::boardCanBreakTheirCard(int enemyPos){
		if(!isCardVisible(enemyPos))
			return true;
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard() && !cardsDecided[i]){
				if(0 <= differenceIfCurrentFight(theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW],enemyPos)){
					return true;
				}
			}
		}
		return false;
	}
}