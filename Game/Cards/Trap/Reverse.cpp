#include <Game\Cards\Trap\Reverse.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\Clock.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Utility\SoundUnit.h>
#include <iostream>

#define ZYUG_GO 0
#define ZYUG_PART 1
#define ZYUG_FADE_IN 2
#define ZYUG_FADE_OUT 3
#define ZYUG_FIN 5

namespace Card{
void Reverse::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		trapUnit.magicEnd = false;
		//time = 0; speed = 1;
		risen = false;
	}
	void Reverse::cleanup(){}
	void Reverse::render(){}

	void Reverse::update(){
		if(!isWaiting){
			switch(chain){
			case ZYUG_GO:
				startUpdate();
				break;
			case ZYUG_FADE_IN:
				fadeInUpdate();
				break;
			case ZYUG_PART:
				particleUpdate();
				break;
			case ZYUG_FADE_OUT:
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

	void Reverse::startUpdate(){
		wait(0.2f);
		chain = ZYUG_FADE_IN;
	}
	void Reverse::fadeInUpdate(){
		soundUnit.playOnce("GameData/sounds/magic/reverseNoise.wav");
		theBoard.board[theBoard.cP[0]][theBoard.cP[1]].smallRender.interpolateAmtran(glm::vec4(10.0f,0.5f,0.5f,1.0f),1.0f);
		wait(0.3f);
		chain = ZYUG_PART;
	}
	void Reverse::particleUpdate(){
		std::vector<glm::vec3>vs;
		vs.push_back(theBoard.board[theBoard.cP[0]][theBoard.cP[1]].smallRender.position);
		particleUnit.particleRise(vs,glm::vec4(1.0f,0.5f,0.5f,1.0f), 1.4f, theBoard.playerControlling());
		wait(0.7f);
		chain = ZYUG_FADE_OUT;
	}
	void Reverse::fadeOutUpdate(){
		alterCardStats();
		theBoard.board[theBoard.cP[0]][theBoard.cP[1]].smallRender.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f),1.0f);
		wait(1.0f);
		chain = ZYUG_FIN;
	}
	void Reverse::finishUpdate(){
		trapUnit.chain = YUG_TRAP_CH_SPECIFIC_END;
		trapUnit.wait(0.3f);
	}

	void Reverse::alterCardStats(){
		CardData& c = theBoard.board[theBoard.cP[0]][theBoard.cP[1]];
		if(c.attack >= c.atkStatBoost){
			c.atkStatDrop += c.atkStatBoost;
		}else{
			c.atkStatDrop += c.attack;
		}
		if(c.defense >= c.defStatBoost){
			c.defStatDrop += c.defStatBoost;
		}else{
			c.defStatDrop += c.defense;
		}
		c.alterAttack(-(c.atkStatBoost*2));
		c.alterDefense(-(c.defStatBoost*2));

	}
	bool Reverse::listen(int info){
		//std::cout<<"reverse listen called\n";
		//std::cout<<"atk stat boost = "<<theBoard.board[theBoard.cP[0]][theBoard.cP[1]].atkStatBoost<<std::endl;
		if(info == YUG_TRAP_MON_PLAYED){
			return(	theBoard.board[theBoard.cP[0]][theBoard.cP[1]].atkStatBoost > 0 ||
					theBoard.board[theBoard.cP[0]][theBoard.cP[1]].defStatBoost > 0);
		}
		return false;
	}

}