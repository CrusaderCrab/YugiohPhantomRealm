#include <Game\Duel\Board.h>
#include <Game\Duel\BattleUnit.h>
#include <Game\Duel\PositionUnit.h>
#include <Game\Cards\CardDisplayUnit.h>
#include <Game\Animation\Camera.h>
#include <Game\VectorUnit.h>
#include <Utility\BlankUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Duel\Parts\SwordUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Game\Cursor.h>
#include <Utility\SoundUnit.h>
#include <iostream>


namespace Duel{

		void Board::render(){
		if(doRender){
			boardModel.render();
			renderPlayedCards();
			swordUnit.render();
			renderUnderlay();
			battleCursor.render();
			renderFusionUnit();
			renderBattleUnit();
			renderEffectCard();
			player.render();
			enemy.render();
			battleUnit.render();
			if(magicBeforeParticles){
				magicUnit.render();
				if(trapBeforeParticles){
					trapUnit.render();
					particleUnit.render();
				}else{
					particleUnit.render();
					trapUnit.render();
				}
			}else{
				if(trapBeforeParticles){
					trapUnit.render();
					particleUnit.render();
				}else{
					particleUnit.render();
					trapUnit.render();
				}
				magicUnit.render();
			}
			endUnit.render();
			renderField();
			renderLifepoints();

			//cardDisplayUnit.render();
			positionUnit.render();
		}
	}

	void Board::renderUnderlay(){
		underlay.render();
	}
	void Board::renderPlayedCards(){
		for(int col = 0; col < YUG_BOARD_CARDS_PER_ROW; col++){
			for(int row = 0; row < YUG_BOARD_CARDS_PER_COL; row++){
				if(board[col][row].cardNumber != YUG_NO_CARD){
					board[col][row].smallRender.render();
				}
			}
		}
	}
	void Board::renderField(){
		field.render();
	}
	void Board::renderLifepoints(){
		LP.render();
	}

	void Board::renderFusionUnit(){
		fusionUnit.render();
	}
	void Board::renderBattleUnit(){

	}
	void Board::renderEffectCard(){

	}

	void Board::currentPlayerView(float speed){
		if(playerControlling()){
			camera.interpolate(pos.pHandCamOrigin, speed);
			camera.interpolateViewDirection(pos.pHandCamView,
				speed);
		}else{
			camera.interpolate(pos.eHandCamOrigin, speed);
			camera.interpolateViewDirection(pos.eHandCamView,
				speed);
		}

	}
	void Board::currentBoardView(float speed){
		if(playerControlling()){
			camera.interpolate(calcCameraPos(), speed);
			camera.interpolateViewDirection(pos.bPlayerCamViewDirection,
				speed);
		}else{
			camera.interpolate(calcCameraPos(), speed);
			camera.interpolateViewDirection(pos.bEnemyCamViewDirection,
				speed);
		}
	}
	glm::vec3 Board::calcCameraPos(){
		if(playerControlling()){
			if(cP[1] == YUG_BOARD_PLAYER_MON_ROW){
				return mov.addZOffset(pos.bPlayerCamPosOrigin, pos.bPlayerCamPosPMonZ);
			}
			if(cP[1] == YUG_BOARD_PLAYER_MAG_ROW){
				if( halfView )
					return mov.addZOffset(pos.bPlayerCamPosOrigin, pos.bPlayerCamPosPMagZ);
				else
					return mov.addZOffset(pos.bPlayerCamPosOrigin, pos.bPlayerCamPosPMonZ);
			}
			if(cP[1] == YUG_BOARD_ENEMY_MAG_ROW){
				if( halfView )
					return mov.addZOffset(pos.bPlayerCamPosOrigin, pos.bPlayerCamPosEMagZ);
				else
					return mov.addZOffset(pos.bPlayerCamPosOrigin, pos.bPlayerCamPosEMonZ);
			}
			return mov.addZOffset(pos.bPlayerCamPosOrigin, pos.bPlayerCamPosEMonZ);
		}else{//computer
			if(cP[1] == YUG_BOARD_PLAYER_MON_ROW){
				return mov.addZOffset(pos.bEnemyCamPosOrigin, pos.bEnemyCamPosPMonZ);
			}
			if(cP[1] == YUG_BOARD_PLAYER_MAG_ROW){
				if( halfView )
					return mov.addZOffset(pos.bEnemyCamPosOrigin, pos.bEnemyCamPosPMagZ);
				else
					return mov.addZOffset(pos.bEnemyCamPosOrigin, pos.bEnemyCamPosPMonZ);
			}
			if(cP[1] == YUG_BOARD_ENEMY_MAG_ROW){
				if( halfView )
					return mov.addZOffset(pos.bEnemyCamPosOrigin, pos.bEnemyCamPosEMagZ);
				else
					return mov.addZOffset(pos.bEnemyCamPosOrigin, pos.bEnemyCamPosEMonZ);
			}
			return mov.addZOffset(pos.bEnemyCamPosOrigin, pos.bEnemyCamPosEMonZ);
		}
	}

	void Board::moveCamera(){
		camera.interpolate(calcCameraPos(), pos.wait[2]);

	}

	void Board::moveHandCardsIfNeccessary(){
		if(!currentPlayer->handView){
			currentPlayer->moveCardsToViewPosition();

		}

	}

	void Board::battleCursorReaction(){
		if(playerControlling() 
			&& overPlayerCard() 
			&& getCurrentCard()->monMagTrap == YUG_MONSTER_CARD 
			&& !getCurrentCard()->hasAttacked
			&& chain == YUG_BOARD_CH_PLAYERTURN){
				battleCursor.emptyMode = false;
		}else{
			battleCursor.emptyMode = true;
		}

	}

	//camera turnover
	void Board::turnover1Update(){
		underlay.hide();
		float aniTime = 1.5f;
		if(playerControlling()){
			camera.interpolate(pos.bToEmidPosition, aniTime);
		}else{
			camera.interpolate(pos.bToPmidPosition, aniTime);
		}
		wait(aniTime-1.2f);
		chain = YUG_BOARD_CH_TURNOVER1HALF;
	}
	void Board::turnover1HalfUpdate(){
		float aniTime = 1.2f;
		if(playerControlling()){
			
			camera.interpolateViewDirection(pos.bToELookDownView, aniTime);
		}else{
			camera.interpolateViewDirection(pos.bToPLookDownView, aniTime);
		}
		wait(aniTime);
		chain = YUG_BOARD_CH_TURNOVER2;
	}
	
	void Board::turnover2Update(){
		float aniTime = 1.5f;
		if(playerControlling()){
			currentPlayer = &enemy;
			currentPlayerView(aniTime);
		}else{
			currentPlayer = &player;
			currentPlayerView(aniTime);
		}
		prepareCardsForNewTurn();
		currentPlayer->moveCardsToHiddenPosition();
		lockRowMovement = false;
		battleCursor.newTurnRotation();
		wait(aniTime);
		chain = YUG_BOARD_CH_TURNOVER3;
	}
	void Board::turnover3Update(){
		chain = YUG_BOARD_CH_IDLE;
		if(!playerControlling()){
			blankUnit.transitionInput(&(enemy),0.2f);
			player.currentChainPosition = YUG_GAME_PLAYER_CHAIN_IDLE;
			underlay.viewingHand();
			enemy.newTurn();
		}else{
			blankUnit.transitionInput(&(player),0.2f);
			enemy.currentChainPosition = YUG_GAME_PLAYER_CHAIN_IDLE;
			underlay.viewingHand();
			player.newTurn();
		}
	}

	void Board::prepareCardsForNewTurn(){
		for(unsigned int i = 0; i < 5; i++){
			for(unsigned int j = 0; j < 4; j++){
				if(!board[i][j].blankCard()){
					//std::cout<<"Board ani: c = "<<i<<" r = "<<j<<std::endl;
					//std::cout<<"Board ani: rotate card: "<<board[i][j].name.data()<<std::endl;
					board[i][j].smallRender.rotationMatrix = theBoard.getCorrectCardRotation(i,j);
					board[i][j].smallRender.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
					board[i][j].hasAttacked = false;
				}
			}
		}
	}

	glm::vec4 Board::getCurrentOverallAmtran(){
		return glm::vec4(1.0f,1.0f,1.0f,1.0f);
	}
	glm::vec4 Board::getCurrentOverallDarkenedAmtran(){
		glm::vec4 v4 = getCurrentOverallAmtran()*0.5f;
		v4.w = 1.0f;
		return v4;
	}

	void Board::panInUpdate(){
		//std::cout<<"Board::panInUpdate start\n";
		boardModel.interpolateAmtran(glm::vec4(1,1,1,1), 1.0f);
		boardModel.rotate(glm::mat4(), 5.0f);
		currentPlayerView(5.0f);
		fadeUnit.fadeTo(glm::vec4(0.001f,0,0,0), 1.0f);
		wait(5.5f);
		chain = YUG_BOARD_CH_STARTNORM;

		LP.displayedEnemyLP = 0;
		LP.newEnemyLP(8000, pos.wait[9]);
		LP.displayedPlayerLP = 0;
		LP.newPlayerLP(8000, pos.wait[9]);
		//std::cout<<"Board::panInUpdate end\n";
	}

	void Board::prepareBoardModel(){
		boardModel.amtran = glm::vec4(1,1,1,0);
		boardModel.rotationMatrix = pos.atkFacedownUp;
		glm::vec3 cPos = pos.pHandCamOrigin;
		cPos.z += 10.0f;
		camera.position = cPos;
		camera.viewDirection = glm::vec3(0,0,-1);
		puzzleCursor.doRender = false;
		glm::vec3 curPos = pos.pHandCursorPosOrigin;
		curPos.x += 0.5f;
		puzzleCursor.cursorModel.position = curPos;
	}

	void Board::panFinishUpdate(){
		//std::cout<<"Board::prepareBoardUpdate start\n";
		chain = YUG_BOARD_CH_IDLE;
		puzzleCursor.doRender = true;
		soundUnit.playLeadinTrack(leadInTrack.c_str(), mainTrack.c_str(), leadInLength);
		player.newTurn();
		//std::cout<<"Board::prepareBoardUpdate end\n";
	}

}