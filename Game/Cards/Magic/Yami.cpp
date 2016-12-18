#include <Game\Cards\Magic\Yami.h>
#include <Game\Duel\Board.h>
#include <Utility\TextureLoader.h>
#include <vector>
namespace Card{

	void Yami::setNewFieldTexture(){
		theBoard.field.currentField = 843;
		textureLoader.deleteTexture(&theBoard.boardModel.textureBO);
		textureLoader.loadTexture(
			"GameData/textures/board/darkBoard.png",
			&theBoard.boardModel.textureBO);
	}
	void Yami::setFieldBoosts(){
		std::vector<int> bsts;
		bsts.push_back(YUG_FIEND);
		bsts.push_back(YUG_SPELLCASTER);
		theBoard.field.newBoosts(bsts);
	}
	void Yami::setFieldWeakens(){
		std::vector<int> wks;
		wks.push_back(YUG_FAIRY);
		theBoard.field.newWeakens(wks);
	}
	void Yami::setFieldAmtran(){
		theBoard.field.currentAmtran = glm::vec4(1,1,1,1);
	}


}
