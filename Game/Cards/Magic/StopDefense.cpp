#include <Game\Cards\Magic\StopDefense.h>
#include <Game\Duel\Board.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\VectorUnit.h>
#include <Utility\SoundUnit.h>

#define ZYUG_GO 0
#define ZYUG_WAIT 1
#define ZYUG_FIN 2 

namespace Card{


	void StopDefense::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
	}

	void StopDefense::cleanup(){

	}
	void StopDefense::render(){

	}
	void StopDefense::update(){
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_WAIT:
				waitUpdate();
				break;
			case ZYUG_FIN:
				finishUpdate();
				break;
			default:
				break;
			}
		}else{
			continueWaiting();
		}
	}

	void StopDefense::startUpdate(){
		wait(0.3f);
		chain = ZYUG_WAIT;
	}
	void StopDefense::waitUpdate(){
		flipCards();
		wait(0.5f);
		chain = ZYUG_FIN;
	}
	void StopDefense::finishUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}

	void StopDefense::flipCards(){
		int row = (theBoard.playerControlling()
			?YUG_BOARD_ENEMY_MON_ROW:YUG_BOARD_PLAYER_MON_ROW);
		if(!theBoard.playerControlling()){
			for(int col = 0; col<5; col++){
				if(!theBoard.board[col][row].attackMode){
					soundUnit.cardSwivel();
					theBoard.board[col][row].attackMode = true;
					if(theBoard.board[col][row].faceUp){
						theBoard.board[col][row].smallRender.rotate(pos.eAtkFaceupFlat,0.2f);
					}else{
						theBoard.board[col][row].smallRender.rotate(pos.eAtkFacedownFlat,0.2f);
					}
				}
			}
		}else{
			for(int col = 0; col<5; col++){
				if(!theBoard.board[col][row].attackMode){
					soundUnit.cardSwivel();
					theBoard.board[col][row].attackMode = true;
					if(theBoard.board[col][row].faceUp){
						theBoard.board[col][row].smallRender.rotate(pos.atkFaceupFlat,0.2f);
					}else{
						theBoard.board[col][row].smallRender.rotate(pos.atkFacedownFlat,0.2f);
					}
				}
			}
		}
	}
	




}
