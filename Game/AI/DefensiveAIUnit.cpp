#include <Game\AI\DefensiveAIUnit.h>
#include <Utility\InputUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Duel\FusionUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\Clock.h>
#include <iostream>

namespace AI{



	DefensiveAIUnit::DefensiveAIUnit(int ab, int low, int high, int danger, int disNum)
	{
		ability = ab; lowStat = low; highStat = high;
		dangerStat = danger; discardNumber = disNum;
	}

	void DefensiveAIUnit::calcHandMove(){
		newTurnSetup();
		setupHandState();
		decideHandPath();
		prepareBattleAI();
	}

	void DefensiveAIUnit::setupHandState(){
		setHighestCards();

		calcStrongest1Cards();

		if(ability >= YUG_AI_LOW_AB)
			calcStrongest2Cards();
		if(ability >= YUG_AI_MID_AB)
			calcStrongest3Cards();

		if(ability >= YUG_AI_HIGHEST_AB)
			calcStrongest4Cards();

		removeDuplicateStrongestCards();
		sortStrongestCardsDefense();
		gameClock.newLoop();
		moveList.clear();
		moveIndex = 0;

		doIHaveBestAttack();
		canIBreakThrough();
		canIHoldHim();
		doIHaveAttackMagic();
		printStrongestCards();
	}

	void DefensiveAIUnit::decideHandPath(){
		/*std::cout<<"breakThrough: "<<canBreak<<
			"\nBest Attack: "<<bestAttack<<
			"\nHold Him: "<<canHold<<std::endl;
		std::cout<<"Their highest atk at "<<theirAtkCard<<
			"\nTheir highest def at "<<theirDefCard<<std::endl;
		*/
		if(isFirstTurn()){
			firstTurnHandPath();
			return;
		}
		if(isEnemyBoardMonsterless()){
			emptyBoardHandPath();
			return;
		}
		if(noOfCardsIn(YUG_BOARD_ENEMY_MON_ROW)==0){
			iHaveNoCardsHandPath();
			return;
		}

		if(canBreak){
			breakThroughHandPath();
			return;
		}
		if(bestAttack){
			bestAttackHandPath();
			return;
		}
		if(canHold){
			canHoldHandPath();
			return;
		}
		badPositonHandPath();
	}
	
	void DefensiveAIUnit::firstTurnHandPath(){
		//std::cout<<"Debug AI: first turn hand path\n";
		for(unsigned int i = 0; i <  5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MAG_ROW].blankCard()){
				boardMarks[i][YUG_BOARD_PLAYER_MAG_ROW] = YUG_AI_IS_TRAP;
				break;
			}
		}
		if(noOfCardsIn(YUG_BOARD_PLAYER_MON_ROW)==0){
			int i = hasField();
				if(i != YUG_AI_NO_MARK && overlappingFields(i)){
					playMagicCard(i);
					return;
			}
		}//no enemy monsters
		playStrongestCard();
	}

	void DefensiveAIUnit::iHaveNoCardsHandPath(){
		//std::cout<<"Debug AI: I have no cards hand path\n";
		if(foundAndPlayedNewStrongestDefense()){
			return;
		}
		if(foundAndPlayedNewStrongestCard()){
			//std::cout<<"Playing new strongest Card\n";
			return;
		}
		if(opponentHasStrongField()){
			if(foundAndPlayedAttackMagic()){
				return;
			}
			discardCards(discardNumber);
		}
		playStrongestCard();
	}

	void DefensiveAIUnit::emptyBoardHandPath(){
		//std::cout<<"DebugAI: empty board path\n";
		if(canWin())
			playStrongestCard();
		//std::cout<<"debug AI 2b\n";
		if(myNoOfMonsterCards() > 0){
			if( enemyHasTraps() || noOfEnemyMagics() <= 2){
				//std::cout<<"Trying To play harpies\n";
				int posi = canPlayHarpies();
				if(posi != YUG_AI_NO_MARK){
					//std::cout<<"Playing harpies\n";
					playMagicCard(posi);
					return;
				}
			}//if has traps
		}//if monsters
		playStrongestCard();
	}

	void DefensiveAIUnit::breakThroughHandPath(){
		//std::cout<<"DebugAI: breakthrough path\n";
		bool cardFaceup = false;
		int posi = hasSupportMagics(cardFaceup);
		if(posi != YUG_AI_NO_MARK){
			if(cardFaceup){
				playMagicCard(posi);
			}else{
				playCard(posi);
			}
			return;
		}

		if(enemyHasTraps() || (noOfEnemyMagics() >= 2) ){
			//std::cout<<"Trying To play harpies\n";
			int posi = canPlayHarpies();
			if(posi != YUG_AI_NO_MARK){
				//std::cout<<"Playing harpies\n";
				playMagicCard(posi);
				return;
			}
		}

		if( canWin()){
			playStrongestCard();
			return;
		}
		int dt = hasDefensiveTrap();
		if(dt != YUG_AI_NO_MARK){
			playCard(dt);
			return;
		}
		playStrongestCard();

	}

	void DefensiveAIUnit::bestAttackHandPath(){
		//std::cout<<"Best attack hand path\n";
		bool cardFaceup = false;
		int posi = hasSupportMagics(cardFaceup);
		if(posi != YUG_AI_NO_MARK){
			if(cardFaceup){
				playMagicCard(posi);
			}else{
				playCard(posi);
			}
			return;
		}

		if(enemyHasTraps() || (noOfEnemyMagics() >= 2) ){
			//std::cout<<"Trying To play harpies\n";
			int posi = canPlayHarpies();
			if(posi != YUG_AI_NO_MARK){
				//std::cout<<"Playing harpies\n";
				playMagicCard(posi);
				return;
			}
		}
		//std::cout<<"\ttry break defense\n";
		if(foundAndPlayedCardToBreakDefense()){
			//std::cout<<"Best Attack: break defense\n";
			return;
		}
		if(foundAndPlayedAttackMagic()){
			return;
		}

		int dt = hasDefensiveTrap();
		if(dt != YUG_AI_NO_MARK){
			playCard(dt);
			return;
		}
		//std::cout<<"Best attack: default: strongest card\n";
		playStrongestCard();
	}

	void DefensiveAIUnit::canHoldHandPath(){
		//std::cout<<"Hold Him Hand Path\n";
		bool cardFaceup = false;
		int posi = hasSupportMagics(cardFaceup);
		if(posi != YUG_AI_NO_MARK){
			if(cardFaceup){
				playMagicCard(posi);
			}else{
				playCard(posi);
			}
			return;
		}

		if(foundAndPlayedAttackMagic()){
			//std::cout<<"Playing attack magic\n";
			return;
		}
		int dt = hasDefensiveTrap();
		if(dt != YUG_AI_NO_MARK){
			playCard(dt);
			return;
		}
		if(foundAndPlayedNewStrongestCard()){
			//std::cout<<"Playing new strongest Card\n";
			return;
		}
		playStrongestCard();
	}

	void DefensiveAIUnit::badPositonHandPath(){
		//std::cout<<"Bad position hand path\n";
		if(foundAndPlayedNewStrongestCard()){
			return;
		}
		//std::cout<<"\tCouldnt find new attacker\n";
		if(foundAndPlayedNewStrongestDefense()){
			return;
		}
		//std::cout<<"\tCouldnt find new defender\n";
		if(!theirSworded()){
			int s = hasSwords();
			if(s != YUG_AI_NO_MARK){
				playMagicCard(s);
				return;
			}
		}
		if(foundAndPlayedAttackMagic()){
			return;
		}
		discardCards(discardNumber);
	}

	void DefensiveAIUnit::fieldRecursion(){
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

		bool DefensiveAIUnit::defenseOnlyMon(int myCol){
		//std::cout<<"\t\t\tDefenseOnly Monster\n";
		Card::CardData& c = theBoard.board[myCol][YUG_BOARD_ENEMY_MON_ROW];
		if (c.attack < (c.defense-500))
			return true;
		if(thisDefenseIsNeeded(myCol)){
				return true;
		}
		return false;
	}

}


