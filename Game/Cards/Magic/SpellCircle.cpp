#include <Game\Cards\Magic\SpellCircle.h>
#include <Utility\Clock.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Duel\Board.h>
#include <Utility\SoundUnit.h>

#define ZYUG_GO 0
#define ZYUG_FADE_CIRCLE 1
#define ZYUG_FADE_CARD 2
#define ZYUG_FADE_IN 3
#define ZYUG_FADE_OUT 4
#define ZYUG_FINISH 5


namespace Card{

	void SpellCircle::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		circle.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/magic/783circle2.png");
		circle.ignoreCamera = true;
		circle.doRender = false;
		circle.amtran = glm::vec4(1,1,1,0);
		circle.scale = glm::vec3(2,2,2);
		circle.position = glm::vec3(0.0f,0.5f,-7.0f);
		circle.rotationMatrix = glm::mat4();
		theBoard.magicBeforeParticles = true;
	}

	void SpellCircle::cleanup(){
		circle.cleanup();
		theBoard.magicBeforeParticles = false;
	}

	void SpellCircle::render(){
		circle.render();
	}

	void SpellCircle::update(){
		circle.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_FADE_CIRCLE:
				circleFadeInUpdate();
				break;
			case ZYUG_FADE_CARD:
				cardFadeInUpdate();
				break;
			case ZYUG_FADE_IN:
				finishFadeInUpdate();
				break;
			case ZYUG_FADE_OUT:
				fadeOutUpdate();
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

	void SpellCircle::startUpdate(){
		cardsToAlter = magicUnit.returnAllEnemyMonsters();
		cardTime = 0;
		cardSpeed = 0.5f;
		chain = ZYUG_FADE_CIRCLE;
		wait(0.1f);
	}

	void SpellCircle::circleFadeInUpdate(){
		circle.doRender = true;
		soundUnit.playOnce("GameData/sounds/magic/spellCircleNoise.wav");
		circle.rotate(glm::mat4(-2,0,0,0, 0,-2,0,0, 0,0,1,0, 0,0,0,1), 4.0f);
		circle.interpolateAmtran(glm::vec4(1,1,1,1), 1.0f);
		chain = ZYUG_FADE_CARD;
		wait(0.5f);
	}

	void SpellCircle::cardFadeInUpdate(){
		cardTime += gameClock.lastLoopTime()*cardSpeed;
		if(cardTime < 0.2f){
			unsigned int c = cardsToAlter.size();
			for(unsigned int i = 0; i <c; i++){
				cardsToAlter[i]->smallRender.amtran = glm::vec4(1.0f+cardTime*10.0f,1.0f-cardTime*1.5f,1.0f-cardTime*1.5f, 1.0f); 
			}
		}else{
			particleUnit.ignoreCamera = false;
			particleUnit.particleRise(getCardCentres(), glm::vec4(1.0f,0.5f,0.5f,1.0f), 2.0f, theBoard.playerControlling());
			alterCardStats();
			chain = ZYUG_FADE_IN;
		}
	}

	void SpellCircle::finishFadeInUpdate(){
		cardTime += gameClock.lastLoopTime()*cardSpeed;
		if(cardTime < 1.0f){
			unsigned int c = cardsToAlter.size();
			for(unsigned int i = 0; i <c; i++){
				cardsToAlter[i]->smallRender.amtran = glm::vec4(1.0f+cardTime*10.0f,1.0f-cardTime*1.5f,1.0f-cardTime*1.5f, 1.0f); 
			}
		}else{
			chain = ZYUG_FADE_OUT;
			cardTime = 0;
			circle.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),1.0f);
			//circle.rotate(glm::mat4(-1.01f,0.0f,0.0f,0.0f, 0.0f,-1.01f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f), 2.5f);
		}
	}

	void SpellCircle::fadeOutUpdate(){
		cardTime += gameClock.lastLoopTime()*cardSpeed;
		if(cardTime < 1.0f){
			unsigned int c = cardsToAlter.size();
			for(unsigned int i = 0; i <c; i++){
				cardsToAlter[i]->smallRender.amtran = glm::vec4(11.0f-cardTime*10.0f,cardTime,cardTime, 1.0f);  
			}
		}else{
			chain = ZYUG_FINISH;
			unsigned int c = cardsToAlter.size();
			for(unsigned int i = 0; i <c; i++){
				cardsToAlter[i]->smallRender.amtran = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
			}
		}
	}

	void SpellCircle::finishUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}

	std::vector<glm::vec3> SpellCircle::getCardCentres(){
		std::vector<glm::vec3> vs;
		for(unsigned i = 0; i < cardsToAlter.size(); i++){
			vs.push_back(cardsToAlter[i]->smallRender.position);
		}
		return vs;
	}

	void SpellCircle::alterCardStats(){
		for(unsigned i = 0; i < cardsToAlter.size(); i++){
			cardsToAlter[i]->atkStatDrop += atkCalcLose(cardsToAlter[i]);
			cardsToAlter[i]->defStatDrop += defCalcLose(cardsToAlter[i]);
			cardsToAlter[i]->alterAttack(-500);
			cardsToAlter[i]->alterDefense(-500);
			
		}
	}

	int SpellCircle::atkCalcLose(CardData* c){
		if(c->attack >= 500)
			return 500;
		return c->attack;
	}
	int SpellCircle::defCalcLose(CardData* c){
		if(c->defense >= 500)
			return 500;
		return c->defense;
	}


}