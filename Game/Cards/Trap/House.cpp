#include <Game\Cards\Trap\House.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\Clock.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Utility\SoundUnit.h>

#define ZYUG_GO 0
#define ZYUG_CR 1
#define ZYUG_DC 2
#define ZYUG_FINISH 5

namespace Card{
	House::House(int a): amount(a) {}

	void House::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		time = 0; speed = 2;
		blink1 = false; blink2 = false; blink3 = false; blink4 = false;
		crosshair.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/crosshairs.png");
		crosshair.ignoreCamera = false;
		crosshair.doRender = false;
		crosshair.scale = glm::vec3(0.2f,0.2f,1.0f);
		crosshair.amtran = glm::vec4(1.0f,0.5f,0.5f,1.0f);
		zOff = (theBoard.playerControlling()?0.3f:-0.3f);
		trapUnit.magicEnd = false;
		if(theBoard.playerControlling()){
			crosshair.rotate(glm::vec3(-1,0,0),0.5f,0.001f);
		}else{
			crosshair.rotate(glm::vec3(1,0,0),0.5f,0.001f);
		}
	}
	void House::cleanup(){
		crosshair.cleanup();
	}
	void House::render(){
		if(crosshair.doRender){
			for(unsigned int p = 0; p <posis.size(); p++){
				float x = posis[p].x;
				float y = posis[p].y + 0.15f;
				float z = posis[p].z + zOff;
				crosshair.position = glm::vec3(x,y,z);
				crosshair.render();
			}
		}
	}

	void House::update(){
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

	void House::startUpdate(){
		getPositions();
		wait(0.3f);
		if(posis.size()!=0){
			chain = ZYUG_CR;
		}else{
			chain = ZYUG_FINISH;
		}
		crosshair.doRender = true;
	}
	void House::crosshairUpdate(){
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

	void House::cardDestroyUpdate(){
		int j1 = YUG_BOARD_ENEMY_MON_ROW; int j2 = YUG_BOARD_PLAYER_MON_ROW;
		for(unsigned int i =0 ;i < 5; i++){
			if(!theBoard.board[i][j1].blankCard() && (theBoard.board[i][j1].attack<=amount)){
				theBoard.board[i][j1].smallRender.interpolateAmtran(glm::vec4(1,1,1,0),0.01f);
			}
			if(!theBoard.board[i][j2].blankCard() && (theBoard.board[i][j2].attack<=amount)){
				theBoard.board[i][j2].smallRender.interpolateAmtran(glm::vec4(1,1,1,0),0.01f);
			}
		}
		particleUnit.particleCardHit(posis,glm::vec4(1.0f,0.5f,0.2f,1.0f),0.2f,theBoard.playerControlling());
		wait(0.5f);
		crosshair.interpolateAmtran(glm::vec4(1,1,1,0),0.05f);
		chain = ZYUG_FINISH;
	}

	void House::finishUpdate(){
		destroyCards();
	}

	void House::destroyCards(){
		int j1 = YUG_BOARD_PLAYER_MON_ROW;
		int j2 = YUG_BOARD_ENEMY_MON_ROW;
		for(unsigned int i = 0; i < 5; i++){
			if(theBoard.board[i][j1].attack <= amount && !theBoard.board[i][j1].blankCard()){
				theBoard.board[i][j1].smallRender.cleanup();
				theBoard.board[i][j1].cleanup();
				theBoard.board[i][j1] = cardCreator.blankCard();
			}
			if(theBoard.board[i][j2].attack <= amount && !theBoard.board[i][j2].blankCard()){
				theBoard.board[i][j2].smallRender.cleanup();
				theBoard.board[i][j2].cleanup();
				theBoard.board[i][j2] = cardCreator.blankCard();
			}
		}
		trapUnit.chain = YUG_TRAP_CH_SPECIFIC_END;
		trapUnit.wait(1.0f);
	}

	void House::getPositions(){
		int j1 = YUG_BOARD_ENEMY_MON_ROW;
		int j2 = YUG_BOARD_PLAYER_MON_ROW;
		for(unsigned int i =0 ;i < 5; i++){
			if(!theBoard.board[i][j1].blankCard() && (theBoard.board[i][j1].attack <= amount)){
				posis.push_back(theBoard.board[i][j1].smallRender.position);
			}
			if(!theBoard.board[i][j2].blankCard() && (theBoard.board[i][j2].attack <= amount)){
				posis.push_back(theBoard.board[i][j2].smallRender.position);
			}
		}
	}

	bool House::listen(int info){
		if(info == YUG_TRAP_MON_PLAYED){
			return (theBoard.board[theBoard.cP[0]][theBoard.cP[1]].attack <= amount);
		}
		return false;
	}

}
