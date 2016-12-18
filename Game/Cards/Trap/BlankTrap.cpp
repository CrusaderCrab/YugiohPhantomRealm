#include <Game\Cards\Trap\BlankTrap.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>

namespace Card{

	void BlankTrap::startup(){
		trapUnit.magicEnd = false;
	}
	void BlankTrap::cleanup(){}
	void BlankTrap::render(){}
	void BlankTrap::update(){
		trapUnit.chain = YUG_TRAP_CH_SPECIFIC_END;
	}
	bool BlankTrap::listen(int info){
		return false;
	}
}
