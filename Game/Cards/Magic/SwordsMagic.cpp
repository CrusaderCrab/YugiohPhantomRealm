#include <Game\Cards\Magic\SwordsMagic.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Duel\Parts\SwordUnit.h>

namespace Card{

	void SwordsMagic::startup(){
		Game::WaitUnit::startup();
		returnNow = false;
	}
	void SwordsMagic::cleanup(){}
	void SwordsMagic::render(){}
	void SwordsMagic::update(){
		if(isWaiting){
			continueWaiting();
		}else{
			if(returnNow){
				magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
			}else{
				swordUnit.activateSwords();
				returnNow = true;
				wait(1.5f);
			}
		}
	}
}
