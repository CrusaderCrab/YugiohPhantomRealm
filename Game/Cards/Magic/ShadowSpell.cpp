#include <Game\Cards\Magic\ShadowSpell.h>
#include <Utility\Clock.h>
#define GLM_FORCE_RADIANS
#include <Game\Animation\ParticlesUnit.h>
#include <gtc\matrix_transform.hpp>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Utility\SoundUnit.h>
#include <Game\Duel\Board.h>


#define ZYUG_GO 0
#define ZYUG_CHAIN 1
#define ZYUG_FINISH 2

#define ZYUG_CHAIN_SCALE glm::vec3(0.15f,0.15f,0.15f)
#define ZYUG_CHAIN_START_Y 10.0f

namespace Card{

	void ShadowSpell::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		chainTime = 0;
		explosionSet = false;
		cardsToAlter = magicUnit.returnAllEnemyMonsters();
		theBoard.magicBeforeParticles = true;
		chainModel.startup(
			"GameData/models/magic/yugiohChain3.obj",
			"GameData/textures/magic/784rawchain4.png");
		chainModel.ignoreCamera = false;
		chainModel.doRender = true;
		chainModel.scale = ZYUG_CHAIN_SCALE;
		chainModel.rotationMatrix = glm::mat4();
	}

	void ShadowSpell::cleanup(){
		theBoard.magicBeforeParticles = false;
		chainModel.cleanup();
	}

	void ShadowSpell::update(){
		chainModel.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_CHAIN:
				chainFallUpdate();
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

	void ShadowSpell::render(){
		if(renderChains)
			renderingChains();
	}

	void ShadowSpell::renderingChains(){
		unsigned int noOfCards = cardsToAlter.size();
		float yOffset = (-8.0f * chainTime)+5.0f;
		for(unsigned int c = 0; c < noOfCards; c++){
			const glm::vec3& cen = cardsToAlter[c]->smallRender.position;
			chainModel.position = glm::vec3(cen.x, cen.y+yOffset, cen.z);
			chainModel.render();
		}

	}

	void ShadowSpell::startUpdate(){
		chain = ZYUG_CHAIN;
		renderChains = true;
		soundUnit.playOnce("GameData/sounds/magic/shadowSpellNoise.wav");
		wait(0.3f);
	}

	void ShadowSpell::chainFallUpdate(){
		chainTime += gameClock.lastLoopTime();
		if(chainTime > 0.45f && !explosionSet){
			explosionSet = true;
			std::vector<glm::vec3>vs;
			for(unsigned int i = 0; i < cardsToAlter.size(); i++){
				vs.push_back(cardsToAlter[i]->smallRender.position);
			}
			particleUnit.ignoreCamera = true;
			particleUnit.particleCardHit(vs,glm::vec4(0.7f,0.7f,1.0f,1.0f),0.5f,theBoard.playerControlling());
		}
		if(chainTime >= 1.0f){
			alterCardStats();
			chain = ZYUG_FINISH;
			renderChains = false;
			wait(0.5f);
		}
	}

	void ShadowSpell::finishUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}

	void ShadowSpell::alterCardStats(){
		unsigned int noOfCards = cardsToAlter.size();
		for(unsigned int c = 0; c < noOfCards; c++){
			
			cardsToAlter[c]->atkStatDrop += atkCalcLose(cardsToAlter[c]);
			cardsToAlter[c]->defStatDrop += defCalcLose(cardsToAlter[c]);
			cardsToAlter[c]->alterAttack(-1000);
			cardsToAlter[c]->alterDefense(-1000);
		}
	}

	int ShadowSpell::atkCalcLose(CardData* c){
		if(c->attack >= 1000)
			return 1000;
		return c->attack;
	}
	int ShadowSpell::defCalcLose(CardData* c){
		if(c->defense >= 1000)
			return 1000;
		return c->defense;
	}

}