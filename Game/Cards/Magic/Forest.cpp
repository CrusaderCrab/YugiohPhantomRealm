#include <Game\Cards\Magic\Forest.h>
#include <Game\Duel\Board.h>
#include <Utility\TextureLoader.h>
#include <vector>
namespace Card{

	void Forest::setNewFieldTexture(){
		theBoard.field.currentField = 839;
		textureLoader.deleteTexture(&theBoard.boardModel.textureBO);
		textureLoader.loadTexture(
			"GameData/textures/board/forestBoard.png",
			&theBoard.boardModel.textureBO);
	}
	void Forest::setFieldBoosts(){
		std::vector<int> bsts;
		bsts.push_back(YUG_BEASTWARRIOR);
		bsts.push_back(YUG_PLANT);
		bsts.push_back(YUG_INSECT);
		bsts.push_back(YUG_BEAST);
		theBoard.field.newBoosts(bsts);
	}
	void Forest::setFieldWeakens(){
		std::vector<int> wks;
		theBoard.field.newWeakens(wks);
	}
	void Forest::setFieldAmtran(){
		theBoard.field.currentAmtran = glm::vec4(1,1,1,1);
	}


}