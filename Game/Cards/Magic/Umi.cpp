#include <Game\Cards\Magic\Umi.h>
#include <Game\Duel\Board.h>
#include <Utility\TextureLoader.h>
#include <vector>
namespace Card{

	void Umi::setNewFieldTexture(){
		theBoard.field.currentField = 842;
		textureLoader.deleteTexture(&theBoard.boardModel.textureBO);
		textureLoader.loadTexture(
			"GameData/textures/board/oceanBoard.png",
			&theBoard.boardModel.textureBO);
	}
	void Umi::setFieldBoosts(){
		std::vector<int> bsts;
		bsts.push_back(YUG_FISH);
		bsts.push_back(YUG_AQUA);
		bsts.push_back(YUG_SEASERPENT);
		bsts.push_back(YUG_THUNDER);
		theBoard.field.newBoosts(bsts);
	}
	void Umi::setFieldWeakens(){
		std::vector<int> wks;
		wks.push_back(YUG_PYRO);
		wks.push_back(YUG_MACHINE);
		theBoard.field.newWeakens(wks);
	}
	void Umi::setFieldAmtran(){
		theBoard.field.currentAmtran = glm::vec4(1,1,1,1);
	}


}