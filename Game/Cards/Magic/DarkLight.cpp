#include <Game\Cards\Magic\DarkLight.h>
#include <Utility\Clock.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Animation\FadeUnit.h>
#include <Utility\SoundUnit.h>

#define ZYUG_GO 0
#define ZYUG_GLOW 1
#define ZYUG_FLASH 2
#define ZYUG_FADE 3
#define ZYUG_FIN 4

namespace Card{

	void DarkLight::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		theBoard.magicBeforeParticles = true;
		centreGlow.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/particles/glowingorb.png");
		centreGlow.ignoreCamera = true;
		centreGlow.position = glm::vec3(0.0f,0.0f,-0.8f);
		centreGlow.amtran = glm::vec4(1,1,1,0);
		centreGlow.scale = glm::vec3(0,0,1);
		centreGlow.doRender = false;
	}

	void DarkLight::cleanup(){
		centreGlow.cleanup();
		theBoard.magicBeforeParticles = false;
	}

	void DarkLight::render(){
		centreGlow.render();
	}

	void DarkLight::update(){
		centreGlow.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_GLOW:
				glowUpdate();
				break;
			case ZYUG_FLASH:
				flashUpdate();
				break;
			case ZYUG_FADE:
				fadeUpdate();
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

	void DarkLight::startUpdate(){
		soundUnit.playOnce("GameData/sounds/magic/darkLightNoise.wav");
		centreGlow.doRender = true;
		centreGlow.interpolateAmtran(glm::vec4(1,1,1,1),0.75f);
		centreGlow.scaleInterpolate(glm::vec3(0.2f,0.2f,1.0f),1.0f);
		wait(0.9f);
		chain = ZYUG_FLASH;
	}
	void DarkLight::glowUpdate(){
		wait(0.1f);
		chain = ZYUG_FLASH;
	}
	void DarkLight::flashUpdate(){
		fadeUnit.sheet.amtran = glm::vec4(1,1,1,0);
		fadeUnit.fadeTo(glm::vec4(1,1,1,1),0.05f);
		for(unsigned int i = 0; i<5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].faceUp){
				theBoard.flipCard(i,YUG_BOARD_PLAYER_MON_ROW);
			}
			if(!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].faceUp){
				theBoard.flipCard(i,YUG_BOARD_ENEMY_MON_ROW);
			}
		}
		chain = ZYUG_FADE;
		wait(0.05f);
	}
	void DarkLight::fadeUpdate(){
		particleUnit.ignoreCamera = true;
		particleUnit.doRender = true;
		glm::vec3 v = centreGlow.position;
		v.z = -1.1f;
		particleUnit.particleBang(v,0.2f);
		centreGlow.doRender = false;
		fadeUnit.fadeTo(glm::vec4(1,1,1,0),0.2f);
		wait(0.3f);
		chain = ZYUG_FIN;
	}
	void DarkLight::finishUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}
	





}
