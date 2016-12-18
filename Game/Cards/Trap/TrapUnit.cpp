#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Animation\Camera.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Cursor.h>
#include <Utility\BlankUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Duel\PositionUnit.h>
#include <Utility\SoundUnit.h>
#include <iostream>

#include <Game\Cards\Trap\WidespreadRuin.h>
#include <Game\Cards\Trap\AcidHole.h>
#include <Game\Cards\Trap\TrapHole.h>
#include <Game\Cards\Trap\BadReaction.h>
#include <Game\Cards\Trap\GoblinFan.h>
#include <Game\Cards\Trap\Fake.h>
#include <Game\Cards\Trap\House.h>
#include <Game\Cards\Trap\Reverse.h>
#include <Game\Cards\Trap\MagicJammer.h>
#include <Game\Cards\Trap\BlankTrap.h>

namespace Card{
	TrapUnit TrapUnit::trapUnitInstance;

	void TrapUnit::startup(){
		hasCard = false;
		magicEnd = false;
		trapCard = YUG_NULL_ID;
		cardToCollect = false;
		spareTrap = YUG_NULL_ID;
		for(unsigned int j = 0; j < 2; j++){
			for(unsigned int i = 0; i < 5; i++){
				traps[i][j] = YUG_NULL_ID;;
			}
		}
		currentTrapCol = -1;
	}
	void TrapUnit::cleanup(){
		for(unsigned int j = 0; j < 2; j++){
			for(unsigned int i = 0; i < 5; i++){
				if(traps[i][j] != YUG_NULL_ID){
					delete traps[i][j];
				}
			}
		}
	}

	void TrapUnit::addTrap(int col, bool player){
		int row = (player?0:1);
		if(traps[col][row] != YUG_NULL_ID){
			//std::cout<<"trap: Unempty trap pointer overwritten. local = "<<col<<" "<<row<<std::endl;
			delete traps[col][row];
		}
		traps[col][row] = getTrapPointer();
		traps[col][row]->listen(0);
	}


	TrapSource* TrapUnit::getTrapPointer(){
		TrapSource* t;
		switch(cardNumber){
		case 780: t = new WidespreadRuin(); break;
		case 829: t = new  BadReaction(); break;
		case   830: t = new  Reverse(); break;
		case   831: t = new  Fake(); break;
		case   832: t = new  House(500); break;
		case   833: t = new  House(1000); break;
		case   834: t = new  GoblinFan(); break;
		case   835: t = new  AcidHole(); break;
		case   836: t = new  TrapHole(); break;
		case   837: t = new MagicJammer(); break;
		default: t = new BlankTrap(); break;
		}
		t->listen(0);
		return t;
	}

	void TrapUnit::removeTrap(int col, bool player){
		int row = (player?0:1);
		if(traps[col][row] == YUG_NULL_ID){
			//std::cout<<"trap: No trap to remove at local = "<<col<<" "<<row<<std::endl;
		}else{
			delete traps[col][row];
		}
	}

	void TrapUnit::removeAllTraps(){
		for(unsigned int i = 0; i < 5; i++){
			removeTrap(i,false);
			removeTrap(i,true);
		}
	}

	void TrapUnit::render(){
		if(chain == YUG_TRAP_CH_SPECIFIC){
			currentTrap->render();
		}
	}
	void TrapUnit::update(){
		if(!isWaiting){
			switch(chain){
			case YUG_TRAP_CH_CAM_PULL:
				camPullUpdate();
				break;
			case YUG_TRAP_CH_SPARKLE:
				sparkleUpdate();
				break;
			case YUG_TRAP_CH_CAM_BACK:
				camBackUpdate();
				break;
			case YUG_TRAP_CH_SPECIFIC:
				specificUpdate();
				break;
			case YUG_TRAP_CH_SPECIFIC_END:
				deleteUpdate();
				break;
			case YUG_TRAP_CH_RETURN:
				returnUpdate();
				break;
			case YUG_TRAP_CH_END:
				endUpdate();
				break;
			case YUG_TRAP_CH_MAG_RETURN:
				magicReturnUpdate();
				break;
			case YUG_TRAP_CH_MAG_END:
				magicEndUpdate();
				break;
			default:
				break;
			}
		}else{
			continueWaiting();
		}
	}

	void TrapUnit::start(int info){
		if(!theBoard.playerControlling()){
			endUnit.trapsUsed++;
		}
		currentTrap->startup();
		returnPoint = info;
		switch(info){
		case YUG_TRAP_ATTACK:
			setUpFromBoard();
			break;
		case YUG_TRAP_STANCE_CHANGE:
			setUpFromBoard();
			break;
		case YUG_TRAP_MAGIC_ACTIVATE:
			setUpFromMagic();
			break;
		case YUG_TRAP_MON_PLAYED:
			setUpFromPlay();
			break;
		default:
			setUpFromBoard();
			break;
		}

	}

	void TrapUnit::setUpFromBoard(){
		theBoard.underlay.hide();
		theBoard.chain = YUG_BOARD_CH_IDLE;
		chain = YUG_TRAP_CH_CAM_PULL;
		theBoard.battleCursor.turnoff();
	}
	void TrapUnit::setUpFromMagic(){
		chain = YUG_TRAP_CH_CAM_PULL;
	}
	void TrapUnit::setUpFromPlay(){
		chain = YUG_TRAP_CH_CAM_PULL;
		wait(0.3f);
	}

	void TrapUnit::camPullUpdate(){
		cameraToTrapView(0.3f);
		chain = YUG_TRAP_CH_SPARKLE;
		wait(0.35f);
	}
	void TrapUnit::sparkleUpdate(){
		soundUnit.playOnce("GameData/sounds/magic/trapNoise.wav");
		glm::vec3 c = theBoard.getBoardPosition(cardCol, cardRow);
		particleUnit.trapParticles(c,0.5f,theBoard.playerControlling());
		theBoard.board[cardCol][cardRow].smallRender.interpolateAmtran(
			glm::vec4(1.0f,1.0f,1.0f,0.0f),0.5f);
		chain = YUG_TRAP_CH_CAM_BACK;
		wait(0.55f);
	}
	void TrapUnit::camBackUpdate(){
		theBoard.cameraToPlayer();
		chain = YUG_TRAP_CH_SPECIFIC;
		wait(0.2f);
	}
	void TrapUnit::specificUpdate(){
		currentTrap->update();
	}
	void TrapUnit::deleteUpdate(){
		currentTrap->cleanup();
		delete currentTrap;
		traps[currentSourceCol][currentSourceRow] = YUG_NULL_ID;
		if(!magicEnd)
			chain = YUG_TRAP_CH_RETURN;
		else
			chain = YUG_TRAP_CH_MAG_RETURN;
	}
	void TrapUnit::returnUpdate(){
		theBoard.board[cardCol][cardRow].smallRender.cleanup();
		theBoard.board[cardCol][cardRow].cleanup();
		theBoard.board[cardCol][cardRow] = cardCreator.blankCard();
		theBoard.currentBoardView(0.2f);
		chain = YUG_TRAP_CH_END;
		wait(0.2f);
	}
	void TrapUnit::endUpdate(){
		if(returnPoint==YUG_TRAP_MON_PLAYED){
			positionUnit.chain = YUG_POSITION_CH_PLACING;
			chain = YUG_TRAP_CH_IDLE;
			positionUnit.wait(0.2f);
		}else{
			theBoard.chain = YUG_BOARD_CH_PLAYERTURN;
			theBoard.underlay.viewingBoard();
			chain = YUG_TRAP_CH_IDLE;
			theBoard.enemy.ai->moveList.clear();
			theBoard.enemy.ai->moveIndex = 0;
			if(returnPoint == YUG_TRAP_MAGIC_ACTIVATE){
				theBoard.enemy.ai->moveList.clear();
				theBoard.enemy.ai->moveIndex = 0;
				theBoard.halfView = false;
				puzzleCursor.doRender = false;
				blankUnit.transitionInput(&theBoard, 0.1f);
			}
			theBoard.battleCursor.turnon();
			theBoard.battleCursor.move();
			theBoard.battleCursorReaction();
		}
	}
	void TrapUnit::magicReturnUpdate(){
		theBoard.board[cardCol][cardRow].smallRender.cleanup();
		theBoard.board[cardCol][cardRow].cleanup();
		theBoard.board[cardCol][cardRow] = cardCreator.blankCard();
		chain = YUG_TRAP_CH_MAG_END;
	}
	void TrapUnit::magicEndUpdate(){
		theBoard.enemy.ai->moveList.clear();
		theBoard.enemy.ai->moveIndex = 0;
		magicUnit.chain = YUG_MAG_CH_SPECIFIC;
		chain = YUG_TRAP_CH_IDLE;
		theBoard.cameraToPlayer();
	}

	//utility
	void TrapUnit::cameraToTrapView(float duration){
		if(theBoard.playerControlling()){
			camera.interpolate(pos.trapPCamPos, duration);
			camera.interpolateViewDirection(pos.trapPCamView,duration);
		}else{
			camera.interpolate(pos.trapECamPos, duration);
			camera.interpolateViewDirection(pos.trapECamView,duration);
		}
	}


	bool TrapUnit::listen(int info){
		for(unsigned int i = 0 ; i < 2; i++){
			for(unsigned int j = 0; j < 5; j++){
				if(traps[j][i] != YUG_NULL_ID){
					if(traps[j][i]->listen(info)){
						currentSourceRow = i;
						currentSourceCol = j;
						currentTrap = traps[j][i];
						return true;
					}
				}
			}
		}
		return false;
	}

	bool TrapUnit::enemyListen(int info){
		int i;
		if(theBoard.playerControlling()){
			i = 1; cardRow = YUG_BOARD_ENEMY_MAG_ROW;
		}else{
			i = 0; cardRow = YUG_BOARD_PLAYER_MAG_ROW;
		}
		for(unsigned int j = 0; j < 5; j++){
			if(traps[j][i] != YUG_NULL_ID){
				if(traps[j][i]->listen(info)){
					currentSourceRow = i;
					currentSourceCol = j;
					cardCol = j;
					currentTrap = traps[j][i];
					return true;
				}
			}
		}
		return false;
	}

	void TrapUnit::printOutTrapSources(){
		/*for(unsigned int i = 0 ; i < 2; i++){
			for(unsigned int j = 0; j < 5; j++){
				if(traps[j][i] != YUG_NULL_ID){
					std::cout<<"Trap source at "<<j<<" "<<i<<std::endl;
				}
			}
		}*/
	}

}