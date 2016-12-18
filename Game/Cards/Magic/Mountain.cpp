#include <Game\Cards\Magic\Mountain.h>
#include <Game\Duel\Board.h>
#include <Utility\TextureLoader.h>
#include <vector>
namespace Card{

	void Mountain::setNewFieldTexture(){
		theBoard.field.currentField = 841;
		textureLoader.deleteTexture(&theBoard.boardModel.textureBO);
		textureLoader.loadTexture(
			"GameData/textures/board/mountainBoard.png",
			&theBoard.boardModel.textureBO);
	}
	void Mountain::setFieldBoosts(){
		std::vector<int> bsts;
		bsts.push_back(YUG_DRAGON);
		bsts.push_back(YUG_WINGEDBEAST);
		bsts.push_back(YUG_THUNDER);
		bsts.push_back(YUG_FAIRY);
		theBoard.field.newBoosts(bsts);
	}
	void Mountain::setFieldWeakens(){
		std::vector<int> wks;
		theBoard.field.newWeakens(wks);
	}
	void Mountain::setFieldAmtran(){
		theBoard.field.currentAmtran = glm::vec4(1,1,1,1);
	}


}
