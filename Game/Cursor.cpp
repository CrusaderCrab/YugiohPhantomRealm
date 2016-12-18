#include <Game\Cursor.h>
#include <Game\Duel\Board.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\VectorUnit.h>

namespace Game{

	Cursor Game::Cursor::cursorInstance;

	bool Cursor::initialize(){
		cursorModel.startup("GameData/models/MyFirstPuzzle01.obj",
			"gameData/textures/board/cursorUV.png");
		cursorModel.position = glm::vec3(0.2f, -0.175f, -0.88f);
		cursorModel.ignoreCamera = true;
		cursorModel.scale = glm::vec3(0.025f,0.025f,0.025f);
		rotate180 = glm::mat4(1,0,0,0, 0,-1,0,0, 0,0,-1,0, 0,0,0,1);
		first180 = false;
		doRender = false;
		return true;
	}

	bool Cursor::shutdown(){
		cursorModel.cleanup();
		return true;
	}

	void Cursor::render(){
		if(doRender)
			cursorModel.render();
	}
	void Cursor::update(){
		cursorModel.update();
		if(!cursorModel.isRotating){
			if(first180){
				cursorModel.rotate(rotate180, 1.0f);
				first180 = false;
			}else{
				cursorModel.rotate(glm::mat4(), 1.0f);
				first180 = true;
			}
		}
	}

	void Cursor::interpolateToHandPos(){
		theBoard.currentPlayer;
			cursorModel.interpolate(
				mov.addXOffset(
					theBoard.currentPlayer->cursorFirstPosition,
					theBoard.currentPlayer->cursorOffset.x *
					theBoard.currentPlayer->cursorPosition),
				pos.wait[2]
			);

	}

}