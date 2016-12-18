#include <Game\Cards\Trap\MagicJammer.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Cards\Magic\BlankMagic.h>

namespace Card{

	void MagicJammer::startup(){
		trapUnit.magicEnd = true;
	}
	void MagicJammer::cleanup(){}
	void MagicJammer::render(){}
	void MagicJammer::update(){
		magicUnit.magicSource->cleanup();
		delete magicUnit.magicSource;
		magicUnit.magicSource = new BlankMagic();
		trapUnit.chain = YUG_TRAP_CH_SPECIFIC_END;
	}
	bool MagicJammer::listen(int info){
		if(info == YUG_TRAP_MAGIC_ACTIVATE){
			int& m = magicUnit.cardNo;
			if(m ==780 ||( 829  <= m && m <= 837)){
				return false;
			}	
			return true;
		}
		return false;
	}
}

