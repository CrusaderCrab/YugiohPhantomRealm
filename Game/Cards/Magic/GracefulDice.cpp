#include <Game\Cards\Magic\GracefulDice.h>
#include <random>
#include <ctime>
#include <Utility\Clock.h>
#include <gtc\matrix_transform.hpp>
#include <Game\Animation\MovementUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Duel\Board.h>
#include <Utility\SoundUnit.h>
#include <iostream>

#define ZYUG_GO 0
#define ZYUG_ROT1 1
#define ZYUG_ROT2 2
#define ZYUG_FADEIN_TEXT 3
#define ZYUG_FADEIN_CARD 6
#define ZYUG_PARTICLES 7
#define ZYUG_FADEIN_COUNT 9
#define ZYUG_FADEOUT_COUNT 10
#define ZYUG_FADEOUT_CARD 8
#define ZYUG_FADEOUT_TEXT 4
#define ZYUG_FINISH 5
#define ZYUG_ROTLINE glm::vec3(1.0f,0.0f,-1.0f)
#define ZYUG_ROT1DISTANCE 1.0f
#define ZYUG_ROT2DISTANCE 0.8f
#define ZYUG_ROT1CENTRE glm::vec3(1.8f, -0.55f, -3.4f)
#define ZYUG_ROT2CENTRE glm::vec3(0.0f, -0.55f, -5.2f)
#define ZYUG_DSCALE glm::vec3(0.15f,0.15f,0.15f)
#define ZYUG_DSTARTPOS glm::vec3(5.0f,0.0f,-3.0f)


namespace Card{

	void GracefulDice::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		//std::cout<<"Gdice startup\n";
		dice.startup("GameData/models/magic/626dice.obj",
			"GameData/textures/magic/626dice.png");
		chooseResult();
		calcStartingRotation();
		setupDice();
		renderText = false;

	}
	void GracefulDice::cleanup(){
		dice.cleanup();
	}
	void GracefulDice::update(){
		dice.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_ROT1:
				firstRotationUpdate();
				break;
			case ZYUG_ROT2:
				secondRotationUpdate();
				break;
			case ZYUG_FADEIN_TEXT:
				fadeTextInUpdate();
				break;
			case ZYUG_FADEIN_CARD:
				fadeCardInUpdate();
				break;
			case ZYUG_PARTICLES:
				particleRiseUpdate();
				break;
			case ZYUG_FADEIN_COUNT:
				finishFadeInCount();
				break;
			case ZYUG_FADEOUT_COUNT:
				startFadeOutCount();
				break;
			case ZYUG_FADEOUT_TEXT:
				fadeTextOutUpdate();
				break;
			case ZYUG_FINISH:
				finishupdate();
				break;
			default:

				break;

			}
		}else{
			continueWaiting();
		}

	}
	void GracefulDice::render(){
		dice.render();
		if(renderText){
			textPrinter.ignoreCamera = true;
			textPrinter.leftAlign = false;
			textPrinter.printMagicNumber(gain,glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.07f, 0.0f,-0.8f));
		}
	}

	void GracefulDice::startUpdate(){
		rotationTime = 0.0f;
		rotationSpeed = 1/2.0f;
		cardsToAlter = magicUnit.returnAllPlayerMonsters();
		chain = ZYUG_ROT1;
		//std::cout<<"rot1 start\n";
	}
	void GracefulDice::firstRotationUpdate(){
		rotationTime += gameClock.lastLoopTime()*rotationSpeed;
		if(rotationTime < 1.0f){
			dice.position = mov.rotateAroundXZ(
				ZYUG_ROT1CENTRE,ZYUG_ROTLINE,
				1.0f,rotationTime*-YUG_PI);
			calcDiceRotation();
		}else{
			soundUnit.playOnce("GameData/sounds/magic/diceHitBoard.wav");
			chain = ZYUG_ROT2;
			//std::cout<<"rot1 end\n";
			rotationTime = 0.0f;
			rotationSpeed = 1/2.0f;
			startingRotation = glm::quat(dice.rotationMatrix);
		}
	}
	void GracefulDice::calcDiceRotation(){
		glm::mat4 rot = glm::rotate(glm::mat4(),rotationTime*-9.424778f,ZYUG_ROTLINE);
		glm::quat q1(rot);
		dice.rotationMatrix = glm::mat4_cast(q1*startingRotation);
	}

	void GracefulDice::secondRotationUpdate(){
		rotationTime += gameClock.lastLoopTime()*rotationSpeed;
		if(rotationTime < 1.0f){
			dice.position = mov.rotateAroundXZ(
				ZYUG_ROT2CENTRE,ZYUG_ROTLINE,
				ZYUG_ROT2DISTANCE,rotationTime*-YUG_HALF_PI);
			calcDiceRotation();
		}else{
			chain = ZYUG_FADEIN_TEXT;
			//std::cout<<"rot2 end\n";
		}
	}
	void GracefulDice::fadeTextInUpdate(){
		soundUnit.playOnce("GameData/sounds/magic/goodTextSound2.wav");
		magicUnit.startingAmtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		magicUnit.currentAmtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		textPrinter.magicAmtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		magicUnit.interpTextAmtran(YUG_MAG_GOOD_COLOR,0.8f);
		renderText = true;
		wait(0.3f);
		chain = ZYUG_FADEIN_CARD;
		rotationTime = 0.0f;
		rotationSpeed = 1.0f;
	}
	void GracefulDice::fadeCardInUpdate(){
		rotationTime += gameClock.lastLoopTime()*rotationSpeed;
		if(rotationTime < 0.3f){
			unsigned int c = cardsToAlter.size();
			for(unsigned int i = 0; i <c; i++){
				cardsToAlter[i]->smallRender.amtran = glm::vec4(1.0f-rotationTime*1.5f,1.0f+rotationTime*10.0f,1.0f-rotationTime*1.5f, 1.0f); 
			}
		}else{
			chain = ZYUG_PARTICLES;
		}
	}
	void GracefulDice::particleRiseUpdate(){
		particleUnit.ignoreCamera = false;
		particleUnit.particleRise(getCardCentres(), glm::vec4(0.5f,1.0f,0.5f,1.0f), 2.0f, theBoard.playerControlling());
		alterCardStats();
		chain = ZYUG_FADEIN_COUNT;
	}

	void GracefulDice::finishFadeInCount(){
		rotationTime += gameClock.lastLoopTime()*rotationSpeed;
		if(rotationTime < 1.0f){
			unsigned int c = cardsToAlter.size();
			for(unsigned int i = 0; i <c; i++){
				cardsToAlter[i]->smallRender.amtran = glm::vec4(1.0f-rotationTime*1.5f,1.0f+rotationTime*10.0f,1.0f-rotationTime*1.5f, 1.0f); 
			}
		}else{
			dice.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),0.5f);
			chain = ZYUG_FADEOUT_COUNT;
			rotationTime = 0.0f;
		}
	}
	void GracefulDice::startFadeOutCount(){
		rotationTime += gameClock.lastLoopTime()*rotationSpeed;
		if(rotationTime < 1.0f){
			unsigned int c = cardsToAlter.size();
			for(unsigned int i = 0; i <c; i++){
				cardsToAlter[i]->smallRender.amtran = glm::vec4(rotationTime,10.0f-rotationTime*9.0f,rotationTime, 1.0f); 
			}
		}else{
			unsigned int c = cardsToAlter.size();
			for(unsigned int i = 0; i <c; i++){
				cardsToAlter[i]->smallRender.amtran = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
			}
			magicUnit.startingAmtran = YUG_MAG_GOOD_COLOR;
			magicUnit.currentAmtran = YUG_MAG_GOOD_COLOR;
			textPrinter.magicAmtran = YUG_MAG_GOOD_COLOR;
			magicUnit.interpTextAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),0.2f);
			chain = ZYUG_FADEOUT_TEXT;
			rotationSpeed = 1.0f;
			rotationTime = 0.0f;
		}
	}

	void GracefulDice::fadeTextOutUpdate(){
		rotationTime += gameClock.lastLoopTime()*rotationSpeed;
		if(rotationTime > 0.5f){
			chain = ZYUG_FINISH;
			renderText = false;
			dice.doRender = false;
			wait(0.3f);
		}
	}
	void GracefulDice::finishupdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}

	std::vector<glm::vec3> GracefulDice::getCardCentres(){
		std::vector<glm::vec3> vs;
		for(unsigned i = 0; i < cardsToAlter.size(); i++){
			vs.push_back(cardsToAlter[i]->smallRender.position);
		}
		return vs;
	}
	void GracefulDice::alterCardStats(){
		for(unsigned i = 0; i < cardsToAlter.size(); i++){
			cardsToAlter[i]->alterAttack(gain);
			cardsToAlter[i]->alterDefense(gain);
		}
	}

	void GracefulDice::chooseResult(){
		srand(time(NULL));
		gain = 100 * (rand()%6+1);
	}
	void GracefulDice::calcStartingRotation(){
		switch(gain){
		case 100:
			startingRotation = glm::quat(glm::rotate(glm::mat4(),YUG_HALF_PI, glm::vec3(0.0f,-1.0f,0.0f)));
			break;
		case 200:
			startingRotation = glm::quat(glm::rotate(glm::mat4(),YUG_HALF_PI, glm::vec3(-1.0f,0.0f,0.0f)));
			break;
		case 300:
			startingRotation = glm::quat(glm::mat4());
			break;
		case 400:
			startingRotation = glm::quat(glm::rotate(glm::mat4(),YUG_PI, glm::vec3(-1.0f,0.0f,0.0f)));
			break;
		case 500:
			startingRotation = glm::quat(glm::rotate(glm::mat4(),YUG_HALF_PI, glm::vec3(1.0f,0.0f,0.0f)));
			break;
		default:
			startingRotation = glm::quat(glm::rotate(glm::mat4(),YUG_HALF_PI, glm::vec3(0.0f,1.0f,0.0f)));
			break;
		}

	}
	void GracefulDice::setupDice(){
		dice.ignoreCamera = true;
		dice.doRender = true;
		dice.scale = ZYUG_DSCALE;
		dice.position = ZYUG_DSTARTPOS;

	}
	void GracefulDice::increaseValues(){


	}
}




