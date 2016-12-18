#include <Game\Duel\Parts\SwordUnit.h>
#include <Game\Duel\Board.h>
#include <Game\VectorUnit.h>
#include <Utility\Clock.h>
#include <Utility\SoundUnit.h>

#define YUG_SWORDS_NOT_ACTIVE -1
#define ZYUG_IDLE 0
#define ZYUG_FALL 1
#define ZYUG_FADE 2
#define ZYUG_RESET 3
#define ZYUG_END_UPDATING 4
#define ZYUG_FALL2 5

namespace Duel{
	SwordUnit SwordUnit::swordUnitInstance; 
	void SwordUnit::startup(){
		Game::WaitUnit::startup();
		playerWait = YUG_SWORDS_NOT_ACTIVE;
		enemyWait = YUG_SWORDS_NOT_ACTIVE;
		doRender = false; doUpdate = false;
		playerView = false;
		setupSwords();
		wait(0.1f);
		chain = ZYUG_END_UPDATING;
	}
	void SwordUnit::cleanup(){
		playerWait = YUG_SWORDS_NOT_ACTIVE;
		enemyWait = YUG_SWORDS_NOT_ACTIVE;
		doRender = false; doUpdate = false;
		for(unsigned int i = 0; i < 14; i++){
			swords[i].cleanup();
		}
	}
	void SwordUnit::render(){
		if(doRender){
			for(unsigned int i = 0; i < 14; i++){
				swords[i].render();
			}
		}
	}
	void SwordUnit::update(){
		if(doUpdate){
			for(unsigned int i = 0; i < 14; i++){
				swords[i].update();
			}
		}
		if(!isWaiting){
			switch(chain){
			case ZYUG_FALL:
				fallingUpdate();
				break;
			case ZYUG_FALL2:
				fallingUpdate2();
				break;
			case ZYUG_FADE:
				fadeUpdate();
				break;
			case ZYUG_RESET:
				resetUpdate();
				break;
			case ZYUG_END_UPDATING:
				doUpdate = false;
				chain = ZYUG_IDLE;
				break;
			default:
				break;
			}
		}else{
			continueWaiting();
		}
	}

	void SwordUnit::fallingUpdate(){
		doUpdate = true;
		int offset = 0;
		if(playerView){
			offset = 7;
		}
		for(unsigned int i = offset; i < 7+offset; i++){
			glm::vec3 v = swords[i].position;
			v.y-=5.1f;
			swords[i].interpolate(v,0.5f);
			swords[i].doRender = true;
		}
		wait(0.4f);
		chain = ZYUG_FALL2;
	}
	void SwordUnit::fallingUpdate2(){
		int j = playerView?YUG_BOARD_ENEMY_MON_ROW:YUG_BOARD_PLAYER_MON_ROW;
		for(unsigned int i = 0; i < 5; i++){
			Card::CardData& c = theBoard.board[i][j];
			if(!c.blankCard() && !c.faceUp){
				theBoard.flipCard(i,j);
			}
		}
		chain = ZYUG_END_UPDATING;
		wait(0.2f);
	}
	void SwordUnit::fadeUpdate(){
		int offset = 0;
		playerReset = playerView;
		if(playerView){
			offset = 7;
		}
		for(unsigned int i = offset; i < 7+offset; i++){
			swords[i].interpolateAmtran(glm::vec4(1,1,1,0),1.0f);
		}
		chain = ZYUG_RESET;
	}
	void SwordUnit::resetUpdate(){
		int offset = 0;
		if(!playerReset){
			offset = 7;
		}
		for(unsigned int i = offset; i < 7+offset; i++){
			glm::vec3 v = swords[i].position;
			v.y+=5.1f;
			swords[i].interpolate(v,0.05f);
			swords[i].doRender = false;
			swords[i].interpolateAmtran(glm::vec4(1,1,1,1),0.05f);
		}
		wait(0.2f);
		chain = ZYUG_END_UPDATING;
		if(playerWait==enemyWait&&enemyWait==YUG_SWORDS_NOT_ACTIVE){
			doRender = false;
		}
	}
	void SwordUnit::activateSwords(){
		doRender = true;
		playerView = theBoard.playerControlling();
		if(!playerView){
			if(playerWait != YUG_SWORDS_NOT_ACTIVE)
				chain = ZYUG_END_UPDATING;
			else
				chain = ZYUG_FALL;
			playerWait = 3;
		}else{
			if(enemyWait != YUG_SWORDS_NOT_ACTIVE)
				chain = ZYUG_END_UPDATING;
			else
				chain = ZYUG_FALL;
			enemyWait = 3;
		}
	}
	int SwordUnit::currentPlayerCountLeft(){
		return (theBoard.playerControlling()?playerWait:enemyWait);
	}
	bool SwordUnit::currentPlayerCanAttack(){
		int no = (theBoard.playerControlling()?playerWait:enemyWait);
		return no == YUG_SWORDS_NOT_ACTIVE;
	}
	void SwordUnit::currentPlayerTurnEnd(){
		playerView = theBoard.playerControlling();
		if(playerView){
			if(playerWait != YUG_SWORDS_NOT_ACTIVE){
				playerWait--;
				if(playerWait <= 0){
					playerWait = YUG_SWORDS_NOT_ACTIVE;
					doUpdate = true;
					chain = ZYUG_FADE;
				}
			}
		}else{
			if(enemyWait != YUG_SWORDS_NOT_ACTIVE){
				enemyWait--;
				if(enemyWait <= 0){
					enemyWait = YUG_SWORDS_NOT_ACTIVE;
					doUpdate = true;
					chain = ZYUG_FADE;
				}
			}
		}
	}

	void SwordUnit::setupSwords(){
		for(unsigned int i = 0; i < 14; i++){
			swords[i].startup("GameData/models/magic/yugiohSword3.obj", 
			"GameData/textures/magic/sword.png");
			swords[i].scale = glm::vec3(0.2f,0.2f,0.2f);
			swords[i].doRender = false;
			swords[i].ignoreCamera = false;
			swords[i].rotate(glm::vec3(0.0f,1.0f,0.0f),(i),0.01f);
			swords[i].amtran = glm::vec4(1,1,1,1);
		}
		swords[0].position =  glm::vec3(	-0.826f,	4.55f,	0.3f);
		swords[1].position =  glm::vec3(	-0.616f,	4.55f,	0.4f);
		swords[2].position =  glm::vec3(	-0.259f,	4.51f,	0.5f);
		swords[3].position =  glm::vec3(	-0.036f,	4.55f,	0.45f);
		swords[4].position =  glm::vec3(	 0.456f,	4.52f,	0.35f);
		swords[5].position =  glm::vec3(	 0.55f, 	4.55f,	0.23f);
		swords[6].position =  glm::vec3(	 0.66f, 	4.54f,	0.54f);
							
		swords[7].position =  glm::vec3(	0.831f,	4.55f,	-0.46f);
		swords[8].position =  glm::vec3(	0.641f,	4.55f,	-0.37f);
		swords[9].position =  glm::vec3(	0.221f,	4.55f,	-0.58f);
		swords[10].position = glm::vec3(	-0.09f,	4.55f,	-0.39f);
		swords[11].position = glm::vec3(	-0.34f,	4.55f,	-0.21f);
		swords[12].position = glm::vec3(	-0.51f,	4.55f,	-0.52f);
		swords[13].position = glm::vec3(	-0.81f,	4.55f,	-0.33f);
	}







}
