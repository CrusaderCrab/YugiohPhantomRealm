#include <Game\Cards\Magic\HealMagic.h>
#include <Utility\Clock.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Duel\Board.h>
#include <Utility\SoundUnit.h>
#include <Game\Duel\EndUnit.h>

#define ZYUG_GO 0
#define ZYUG_TEXT 1
#define ZYUG_GLIM 2
#define ZYUG_FADE 3
#define ZYUG_FIN 4
#define INVERT_ROT glm::mat4(-1,0,0,0, 0,-1,0,0, 0,0,1,0, 0,0,0,1)

namespace Card{

	HealMagic::HealMagic(int a){
		amount = a;
	}
	void HealMagic::startup(){
		Game::WaitUnit::startup();
		reversed = false;
		player = theBoard.playerControlling();
		time = 0 ;speed = 0.5f;
		setUpModels();
		theBoard.magicBeforeParticles = true;
		chain = ZYUG_GO;
		b1 = false; b2 = false; b3 = false; b4 = false; b5 = false;
	}
	void HealMagic::cleanup(){
		sp1.cleanup();
		sp2.cleanup();
		sp3.cleanup();
		fg.cleanup();
		theBoard.magicBeforeParticles = false;
	}

	void HealMagic::update(){
		sp1.update(); sp2.update();
		sp3.update(); fg.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_TEXT:
				textFadeInUpdate();
				break;
			case ZYUG_GLIM:
				glimmerUpdate();
				break;
			case ZYUG_FADE:
				fadeOutUpdate();
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

	void HealMagic::render(){
		fg.render();
		textPrinter.leftAlign = false;
		textPrinter.ignoreCamera = true;
		if(chain >= ZYUG_TEXT)
			textPrinter.printMagicNumber(amount,glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.07f, 0.0f,-0.8f));
		textPrinter.ignoreCamera = false;
		textPrinter.leftAlign = true;
		sp1.render();
		sp2.render();
		sp3.render();
	}

	void HealMagic::startUpdate(){
		theBoard.field.hide();
		theBoard.LP.hide();
		fg.doRender = true;
		if(!reversed){
			soundUnit.playOnce("GameData/sounds/magic/goodTextSound2.wav");
			fg.interpolateAmtran(glm::vec4(1,1,1,1),0.8f);
		}else{
			soundUnit.playOnce("GameData/sounds/magic/healCoughingNoise.wav");
			fg.interpolateAmtran(glm::vec4(0.2f,1.0f,0.7f,1.0f),0.8f);
		}
		wait(0.2f);
		chain = ZYUG_TEXT;
		magicUnit.startingAmtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		magicUnit.currentAmtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		textPrinter.magicAmtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
	}

	void HealMagic::textFadeInUpdate(){
		if(!reversed)
			magicUnit.interpTextAmtran(YUG_MAG_GOOD_COLOR,0.2f);
		else
			magicUnit.interpTextAmtran(YUG_MAG_BAD_COLOR,0.2f);
		wait(0.3f);
		chain = ZYUG_GLIM;
	}

	void HealMagic::glimmerUpdate(){
		time += gameClock.lastLoopTime()*speed;
		if(time>0.2f && !b1){
			b1 = true;
			sp1.doRender = true;
			sp1.interpolate(glm::vec3(0.2f,0.15f,-0.79f),1.2f);
			sp1.scaleInterpolate(glm::vec3(0.05f,0.05f,1.0f),0.3f);
			sp1.rotate(INVERT_ROT, 1.2f);
		}
		if(time>0.3f && !b2){
			b2 = true;
			sp2.doRender = true;
			sp2.interpolate(glm::vec3(-0.05f,0.1f,-0.79f),1.2f);
			sp2.scaleInterpolate(glm::vec3(0.1f,0.1f,1.0f),0.6f);
			sp2.rotate(INVERT_ROT, 1.5f);
		}
		if(time>0.5f && !b3){
			b3 = true;
			sp1.scaleInterpolate(glm::vec3(0.0002f,0.0002f,1.0f),0.3f);
			sp3.doRender = true;
			sp3.interpolate(glm::vec3(-0.2f,-0.03f,-0.79f),1.2f);
			sp3.scaleInterpolate(glm::vec3(0.07f,0.07f,1.0f),0.2f);
			sp3.rotate(INVERT_ROT, 0.5f);
		}
		if(time>0.6f && !b4){
			b4 = true;
			sp3.scaleInterpolate(glm::vec3(0.0002f,0.0002f,1.0f),0.3f);
		}
		if(time >0.8f && !b5){
			b5 = true;
			sp2.scaleInterpolate(glm::vec3(0.0002f,0.0002f,1.0f),0.4f);
		}
		if(time > 1.0f){
			chain = ZYUG_FADE;
			sp1.doRender = false; sp2.doRender = false; sp3.doRender = false;
		}
	}

	void HealMagic::fadeOutUpdate(){
		magicUnit.interpTextAmtran(glm::vec4(1,1,1,0),0.3f);
		fg.interpolateAmtran(glm::vec4(1,1,1,0),0.5f);
		chain = ZYUG_FIN;
		wait(0.6f);
	}

	void HealMagic::finishUpdate(){
		if(reversed)
			amount = -amount;
		if(player){
			theBoard.player.removeLP(-amount);
			if(!reversed)
				endUnit.magicCardHealing += amount;
		}else{
			theBoard.enemy.removeLP(-amount);
			if(reversed)
				endUnit.magicCardDamage -= amount;
		}
		theBoard.LP.reveal();
		theBoard.field.reveal();
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}


	void HealMagic::setUpModels(){
		sp1.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/particles/sparkle.png");
		sp2.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/particles/sparkle.png");
		sp3.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/particles/sparkle.png");
		sp1.ignoreCamera = true;
		sp1.doRender = false;
		sp1.amtran = glm::vec4(1,1,1,1);
		sp1.scale = glm::vec3(0.001f,0.001f,1.0f);
		sp1.position = glm::vec3(0.2f,0.0f,-0.79f);
		sp2.ignoreCamera = true;
		sp2.doRender = false;
		sp2.amtran = glm::vec4(1,1,1,1);
		sp2.scale = glm::vec3(0.001f,0.001f,1.0f);
		sp2.position = glm::vec3(-0.05f,0.0f,-0.79f);
		sp3.ignoreCamera = true;
		sp3.doRender = false;
		sp3.amtran = glm::vec4(1,1,1,1);
		sp3.scale = glm::vec3(0.001f,0.001f,1.0f);
		sp3.position = glm::vec3(-0.2f,-0.1f,-0.79f);
		sp3.rotationMatrix = glm::mat4();
		fg.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/magic/healborder1.png");
		fg.ignoreCamera = true;
		fg.doRender = false;
		fg.amtran = glm::vec4(1,1,1,0);
		fg.scale = glm::vec3(0.35f,0.3f,1.0f);
		fg.position = glm::vec3(0.0f,0.0f,-0.81);
	}

}
