#include <Game\Duel\Board.h>
#include <Utility\Clock.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\MovementUnit.h>

#include <iostream>


namespace Duel{

	bool Board::enemyHasNoCards(){
		if(playerControlling()){
			for(int i = 0; i< 5; i++){
				if(!board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard())
					return false;
			}
			return true;
		}else{
			for(int i = 0; i< 5; i++){
				if(!board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard())
					return false;
			}
			return true;
		}
	}

	bool Board::currentCardGood(){
		return (!board[cP[0]][cP[1]].blankCard());
	}
	bool Board::currentAtkCardGood(){
		return (!board[aP[0]][aP[1]].blankCard());
	}

	Card::CardData* Board::getCurrentCard(){
		return &(board[cP[0]][cP[1]]);
	}

	Card::CardData* Board::getCurrentAtkCard(){
		return &(board[aP[0]][aP[1]]);
	}

	void Board::flipCardIfNecessary(){
		if(!currentCard.faceUp){
			flipCard(cP[0],cP[1]);
		}
	}
	void Board::flipAttackerIfNecessary(){
		if(!board[aP[0]][aP[1]].faceUp){
			flipCard(aP[0],aP[1]);
		}
	}

	bool Board::overMonsterCard(){
		return(board[cP[0]][cP[1]].monMagTrap == YUG_MONSTER_CARD);
	}
	bool Board::onPlayerSide(){
		return (cP[1] == YUG_BOARD_PLAYER_MON_ROW ||
			cP[1] == YUG_BOARD_PLAYER_MAG_ROW);
	}

	bool Board::overPlayerCard(){
		if(playerControlling() && (onPlayerSide()))
			return true;
		if(!playerControlling() && !(onPlayerSide()))
			return true;
		return false;
	}

	bool Board::playerControlling(){
		return (currentPlayer == &player);
	}

	void Board::wait(float duration){
		//if(isWaiting)
		//	std::cout<<"Game Player: one wait time overriding another\n";
		waitDuration = duration;
		currentlyWaited = 0;
		isWaiting = true;

	}
	void Board::continueWaiting(){
		currentlyWaited += gameClock.lastLoopTime();
		if(currentlyWaited > waitDuration){
			isWaiting = false;
		}
	}

	glm::vec3 Board::getBoardPosition(int col, int row){
		float y = pos.bPlayerCardPosOrigin.y;
		float x = pos.bPlayerCardPosOrigin.x +
					pos.bPlayerCardPosOffset.x*col;
		float z = pos.bPlayerCardPosOrigin.z;
		if(row == YUG_BOARD_PLAYER_MON_ROW)
			z += pos.bPlayerCardPosOffset.z;
		if(row == YUG_BOARD_ENEMY_MAG_ROW)
			z = pos.bEnemyCardPosOrigin.z +
					pos.bEnemyCardPosOffset.z;
		if(row == YUG_BOARD_ENEMY_MON_ROW)
			z = pos.bEnemyCardPosOrigin.z;
		return glm::vec3(x,y,z);

	}

	void Board::playerTurnStartPos(){
		if(playerControlling()){
			cP[0] = 0;
			cP[1] = YUG_BOARD_PLAYER_MON_ROW;
		}else{
			cP[0] = 4;
			cP[1] = YUG_BOARD_ENEMY_MON_ROW;
		}

	}
	void Board::pullCardToBoard(int col, int row){
		board[col][row].smallRender.interpolate(getBoardPosition(col,row), pos.wait[2]);
		board[col][row].smallRender.rotate(getCorrectCardRotation(col,row), pos.wait[2]);
	}
	void Board::pullCardToBoard(int col, int row, float duration){
		board[col][row].smallRender.interpolate(getBoardPosition(col,row), duration);
		board[col][row].smallRender.rotate(getCorrectCardRotation(col,row), duration);
	}
	glm::mat4 Board::getCorrectCardRotation(int col, int row){
		Card::CardData* c = &board[col][row];
		if(playerControlling()){
			if(c->faceUp){
				if(c->attackMode)
					return pos.atkFaceupFlat;
				else
					return pos.defFaceupFlat;
			}else{
				if(c->attackMode)
					return pos.atkFacedownFlat;
				else
					return pos.defFacedownFlat;
			}
		}else{
			if(c->faceUp){
				if(c->attackMode)
					return pos.eAtkFaceupFlat;
				else
					return pos.eDefFaceupFlat;
			}else{
				if(c->attackMode)
					return pos.eAtkFacedownFlat;
				else
					return pos.eDefFacedownFlat;
			}
		}
	}

	bool Board::cardVisibleToPlayer(){
		return (onPlayerSide() || board[cP[0]][cP[1]].faceUp);

	}

	void Board::updateGuardWarnings(){
		if(chain == YUG_BOARD_CH_CHOOSEENEMY && !board[cP[0]][cP[1]].blankCard()){
			if(board[aP[0]][aP[1]].versusConstellation(currentCard.currentConstellation) ==
				YUG_WEAK_AGAINST){
				underlay.badGuard();
			}else if(board[aP[0]][aP[1]].versusConstellation(currentCard.currentConstellation) ==
				YUG_STRONG_AGAINST){
				underlay.goodGuard();
			}else{
				underlay.noGuard();
			}
		}else{
				underlay.noGuard();
		}

	}

	int Board::getFirstEmptyColumn(int row){
		if(playerControlling()){
			for(int i = 0; i < 5; i++){
				if(board[i][row].blankCard()){
					return i;
				}
			}
			return 0;
		}else{
			for(int i = 4; i > -1; i--){
				if(board[i][row].blankCard()){
					return i;
				}
			}
			return 4;
		}
	}

	void Board::interpolateBoardAndCardAmtran(glm::vec4 newAmtran, float duration){
		for(unsigned int i = 0; i < 5; i++){
			for(unsigned int j = 0; j < 4; j++){
				if(!board[i][j].blankCard()){
					board[i][j].smallRender.interpolateAmtran(newAmtran,duration);
				}
			}
		}
		boardModel.interpolateAmtran(newAmtran,duration);
		battleCursor.interpolateAmtran(newAmtran,duration);
	}

	bool Board::isMagicCard(Card::CardData* c){
		return c->monMagTrap == YUG_MAGIC_CARD;
	}

	void Board::rotateCardToBoard(int col, int row){
		board[col][row].smallRender.rotate(getCorrectCardRotation(col,row),0.2f);
	}
	void Board::rotateCardToBoard(int col, int row, float duration){
		board[col][row].smallRender.rotate(getCorrectCardRotation(col,row),duration);
	}

}