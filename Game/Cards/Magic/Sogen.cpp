#include <Game\Cards\Magic\Sogen.h>
#include <Game\Duel\Board.h>
#include <Utility\TextureLoader.h>
#include <vector>
namespace Card{

	void Sogen::setNewFieldTexture(){
		theBoard.field.currentField = 338;
		textureLoader.deleteTexture(&theBoard.boardModel.textureBO);
		textureLoader.loadTexture(
			"GameData/textures/board/sogenBoard.png",
			&theBoard.boardModel.textureBO);
	}
	void Sogen::setFieldBoosts(){
		std::vector<int> bsts;
		bsts.push_back(YUG_BEASTWARRIOR);
		bsts.push_back(YUG_WARRIOR);
		theBoard.field.newBoosts(bsts);
	}
	void Sogen::setFieldWeakens(){
		std::vector<int> wks;
		theBoard.field.newWeakens(wks);
	}
	void Sogen::setFieldAmtran(){
		theBoard.field.currentAmtran = glm::vec4(1,1,1,1);
	}


}