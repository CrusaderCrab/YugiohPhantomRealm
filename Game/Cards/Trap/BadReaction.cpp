#include <Game\Cards\Trap\BadReaction.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Cards\Magic\HealMagic.h>

namespace Card{

	void BadReaction::startup(){
		trapUnit.magicEnd = true;
	}
	void BadReaction::cleanup(){}
	void BadReaction::render(){}
	void BadReaction::update(){
		HealMagic* h = static_cast<HealMagic*>( magicUnit.magicSource);
		h->reversed = true;
		trapUnit.chain = YUG_TRAP_CH_SPECIFIC_END;

	}
	bool BadReaction::listen(int info){
		if(info == YUG_TRAP_MAGIC_ACTIVATE){
			int& m = magicUnit.cardNo;
			if(817  <= m && m <= 821){
				return true;
			}
		}
		return false;
	}



}
