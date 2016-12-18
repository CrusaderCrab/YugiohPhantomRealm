#include <Game\Cards\Magic\BlankMagic.h>
#include <Game\Cards\Magic\MagicUnit.h>

namespace Card{

	void BlankMagic::startup(){}
	void BlankMagic::cleanup(){}
	void BlankMagic::render(){}
	void BlankMagic::update(){
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}

}