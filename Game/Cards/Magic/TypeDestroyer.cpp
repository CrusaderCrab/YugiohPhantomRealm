#include <Game\Cards\Magic\TypeDestroyer.h>
#include <Utility\Clock.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\SoundUnit.h>

#define ZYUG_GO 0
#define ZYUG_CR 1
#define ZYUG_DC 2
#define ZYUG_FINISH 5

namespace Card{

	TypeDestroyer::TypeDestroyer(int type, glm::vec4 colour){
		deadType = type;
		color = colour;
	}

	void TypeDestroyer::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		time = 0; speed = 2;
		blink1 = false; blink2 = false; blink3 = false; blink4 = false;
		crosshair.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/crosshairs.png");
		crosshair.ignoreCamera = false;
		crosshair.doRender = false;
		crosshair.scale = glm::vec3(0.2f,0.2f,1.0f);
		crosshair.amtran = color;
		getPositions();
		zOff = (theBoard.playerControlling()?0.3f:-0.3f);
	}

	void TypeDestroyer::cleanup(){
		crosshair.cleanup();
	}

	void TypeDestroyer::render(){
		if(crosshair.doRender){
			for(unsigned int p = 0; p <posis.size(); p++){
				float x = posis[p].x;
				float y = posis[p].y + 0.1f;
				float z = posis[p].z + zOff;
				crosshair.position = glm::vec3(x,y,z);
				crosshair.render();
			}
		}
	}

	void TypeDestroyer::update(){
		crosshair.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_CR:
				crosshairUpdate();
				break;
			case ZYUG_DC:
				cardDestroyUpdate();
				break;
			case ZYUG_FINISH:
				finishUpdate();
				break;
			default:
				break;
			}
		}else{
			continueWaiting();
		}
	}

	void TypeDestroyer::startUpdate(){
		wait(0.3f);
		if(posis.size()!=0){
			chain = ZYUG_CR;
		}else{
			chain = ZYUG_FINISH;
		}
		crosshair.doRender = true;
	}
	void TypeDestroyer::crosshairUpdate(){
		if(time==0){
			soundUnit.playOnce("GameData/sounds/magic/typeDestroyerNoise.wav");
		}
		time += gameClock.lastLoopTime()*speed;
		if(time > 0.2f && !blink1){
			blink1 = true;
			crosshair.doRender = false;
		}
		if(time > 0.4f && !blink2){
			blink2 = true;
			crosshair.doRender = true;
		}
		if(time > 1.0f){
			chain = ZYUG_DC;
			time = 0;
			speed = 1;
			wait(0.3f);
		}
	}

	void TypeDestroyer::cardDestroyUpdate(){
		int j = (theBoard.playerControlling()?YUG_BOARD_ENEMY_MON_ROW:YUG_BOARD_PLAYER_MON_ROW);
		if(deadType!=0){
			for(unsigned int i =0 ;i < 5; i++){
				if(!theBoard.board[i][j].blankCard() && (theBoard.board[i][j].actualType == deadType)){
					theBoard.board[i][j].smallRender.interpolateAmtran(glm::vec4(1,1,1,0),0.01f);
				}
			}
		}else{
			for(unsigned int i =0 ;i < 5; i++){
				if(!theBoard.board[i][j].blankCard() && (theBoard.board[i][j].attack>=1500)){
					theBoard.board[i][j].smallRender.interpolateAmtran(glm::vec4(1,1,1,0),0.01f);
				}
			}
		}
		particleUnit.particleCardHit(posis,glm::vec4(1.0f,0.5f,0.2f,1.0f),0.2f,theBoard.playerControlling());
		wait(0.5f);
		crosshair.interpolateAmtran(glm::vec4(1,1,1,0),0.05f);
		chain = ZYUG_FINISH;
	}

	void TypeDestroyer::finishUpdate(){
		int j = (theBoard.playerControlling()?YUG_BOARD_ENEMY_MON_ROW:YUG_BOARD_PLAYER_MON_ROW);
		if(deadType!=0){
			for(unsigned int i =0 ;i < 5; i++){
				if(!theBoard.board[i][j].blankCard() && (theBoard.board[i][j].actualType == deadType)){
					theBoard.board[i][j].smallRender.cleanup();
					theBoard.board[i][j].cleanup();
					theBoard.board[i][j] = cardCreator.blankCard();
				}
			}
		}else{
			for(unsigned int i =0 ;i < 5; i++){
				if(!theBoard.board[i][j].blankCard() && (theBoard.board[i][j].attack>=1500)){
					theBoard.board[i][j].smallRender.cleanup();
					theBoard.board[i][j].cleanup();
					theBoard.board[i][j] = cardCreator.blankCard();
				}
			}
		}
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}


	void TypeDestroyer::getPositions(){
		int j = (theBoard.playerControlling()?YUG_BOARD_ENEMY_MON_ROW:YUG_BOARD_PLAYER_MON_ROW);
		if(deadType!=0){
			for(unsigned int i =0 ;i < 5; i++){
				if(!theBoard.board[i][j].blankCard() && (theBoard.board[i][j].actualType == deadType)){
					posis.push_back(theBoard.board[i][j].smallRender.position);
				}
			}
		}else{//crush card
			for(unsigned int i =0 ;i < 5; i++){
				if(!theBoard.board[i][j].blankCard() && (theBoard.board[i][j].attack >= 1500)){
					posis.push_back(theBoard.board[i][j].smallRender.position);
				}
			}

		}
	}
	


}