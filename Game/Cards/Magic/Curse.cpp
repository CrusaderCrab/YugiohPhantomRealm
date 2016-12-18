#include <Game\Cards\Magic\Curse.h>
#include <Utility\Clock.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Duel\Board.h>
#include <Utility\SoundUnit.h>

#define ZYUG_GO 0
#define ZYUG_PART 1
#define ZYUG_FADE_IN 2
#define ZYUG_FADE_OUT 3
#define ZYUG_FIN 5


namespace Card{

	void Curse::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_GO;
		getCards();
		time = 0; speed = 1;
		risen = false;
	}

	void Curse::cleanup(){}
	void Curse::render(){}

	void Curse::update(){
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

	void Curse::startUpdate(){
		wait(0.2f);
		if(cards.size()==0)
			chain = ZYUG_FIN;
		chain = ZYUG_FADE_IN;
	}
	void Curse::fadeInUpdate(){
		soundUnit.playOnce("GameData/sounds/magic/goodTextSound2.wav");
		for(unsigned int i = 0;i<cards.size(); i++){
			cards[i]->smallRender.interpolateAmtran(glm::vec4(0.5f,10.0f,0.5f,1.0f),1.0f);
		}
		wait(0.3f);
		chain = ZYUG_PART;
	}
	void Curse::particleUpdate(){
		std::vector<glm::vec3>vs;
		for(unsigned int i = 0;i<cards.size(); i++){
			vs.push_back(cards[i]->smallRender.position);
		}
		particleUnit.particleRise(vs,glm::vec4(0.5f,1.0f,0.5f,1.0f), 1.4f, theBoard.playerControlling());
		wait(0.7f);
		chain = ZYUG_FADE_OUT;
	}
	void Curse::fadeOutUpdate(){
		for(unsigned int i = 0;i<cards.size(); i++){
			cards[i]->smallRender.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f),1.0f);
			cards[i]->alterAttack(cards[i]->atkStatDrop);
			cards[i]->alterDefense(cards[i]->defStatDrop);
			cards[i]->atkStatDrop = 0;
			cards[i]->defStatDrop = 0;
		}
		wait(1.0f);
		chain = ZYUG_FIN;
	}
	void Curse::finishUpdate(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}


	void Curse::getCards(){
		int j = theBoard.playerControlling()?YUG_BOARD_PLAYER_MON_ROW:YUG_BOARD_ENEMY_MON_ROW;
		for(unsigned int i = 0; i < 5; i++){
			if(theBoard.board[i][j].atkStatDrop>0||theBoard.board[i][j].defStatDrop>0){
				cards.push_back(&theBoard.board[i][j]);
			}
		}
	}


}