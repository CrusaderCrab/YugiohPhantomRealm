#include <Game\Cards\Magic\Raigeki.h>
#include <Utility\Clock.h>
#include <Utility\StateUnit.h>
#include <Game\Animation\Camera.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\SoundUnit.h>
#include <Game\Animation\FadeUnit.h>

#define ZYUG_GO 0
#define ZYUG_FALL 1
#define ZYUG_EXP 2
#define ZYUG_FIN 3
#define LENGTH 7

namespace Card{

	void Raigeki::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		lNum = 0;
		startUpLightning();
		time = 0;
		speed = 2;
		playerView = theBoard.playerControlling();
		offMove = (playerView?0.001f:-0.001f);
		explosionOff = (playerView?0.4f:-0.4f);
		cardsToAlter = magicUnit.returnAllEnemyMonsters();
		for(unsigned int i = 0; i<cardsToAlter.size(); i++){
			cardPos.push_back(cardsToAlter[i]->smallRender.position);
		}

		explosion.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/explosion1.png");
		explosion.ignoreCamera = false;
		explosion.doRender = false;
		explosion.rotationMatrix = glm::mat4();
		explosion.scale = glm::vec3(0.001f,0.001f,1.0f);
		explosion.amtran = glm::vec4(1,1,1,0);
		expSetOff = false;
	}
	void Raigeki::cleanup(){
		for(unsigned int i = 0; i < LENGTH; i++){
			lightnings[i].cleanup();
		}
	}

	void Raigeki::startUpdate(){
		chain = ZYUG_FALL;
		wait(0.3f);
		soundUnit.playOnce("GameData/sounds/magic/ragiekiNoise.wav");
	}

	void Raigeki::fallingUpdate(){
		time += gameClock.lastLoopTime() * speed;
		updateLNum(time);
		if(time > 0.6f && !expSetOff){
			expSetOff = true;
			explosion.interpolateAmtran(glm::vec4(1,1,1,1),0.1f);
			explosion.scaleInterpolate(glm::vec3(0.5f,0.3f,1.0f),0.2f);
			explosion.doRender = true;
			fadeUnit.sheet.amtran = glm::vec4(1,1,1,0);
			//fadeUnit.fadeTo(glm::vec4(1,1,1,1),0.1f);
		}
		if(time > 0.7f){
			//fadeUnit.fadeTo(glm::vec4(1,1,1,0),0.3f);
			chain = ZYUG_EXP;
			expSetOff = false;
		}
	}
	void Raigeki::explosionUpdate(){
		time += gameClock.lastLoopTime() * speed;
		updateLNum(time);
		if(time > 0.9f && !expSetOff){
			expSetOff = true;
			removeCards();
			explosion.interpolateAmtran(glm::vec4(1,1,1,0),0.1f);
			for(unsigned int i = 0; i < LENGTH; i++){
				lightnings[i].interpolateAmtran(glm::vec4(1,1,1,0),0.1f);
			}
		}
		if(time > 1.0f){
			chain = ZYUG_FIN;
			wait(0.4f);
		}
	}
	void Raigeki::finishUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}


	void Raigeki::render(){
		int lOff = 0;
		float y = 5 - time*5;
		for(unsigned int c = 0; c < cardPos.size(); c++){
			float x = cardPos[c].x;
			float z = cardPos[c].z;
			float cy = cardPos[c].y;
			glm::vec3 posi(x,y+cy,z);
			int l = (lNum+(lOff++))%7;
			lightnings[l].position = posi;
			lightnings[l].render();
			posi.z += offMove;

			l = (lNum+(lOff++))%7;
			lightnings[l].position = posi;
			lightnings[l].render();
			posi.z += offMove;

			l = (lNum+(lOff++))%7;
			lightnings[l].position = posi;
			lightnings[l].render();

			if(chain == ZYUG_EXP || time >0.5f){
				posi.y = cy+0.25f;
				posi.z = z+explosionOff+(offMove*c);
				explosion.position = posi;
				explosion.render();
			}
		}
	}

	void Raigeki::update(){
		for(unsigned int i = 0; i < LENGTH; i++){
			lightnings[i].update();
		}
		explosion.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_FALL:
				fallingUpdate();
				break;
			case ZYUG_EXP:
				explosionUpdate();
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

	void Raigeki::startUpLightning(){
		lightnings[0].startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/lightning1.png");
		lightnings[1].startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/lightning2.png");
		lightnings[2].startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/lightning3.png");
		lightnings[3].startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/lightning4.png");
		lightnings[4].startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/lightning5.png");
		lightnings[5].startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/lightning6.png");
		lightnings[6].startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/lightning7.png");
		for(unsigned int i = 0; i < LENGTH; i++){
			lightnings[i].ignoreCamera = false;
			lightnings[i].scale = glm::vec3(0.2f,2.5f,1.0f);
			lightnings[i].rotationMatrix = glm::mat4();
		}
	}

	void Raigeki::addThisCardPosition(CardData* c, std::vector<int>& positions){
		for(unsigned int i = 0; i <5 ; i++){
			for(unsigned int j = 0; j<4; j++){
				if(c == &theBoard.board[i][j]){
					positions.push_back(i); positions.push_back(j);
					return;
				}
			}
		}
	}

	void Raigeki::removeCards(){
		unsigned int size = cardsToAlter.size();
		std::vector<int>cardPositions;
		for(unsigned int i = 0; i < size; i++){
			addThisCardPosition(cardsToAlter[i], cardPositions);
		}

		for(unsigned int c = 0; c < size; c++){
			cardsToAlter[c]->smallRender.cleanup();
			cardsToAlter[c]->cleanup();
		}
		for(unsigned int i = 0; i <cardPositions.size(); i+=2){
			theBoard.board[cardPositions[i]][cardPositions[i+1]] 
				= cardCreator.blankCard();
		}
	}

	void Raigeki::updateLNum(float t){
		if(t>0.9f){lNum = 6;return;}
		if(t>0.8f){lNum = 0;return;}
		if(t>0.7f){lNum = 1;return;}
		if(t>0.6f){lNum = 5;return;}
		if(t>0.5f){lNum = 3;return;}
		if(t>0.4f){lNum = 4;return;}
		if(t>0.3f){lNum = 6;return;}
		if(t>0.2f){lNum = 3;return;}
		if(t>0.1f){lNum = 1;return;}
	}

}
