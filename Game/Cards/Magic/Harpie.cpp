#include <Game\Cards\Magic\Harpie.h>
#include <Utility\Clock.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\SoundUnit.h>

#define ZYUG_GO 0
#define ZYUG_SWEEP 1
#define ZYUG_UP 2
//#define ZYUG_FADE_IN 3
//#define ZYUG_FADE_OUT 4
#define ZYUG_FINISH 5

namespace Card{

	void Harpie::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		time = 0; speed = 1; rotCount = 0;
		player = theBoard.playerControlling();
		correctRow = (player?YUG_BOARD_ENEMY_MAG_ROW:YUG_BOARD_PLAYER_MAG_ROW);
		//theBoard.magicBeforeParticles = true;
		ticker = false; featherMove = false;
		m1 = false; m2 = false; m3 = false; m4 = false; m5 = false;
		feather.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/duster.png");
		feather.ignoreCamera = false;
		feather.doRender = true;
		feather.amtran = glm::vec4(1,1,1,0);
		feather.scale = glm::vec3(0.2f,0.2f,1.0f);
		if(player){
			feather.rotationMatrix = glm::mat4();
			feather.position = glm::vec3(-0.9f, 0.5f, 0.38f);
		}else{
			feather.rotationMatrix = glm::mat4();//glm::mat4(-1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
			feather.position = glm::vec3(-0.9f, 0.5f, -0.38f);
		}
	}

	void Harpie::cleanup(){
		feather.cleanup();
	}
	void Harpie::render(){
		feather.render();
	}
	void Harpie::update(){
		feather.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_SWEEP:
				sweepingUpdate();
				break;
			case ZYUG_UP:
				upwardsUpdate();
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


	void Harpie::startUpdate(){
		glm::vec3 v = feather.position;
		v.y -= 0.5f;
		feather.interpolate(v, 0.3f);
		feather.interpolateAmtran(glm::vec4(1,1,1,1),0.3f);
		wait(0.3f);
		speed = 0.33333f;
		soundUnit.playOnce("GameData/sounds/magic/harpieNoise.wav");
		chain = ZYUG_SWEEP;
	}
	void Harpie::sweepingUpdate(){
		float t = gameClock.lastLoopTime()*speed;
		rotCount += t; time +=t;
		if(rotCount > 0.1f){
			rotCount = 0;
			ticker = (!ticker);
			if(ticker){
				feather.rotate(glm::vec3(1,0,0),1.0f,0.2f);
			}else{
				feather.rotate(glm::vec3(-1,0,0),1.0f,0.2f);
			}
		}
		if(!featherMove){
			featherMove = true;
			if(player){
				feather.interpolate(glm::vec3(0.9f, 0.0f, 0.38f),3.0f);
			}else{
				feather.interpolate(glm::vec3(0.9f, 0.0f, -0.38f),3.0f);
			}
		}
		if(time>0.12f && !m1){ m1 = true; vanishMagicCard(0); }
		if(time>0.25f && !m2){ m2 = true; vanishMagicCard(1); }
		if(time>0.42f && !m3){ m3 = true; vanishMagicCard(2); }
		if(time>0.57f && !m4){ m4 = true; vanishMagicCard(3); }
		if(time>0.75f && !m5){ m5 = true; vanishMagicCard(4); }
		if(time > 1.0f){
			chain = ZYUG_UP;
		}
	}
	void Harpie::upwardsUpdate(){
		glm::vec3 v = feather.position;
		v.y += 0.5f;
		feather.interpolate(v, 0.3f);
		feather.interpolateAmtran(glm::vec4(1,1,1,0),0.3f);
		wait(0.5f);
		for(unsigned int i = 0; i <5; i++){
			removeMagicCard(i);
		}
		chain = ZYUG_FINISH;
	}
	void Harpie::finishUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}

	void Harpie::vanishMagicCard(int col){
		if(!theBoard.board[col][correctRow].blankCard()){
			theBoard.board[col][correctRow].smallRender.interpolateAmtran(
				glm::vec4(1,1,1,0),0.2f);
			std::vector<glm::vec3> v;
			v.push_back(theBoard.board[col][correctRow].smallRender.position);
			particleUnit.particleCardHit(v,glm::vec4(0.6f,1.0f,1.0f,1.0f),0.3f,player);
		}
	}
	void Harpie::removeMagicCard(int col){
		if(!theBoard.board[col][correctRow].blankCard()){
			theBoard.board[col][correctRow].smallRender.cleanup();
			theBoard.board[col][correctRow].cleanup();
			theBoard.board[col][correctRow] = cardCreator.blankCard();
		}
	}

}