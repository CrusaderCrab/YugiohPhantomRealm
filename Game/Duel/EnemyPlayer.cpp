#include <Game\Duel\EnemyPlayer.h>
#include <Game\EnemyData.h>
#include <Utility\InputUnit.h>
#include <iostream>

namespace Duel{


	void EnemyPlayer::newTurn(){
		ai->moveList.clear();
		ai->moveIndex = 0;
		GamePlayer::newTurn();
	}

	bool EnemyPlayer::startup(int info){
		ai = enemyData.ai;
		return GamePlayer::startup(info);
	}

	void EnemyPlayer::input(){
		if(!isWaiting && currentChainPosition == YUG_GAME_PLAYER_CHAIN_TAKING_INPUTS){
			inputUnit.releaseAllComputerKeys();
			if(ai->hasMovesLeft()){
				ai->pressNextKey();
				//wait(0.2f);
			}else{
				ai->calcHandMove();
			}
		}
		GamePlayer::input();
	}
}