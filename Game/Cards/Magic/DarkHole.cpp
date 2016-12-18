#include <Game\Cards\Magic\DarkHole.h>
#include <random>
#include <Utility\Clock.h>
#define GLM_FORCE_RADIANS
#include <gtc\matrix_transform.hpp>
#include <Game\Animation\MovementUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\SoundUnit.h>

#define ZYUG_GO 0
#define ZYUG_SUCK 1
#define ZYUG_FINISH 2

namespace Card{

	void DarkHole::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		bg.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/darkhole.png");
		bg.ignoreCamera = true;
		bg.amtran = glm::vec4(1,1,1,0);
		bg.rotationMatrix = glm::mat4();
		bg.position = glm::vec3(0.0f,0.8f,-9.0f);
		bg.scale = glm::vec3(4.5f,4.5f,4.5f);
		time = 0;
		getAllCardsOnBoard();
	}

	void DarkHole::cleanup(){
		bg.cleanup();
	}

	void DarkHole::render(){
		bg.render();
	}

	void DarkHole::update(){
		bg.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_SUCK:
				suckupUpdate();
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

	void DarkHole::startUpdate(){
		srand(0);
		soundUnit.playOnce("GameData/sounds/magic/darkHoleNoise2.wav");
		bg.interpolateAmtran(glm::vec4(1,1,1,1), 1.0f);
		chain = ZYUG_SUCK;
		wait(1.0f);
	}

	void DarkHole::suckupUpdate(){
		if(time == 0){
			unsigned int size = cards.size();
			for(unsigned int c = 0; c < size; c++){
				if(theBoard.playerControlling()){
					cards[c]->smallRender.interpolate(glm::vec3(0.0f,0.0f,-6.5f),1.8f);
				}else{
					cards[c]->smallRender.interpolate(glm::vec3(0.0f,0.0f,6.5f),1.8f);
				}
			}
		}
		time += gameClock.lastLoopTime() * 0.5f;
		if(time < 1.0f){
			unsigned int size = cards.size();
			for(unsigned int c = 0; c < size; c++){
				float x = (rand()%100)*0.0001f;
				float y = (rand()%100)*0.0001f;
				float z = (rand()%100)*0.0001f;
				glm::mat4 rot = glm::rotate(glm::mat4(),time*-18.0f,glm::vec3(c,1,0));
				glm::quat q1(rot);
				cards[c]->smallRender.rotationMatrix = glm::mat4_cast(q1*cardQuats[c]);
				cards[c]->smallRender.scale = glm::vec3(1-(time*2.0f),1-(time*2.0f),1);
			}
		}else{
			chain = ZYUG_FINISH;
			bg.interpolateAmtran(glm::vec4(1,1,1,0),1.0f);
			cleanupAllCards();
			wait(1.0f);
		}
	}

	void DarkHole::finishUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}

	void DarkHole::cleanupAllCards(){
		unsigned int size = cards.size();
		for(unsigned int c = 0; c < size; c++){
			cards[c]->smallRender.cleanup();
			cards[c]->cleanup();
		}
		for(unsigned int i = 0; i <5 ; i++){
			for(unsigned int j = 0; j<4; j++){
				theBoard.board[i][j] = cardCreator.blankCard();
			}
		}
	}

	void DarkHole::getAllCardsOnBoard(){
		std::vector<CardData*> cs;
		std::vector<glm::quat> qs;
		for(unsigned int i = 0; i <5 ; i++){
			for(unsigned int j = 0; j<4; j++){
				if(!theBoard.board[i][j].blankCard()){
					cs.push_back(&theBoard.board[i][j]);
					qs.push_back(glm::quat(theBoard.board[i][j].smallRender.rotationMatrix));
				}
			}
		}
		cards = cs;
		cardQuats = qs;
	}

}
