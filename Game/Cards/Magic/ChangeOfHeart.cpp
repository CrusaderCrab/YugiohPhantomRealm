#include <Game\Cards\Magic\ChangeOfHeart.h>
#include <Game\Duel\Board.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Utility\SoundUnit.h>
#include <Utility\DebugUnit.h>
#include <iostream>

#define ZYUG_MSTART 0
#define ZYUG_CARDSLIDEUP 5
#define ZYUG_CARDSWIPE 6
#define ZYUG_CARDFADEOUT 7
#define ZYUG_MPULLDOWN 1
#define ZYUG_DONE 3
#define ZYUG_FAIL -8

#define ZYUG_STARTPOS glm::vec3(-2.9f,-2.5f,-7.0f)
#define ZYUG_UP_OFF 2.0f
#define ZYUG_RIGHT_OFF 5.8f
#define ZYUG_SCALE glm::vec3(6.0f,5.0f,1.0f);

namespace Card{

	void ChangeOfHeart::startup(){
		Game::WaitUnit::startup();
		bg.startup(YUG_PLANE_FILE_PATH, "GameData/textures/magic/625bg.png");
		//std::cout<<"coh startup\n";
		bg.doRender = false;
		chain = ZYUG_MSTART;
	}
	void ChangeOfHeart::cleanup(){
		bg.cleanup();
	}
	void ChangeOfHeart::update(){
		bg.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_MSTART:
				startUpdate();
				break;
			case ZYUG_CARDSWIPE:
				thirdUpdate();
				break;
			case ZYUG_MPULLDOWN:
				pullDownUpdate();
				break;
			case ZYUG_DONE:
				finishedUpdate();
				break;

			default:
				break;
			}


		}else{
			continueWaiting();
		}
	}
	void ChangeOfHeart::render(){
		bg.render();
	}
	bool ChangeOfHeart::copyCardOver(){
		if(theBoard.playerControlling()){
			int col = getStrongestCol(YUG_BOARD_ENEMY_MON_ROW);
			if(col == ZYUG_FAIL){
				return false;
			}
			oldRow = YUG_BOARD_ENEMY_MON_ROW;
			oldColumn = col;
			card = &theBoard.board[col][YUG_BOARD_ENEMY_MON_ROW];
			return true;
		}else{
			int col = getStrongestCol(YUG_BOARD_PLAYER_MON_ROW);
			if(col == ZYUG_FAIL){
				return false;
			}
			oldRow = YUG_BOARD_PLAYER_MON_ROW;
			oldColumn = col;
			card = &theBoard.board[col][YUG_BOARD_PLAYER_MON_ROW];
			return true;
		}
	}
	void ChangeOfHeart::interpolateCardUp(){
		card->smallRender.interpolate(glm::vec3(0.0f,1.0f,0.0f),0.2f);
		card->smallRender.rotate(glm::mat4(-1,4,2,0, 2,9,-5,0, 1,-2,5,0, 0,0,0,1),0.2f);
		wait(0.2f);
	}
	void ChangeOfHeart::startUpdate(){
		if(!copyCardOver()){
			chain = ZYUG_DONE;
			return;
		}
		newColumn = findNewColumn();
		if(newColumn == ZYUG_FAIL){
			chain = ZYUG_DONE;
			return;
		}
		bg.ignoreCamera = true;
		//std::cout<<"coh start update\n";
		bg.rotationMatrix = glm::mat4();
		bg.doRender = true;
		bg.scale = ZYUG_SCALE;
		bg.amtran = glm::vec4(1.000f,1.0f,1.0f,0.0f);
		bg.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f),0.4f);
		bg.position = ZYUG_STARTPOS;
		soundUnit.playOnce("GameData/sounds/magic/cohChoir.wav");
		bg.interpolate(mov.addYOffset(bg.position,ZYUG_UP_OFF), 1.0f);
		wait(1.0f);
		chain = ZYUG_CARDSWIPE;
		
	}
	void ChangeOfHeart::thirdUpdate(){
		//std::cout<<"coh third\n";
		soundUnit.cardSwoosh();
		bg.interpolate(mov.addXOffset(bg.position,ZYUG_RIGHT_OFF),0.1f);
		interpolateCardUp();
		chain = ZYUG_MPULLDOWN;
		wait(0.1f);
	}

	void ChangeOfHeart::pullDownUpdate(){
		//std::cout<<"coh pull update\n";
		card->faceUp = true;
		changeOverData();
		theBoard.pullCardToBoard(newColumn,newRow);
		theBoard.rotateCardToBoard(newColumn, newRow);
		soundUnit.playOnce("GameData/sounds/magic/cohLaugh.wav");
		bg.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		bg.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),0.8f);
		chain = ZYUG_DONE;
		wait(0.8f);
	}
	void ChangeOfHeart::finishedUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
		bg.doRender = false;
	}
	int ChangeOfHeart::findNewColumn(){
		if(theBoard.playerControlling()){
			newRow = YUG_BOARD_PLAYER_MON_ROW;
			for(unsigned int i = 0; i < 5; i++){
				if(theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
					return i;
				}
			}
			return ZYUG_FAIL;
		}else{
			newRow = YUG_BOARD_ENEMY_MON_ROW;
			for(unsigned int i = 0; i < 5; i++){
				if(theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard()){
					return i;
				}
			}
			return ZYUG_FAIL;
		}
	}
	int ChangeOfHeart::getStrongestCol(int row){
		int highestAtk = -1;
		int strongestCol = ZYUG_FAIL;
		for(unsigned int j = 0; j < 5; j++){
			if(!theBoard.board[j][row].blankCard() && theBoard.board[j][row].attack > highestAtk){
				highestAtk = theBoard.board[j][row].attack;
				strongestCol = j;
			}
		}
		return strongestCol;
	}
	
	void ChangeOfHeart::changeOverData(){
		theBoard.board[newColumn][newRow] = theBoard.board[oldColumn][oldRow];
		theBoard.board[oldColumn][oldRow] = cardCreator.blankCard();
		cardCreator.setupParents(&theBoard.board[newColumn][newRow]);
	}









}