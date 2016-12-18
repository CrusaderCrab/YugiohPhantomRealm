#include <Game\AI\RegularAIUnit.h>
#include <Utility\InputUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Duel\FusionUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\Clock.h>
#include <iostream>

namespace AI{



	RegularAIUnit::RegularAIUnit(int ab, int low, int high, int danger, int disNum)
	{
		ability = ab; lowStat = low; highStat = high;
		dangerStat = danger; discardNumber = disNum;
	}

	void RegularAIUnit::calcHandMove(){
		newTurnSetup();
		setupHandState();
		decideHandPath();
		prepareBattleAI();
	}

	void RegularAIUnit::setupHandState(){
		setHighestCards();

		calcStrongest1Cards();
		if(ability >= YUG_AI_LOWEST_AB)
			calcStrongest2BoardCards();
		if(ability >= YUG_AI_LOW_AB)
			calcStrongest2Cards();
		if(ability >= YUG_AI_MID_AB)
			calcStrongest3Cards();
		if(ability >= YUG_AI_HIGH_AB)
			calcStrongest3BoardCards();
		if(ability >= YUG_AI_HIGHEST_AB)
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

	void RegularAIUnit::decideHandPath(){
		/*std::cout<<"RegAI::AI hand info\n";
		std::cout<<"breakThrough: "<<canBreak<<
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
			//std::cout<<"RegAI::breakthroughHandPath()\n";
			breakThroughHandPath();
			return;
		}
		if(bestAttack){
			//std::cout<<"RegAI::bestAttackHandPath()\n";
			bestAttackHandPath();
			return;
		}
		if(canHold){
			//std::cout<<"RegAI::canHoldHandPath()\n";
			canHoldHandPath();
			return;
		}
		badPositonHandPath();
	}
	
	void RegularAIUnit::firstTurnHandPath(){
		//std::cout<<"Debug AI: first turn hand path\n";
		for(unsigned int i = 0; i <  5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MAG_ROW].blankCard()){
				boardMarks[i][YUG_BOARD_PLAYER_MAG_ROW] = YUG_AI_IS_TRAP;
				break;
			}
		}
		if(noOfCardsIn(YUG_BOARD_PLAYER_MON_ROW)==0){
			int i = hasField();
				if(i != YUG_AI_NO_MARK && !overlappingFields(i)){
					playMagicCard(i);
					return;
			}
		}//no enemy monsters
		if(foundAndPlayedNewStrongestCard()){
			//std::cout<<"Playing new strongest Card\n";
			return;
		}
		if(foundAndPlayedNewStrongestDefense()){
			return;
		}
		playStrongestCard();
	}

	void RegularAIUnit::iHaveNoCardsHandPath(){
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
			discardCards(discardNumber);
		}
		playStrongestCard();
	}

	void RegularAIUnit::emptyBoardHandPath(){
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

	void RegularAIUnit::breakThroughHandPath(){
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

	void RegularAIUnit::bestAttackHandPath(){
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

	void RegularAIUnit::canHoldHandPath(){
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
		dt = hasField();
		if(dt != YUG_AI_NO_MARK && !overlappingFields(dt)){
			playMagicCard(dt);
			return;
		}
		playMiddlingCard();
	}

	void RegularAIUnit::badPositonHandPath(){
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
}

