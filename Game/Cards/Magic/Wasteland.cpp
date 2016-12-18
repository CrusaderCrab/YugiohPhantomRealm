#include <Game\Cards\Magic\Wasteland.h>
#include <Game\Duel\Board.h>
#include <Utility\TextureLoader.h>
#include <vector>
namespace Card{

	void Wasteland::setNewFieldTexture(){
		theBoard.field.currentField = 840;
		textureLoader.deleteTexture(&theBoard.boardModel.textureBO);
		textureLoader.loadTexture(
			"GameData/textures/board/desertBoard.png",
			&theBoard.boardModel.textureBO);
	}
	void Wasteland::setFieldBoosts(){
		std::vector<int> bsts;
		bsts.push_back(YUG_ZOMBIE);
		bsts.push_back(YUG_DINOSAUR);
		bsts.push_back(YUG_ROCK);
		bsts.push_back(YUG_MACHINE);
		theBoard.field.newBoosts(bsts);
	}
	void Wasteland::setFieldWeakens(){
		std::vector<int> wks;
		theBoard.field.newWeakens(wks);
	}
	void Wasteland::setFieldAmtran(){
		theBoard.field.currentAmtran = glm::vec4(1,1,1,1);
	}


}
