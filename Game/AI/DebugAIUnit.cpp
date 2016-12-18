#include <Game\AI\DebugAIUnit.h>
#include <Utility\InputUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Duel\FusionUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\Clock.h>
#include <iostream>

namespace AI{

	void DebugAIUnit::setUpHighLowStats(){
		lowStat = 1000; highStat = 2000; dangerStat = 2500;
	}

	void DebugAIUnit::calcHandMove(){
		//std::cout<<"--------DEBUG AI HAND START---------\n";
		newTurnSetup();
		setupHandState();
		decideHandPath();
		prepareBattleAI();
	}

	void DebugAIUnit::setupHandState(){
		setHighestCards();
		calcStrongest1Cards();
		calcStrongest2Cards();
		calcStrongest2BoardCards();
		calcStrongest3BoardCards();
		calcStrongest3Cards();
		calcStrongest4Cards();
		removeDuplicateStrongestCards();
		sortStrongestCardsAttack();
		gameClock.newLoop();
		moveList.clear();
		moveIndex = 0;

		doIHaveBestAttack();
		canIBreakThrough();
		canIHoldHim();
		doIHaveAttackMagic();
	}

	void DebugAIUnit::decideHandPath(){
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
	
	void DebugAIUnit::firstTurnHandPath(){
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

	void DebugAIUnit::iHaveNoCardsHandPath(){
		//std::cout<<"Debug AI: I have no cards hand path\n";
		if(foundAndPlayedNewStrongestCard()){
			//std::cout<<"Playing new strongest Card\n";
			return;
		}
		if(foundAndPlayedNewStrongestDefense()){
			return;
		}
		if(opponentHasStrongField()){
			if(foundAndPlayedAttackMagic()){
				return;
			}
			discardCards(3);
		}
		playStrongestCard();
	}

	void DebugAIUnit::emptyBoardHandPath(){
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

	void DebugAIUnit::breakThroughHandPath(){
		//std::cout<<"DebugAI: breakthrough path\n";
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

		bool cardFaceup = false;
		int posi = hasSupportMagics(cardFaceup);
		if(posi != YUG_AI_NO_MARK){
			if(cardFaceup){
				playMagicCard(posi);
			}else{
				playCard(posi);
			}
		}
		playStrongestCard();

	}

	void DebugAIUnit::bestAttackHandPath(){
		//std::cout<<"Best attack hand path\n";
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
		//std::cout<<"Best attack: default: strongest card\n";
		playStrongestCard();
	}

	void DebugAIUnit::canHoldHandPath(){
		//std::cout<<"Hold Him Hand Path\n";
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
		playMiddlingCard();
	}

	void DebugAIUnit::badPositonHandPath(){
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
		discardCards(3);
	}
}
