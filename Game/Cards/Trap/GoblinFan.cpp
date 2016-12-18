#include <Game\Cards\Trap\GoblinFan.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Cards\Magic\DamageMagic.h>

namespace Card{

	void GoblinFan::startup(){
		trapUnit.magicEnd = true;
	}
	void GoblinFan::cleanup(){}
	void GoblinFan::render(){}
	void GoblinFan::update(){
		DamageMagic* h = static_cast<DamageMagic*>( magicUnit.magicSource);
		h->reversed = true;
		trapUnit.chain = YUG_TRAP_CH_SPECIFIC_END;

	}
	bool GoblinFan::listen(int info){
		if(info == YUG_TRAP_MAGIC_ACTIVATE){
			int& m = magicUnit.cardNo;
			if(822  <= m && m <= 826){
				return true;
			}
		}
		return false;
	}
}
