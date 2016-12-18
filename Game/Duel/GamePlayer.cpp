#include <Game\Duel\GamePlayer.h>
#include <Game\PlayerData.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Animation\Camera.h>
#include <Utility\BlankUnit.h>
#include <Utility\Clock.h>
#include <Game\Duel\Board.h>
#include <Game\Cursor.h>
#include <Utility\InputUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Duel\FusionUnit.h>
#include <Utility\StateUnit.h>
#include <Game\Cards\CardDisplayUnit.h>
#include <Game\Duel\PositionUnit.h>
#include <Game\EnemyData.h>
#include <gtc\matrix_transform.hpp>
#include <iostream>
#include <Game\VectorUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Utility\SoundUnit.h>

#include <Game\Animation\ParticlesUnit.h>

namespace Duel{

	bool GamePlayer::startup(int info){
		currentChainPosition = YUG_GAME_PLAYER_CHAIN_IDLE;
		lifepoints = 8000;
		isWaiting = false;
		if(info == YUG_GP_HUMAN){
			human = true;
			//std::cout<<"Game Player start up\n";
			firstCardPosition = pos.pHandCardPosOrigin	;//glm::vec3(-0.97f, -0.30f, 1.7f);
			cardOffset = pos.pHandCardPosOffset;
			cardOffScreenPosition = pos.pHandCardOffPos;
			cursorFirstPosition = pos.pHandCursorPosOrigin;
			cursorOffset = pos.pHandCursorPosOffset;

			markingOffset = pos.pHandMarkingOffset;
			markingScale = pos.pHandMarkingScale;

			markingBoardOffset		=  pos.pHandBoardMarkOffset;
			firstCardBoardPosition	=  pos.pHandBoardOrigin	;
			cardBoardOffset			=  pos.pHandBoardOffset	;	
			cardBoardJumpOffset		=  pos.pHandBoardJump	;

			deck = playerData.deck;
		}else{
			human = false;
			//std::cout<<"enemy startup\n";
			firstCardPosition  = pos.eHandCardPosOrigin;
			cardOffset = pos.eHandCardPosOffset;
			cardOffScreenPosition = pos.eHandCardOffPos;
			cursorFirstPosition = pos.eHandCursorPosOrigin;
			cursorOffset = pos.eHandCursorPosOffset;

			markingOffset = pos.eHandMarkingOffset;
			markingScale = pos.eHandMarkingScale;

			markingBoardOffset		=   pos.eHandBoardMarkOffset;
			firstCardBoardPosition	=   pos.eHandBoardOrigin	;
			cardBoardOffset			=   pos.eHandBoardOffset	;	
			cardBoardJumpOffset		=   pos.eHandBoardJump	;

			deck = enemyData.deck;
		}
		deck.cardsDrawn = 0;
		deck.cardsLeft = 40;
		//std::cout<<"GamePlayer: Deck took from save: \n";
		deck.shuffleDeck();
		//std::cout<<"Player: filling blank hand\n";
		cursorPosition = 0;
		hand.clear();
		fusionMarks.clear();
		for(unsigned int i = 0; i < 5; i++){
			hand.push_back(cardCreator.blankCard());
			FusionMarkings f;
			fusionMarks.push_back(f);
			fusionMarks[i].upDown = YUG_MARKINGS_MIDDLE;
			fusionMarks[i].position = 0;
		}
		return true;
	}
		
	bool GamePlayer::cleanup(){
		return true;
	}

	void GamePlayer::render(){
		for(int i = 0; i < 5; i++){
			if(!hand[i].blankCard()){
				hand[i].smallRender.render();
				if(fusionMarks[i].upDown != YUG_MARKINGS_MIDDLE){
					drawMarkingOn(i, fusionMarks[i].position);
				}
			}
		}
	}

	void GamePlayer::input(){
		if(!isWaiting && currentChainPosition == YUG_GAME_PLAYER_CHAIN_TAKING_INPUTS)
		{
			if(inputUnit.isKeyActive(YUG_KEY_LEFT)){
				leftKeyPress();
				wait(YUG_WAIT_INPUT_WAIT);
			}
			else if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){
				rightKeyPress();
				wait(YUG_WAIT_INPUT_WAIT);
			}
			else if(inputUnit.isKeyActive(YUG_KEY_UP)){
				upKeyPress();
				wait(YUG_WAIT_INPUT_WAIT);
			}
			else if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
				downKeyPress();
				wait(YUG_WAIT_INPUT_WAIT);
			}
			else if(inputUnit.isKeyActive(YUG_KEY_X)){
				confirmKeyPress();
				wait(YUG_WAIT_INPUT_WAIT);
			}
			else if(inputUnit.isKeyActive(YUG_KEY_TRI)){
				viewCardKeyPress();
				wait(YUG_WAIT_INPUT_WAIT);
			}
			else if(inputUnit.isKeyActive(YUG_KEY_R1) || inputUnit.isKeyActive(YUG_KEY_L1)){
				viewBoardKeyPress();
				wait(YUG_WAIT_INPUT_WAIT);
				theBoard.wait(pos.wait[4]);
			}
		}
	}

	void GamePlayer::newTurn(){
		//std::cout<<"Player - New turn\n";
		showCards();
		positionUnit.brightenOutOtherCards();
		moveCardsToHandPosition(0.45f);
		blankUnit.holdInputForNow();
		theBoard.underlay.hideThen(YUG_BOARD_UL_HAND);
		//theBoard.underlay.viewingHand();
		inputUnit.releaseAllComputerKeys();
		if(human)
			inputUnit.takePlayerInputsOnly();
		else
			inputUnit.takeComputerInputsOnly();
		theBoard.currentPlayer = this;
		theBoard.currentPlayerView(pos.wait[5]);
		puzzleCursor.interpolateToHandPos();
		currentChainPosition = YUG_GAME_PLAYER_CHAIN_SWEEPING_IN;
		noOfCardsMarked = 0;
		cursorPosition = 0;
		handView  = true;
		//std::cout<<"End of player new turn\n";
	}

	bool GamePlayer::drawCard(int position){
		if(deck.deckEmpty())
			return false;
		soundUnit.cardSwoosh();
		hand[position] = deck.drawCard();
		cardCreator.setupParents(&(hand[position]));
		theBoard.field.applyField(&hand[position]);
		hand[position].startup();
		if(!human){hand[position].hidden = true;}
		hand[position].smallRender.startup();
		hand[position].smallRender.scale = pos.bCardScale;
		hand[position].smallRender.position = cardOffScreenPosition;
		hand[position].smallRender.interpolate(
			mov.addXOffset(firstCardPosition,cardOffset.x*position),
			pos.wait[4]);
		if(!human){
			hand[position].smallRender.rotationMatrix = pos.eAtkFaceupUp;
		}
		soundUnit.cardSwoosh();
		//std::cout<<"Player: drew Card: "<<hand[position].name.data()<<std::endl;
		return true;
	}

	void GamePlayer::update(){
		for(int i = 0; i < 5; i++){
			if(!hand[i].blankCard())
				hand[i].smallRender.update();
		}
		if(!isWaiting){
			switch(currentChainPosition){
			case YUG_GAME_PLAYER_CHAIN_SWEEPING_IN:
				sweepingInUpdate();
				break;
			case YUG_GAME_PLAYER_CHAIN_DRAWING_CARDS:
				drawingCardsUpdate();
				break;
			case YUG_GAME_PLAYER_CHAIN_TAKING_INPUTS:
				takingInputsUpdate();
				break;
			default:
				break;
			};//end switch
		}else{
			continueWaiting();
		}
	}
	
	void GamePlayer::wait(float duration){
		//if(isWaiting)
		//	std::cout<<"Game Player: one wait time overriding another\n";
		//std::cout<<"WAIT: "<<duration<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$4\n";
		waitDuration = duration;
		currentlyWaited = 0;
		isWaiting = true;

	}
	void GamePlayer::continueWaiting(){
		currentlyWaited += gameClock.lastLoopTime();
		if(currentlyWaited > waitDuration){
			isWaiting = false;
		}
	}

	void GamePlayer::drawingCardsUpdate(){
		//std::cout<<"Player: Drawing card update\n";
		for(unsigned int  i  = 0; i < 5; i++){
			if(hand[i].blankCard()){
				if(!drawCard(i)){
					//std::cout<<"Game Player: out of cards!\n";
					hideHand();
					endUnit.playerOutOfCards();
					return;
				}
				//std::cout<<"DRAW CARD WAIT!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
				wait(0.2f);
				return;
			}//outer if
		}//for loop
		wait(pos.wait[2]);
		//forceCardPositions();
		currentChainPosition = YUG_GAME_PLAYER_CHAIN_TAKING_INPUTS;
		blankUnit.transitionInput(this,0.2f);
		puzzleCursor.doRender = true;
		cursorPosition = 0;
		interpolateCursor();
		return;
	}

	void GamePlayer::sweepingInUpdate(){
		wait(0.2f);
		currentChainPosition = YUG_GAME_PLAYER_CHAIN_DRAWING_CARDS;
		return;
	}

	void GamePlayer::takingInputsUpdate(){

	}

	void GamePlayer::downKeyPress(){
		//std::cout<<"Game Player: keyDown: upDown: "<<fusionMarks[cursorPosition].upDown<<" "<<fusionMarks[cursorPosition].position<<'\n';
		if(fusionMarks[cursorPosition].upDown == YUG_MARKINGS_UP){
			soundUnit.playOnce("GameData/sounds/menu/cursorMove.wav");
			fusionMarks[cursorPosition].upDown = YUG_MARKINGS_MIDDLE;
			rollBackFusionCount(fusionMarks[cursorPosition].position);
			fusionMarks[cursorPosition].position = 0;
			hand[cursorPosition].smallRender.interpolate(
				mov.addYOffset(
					hand[cursorPosition].smallRender.position,
					-cardOffset.y),
				pos.wait[1]);
			
		}else if(fusionMarks[cursorPosition].upDown == YUG_MARKINGS_MIDDLE)
		{
			soundUnit.playOnce("GameData/sounds/menu/cursorMove.wav");
			fusionMarks[cursorPosition].upDown = YUG_MARKINGS_DOWN;
			fusionMarks[cursorPosition].position = ++noOfCardsMarked;
			hand[cursorPosition].hidden = false;
			hand[cursorPosition].smallRender.interpolate(
				mov.addYOffset(
					hand[cursorPosition].smallRender.position,
					-cardOffset.y),
				pos.wait[1]);
		}	
	}
	void GamePlayer::upKeyPress(){
		if(fusionMarks[cursorPosition].upDown == YUG_MARKINGS_DOWN){
			soundUnit.playOnce("GameData/sounds/menu/cursorMove.wav");
			fusionMarks[cursorPosition].upDown = YUG_MARKINGS_MIDDLE;
			rollBackFusionCount(fusionMarks[cursorPosition].position);
			fusionMarks[cursorPosition].position = 0;
			hand[cursorPosition].smallRender.interpolate(
				mov.addYOffset(
					hand[cursorPosition].smallRender.position,
					cardOffset.y),
				pos.wait[1]);
			
		}else if(fusionMarks[cursorPosition].upDown == YUG_MARKINGS_MIDDLE)
		{
			soundUnit.playOnce("GameData/sounds/menu/cursorMove.wav");
			fusionMarks[cursorPosition].upDown = YUG_MARKINGS_UP;
			fusionMarks[cursorPosition].position = ++noOfCardsMarked;
			hand[cursorPosition].hidden = false;
			hand[cursorPosition].smallRender.interpolate(
				mov.addYOffset(
					hand[cursorPosition].smallRender.position,
					cardOffset.y),
				pos.wait[1]);
		}
	}
	void GamePlayer::leftKeyPress(){
		if(cursorPosition > 0){
			cursorPosition--;
			soundUnit.cursorMove();
			puzzleCursor.cursorModel.interpolate(glm::vec3(
				puzzleCursor.cursorModel.position.x-cursorOffset.x,
				puzzleCursor.cursorModel.position.y,
				puzzleCursor.cursorModel.position.z),
				0.1f);
		}
	}

	void GamePlayer::rightKeyPress(){
		if(cursorPosition < 4){
			cursorPosition++;
			soundUnit.cursorMove();
			puzzleCursor.cursorModel.interpolate(
				mov.addXOffset(
					puzzleCursor.cursorModel.position,
					cursorOffset.x),
				pos.wait[1]);
		}
	}
	void GamePlayer::confirmKeyPress(){
		//std::cout<<"Passing results to fusion unit\n";
		puzzleCursor.doRender = false;
		soundUnit.cursorSelect();
		positionUnit.start();
		currentChainPosition = YUG_GAME_PLAYER_CHAIN_FINISHED;
	}
	void GamePlayer::viewCardKeyPress(){
		//std::cout<<"View card\n";
		cardDisplayUnit.displayCard(hand[cursorPosition].bigRender);
		wait(0.2f);
	}
	void GamePlayer::viewBoardKeyPress(){
		//std::cout<<"View board\n";
		theBoard.lockRowMovement = false;
		theBoard.cP[0] = 0;
		theBoard.cP[1] = YUG_BOARD_PLAYER_MON_ROW;
		toBoardView();
		theBoard.chain = YUG_BOARD_CH_VIEWONLY;
		blankUnit.transitionInput(&theBoard, YUG_WAIT_INPUT_WAIT);
		
		hideCards();
	}

	void GamePlayer::rollBackFusionCount(int removedPos){
		for(int i = 0; i < 5; i++){
			if(fusionMarks[i].position > removedPos)
				fusionMarks[i].position--;
		}
		noOfCardsMarked--;
	}
	void GamePlayer::drawMarkingOn(int position, int markNumber){
		textPrinter.ignoreCamera = false;
		textPrinter.leftAlign = true;
		if(handView){
			glm::mat4 boardRotation = (human?glm::mat4():pos.eAtkFaceupUp);
			textPrinter.printSymbol(YUG_FUSION_SYMBOL_1 + (markNumber-1),
				markingScale,
				mov.addOffset(hand[position].smallRender.position,
				markingOffset),
				boardRotation
				);	
		}else{
			glm::mat4 boardRotation = (human?pos.atkFaceupFlat:pos.eAtkFaceupFlat);
			textPrinter.printSymbol(YUG_FUSION_SYMBOL_1 + (markNumber-1),
				markingScale,
				mov.addOffset(hand[position].smallRender.position,
				markingBoardOffset),
				boardRotation
				);

		}
	}

	void GamePlayer::hideCards(){
		/*for(int i = 0; i <5; i++){
			hand[i].smallRender.position.y = -5;
		}*/
	}
	void GamePlayer::showCards(){
		for(int i = 0; i <5; i++){
			hand[i].smallRender.doRender = true;//position.y = firstCardPosition.y;
		}
	}
	void GamePlayer::forceCardPositions(){
		/*for(int i = 0; i <5; i++){
			if(!hand[i].blankCard()){
				hand[i].smallRender.position = 
					stateUnit.relativeInterpolate(
					firstCardPosition,
					cardXOffset*i,0.0f,0.0f);
			}
		}*/
	}

	void GamePlayer::interpolateCursor(){
		puzzleCursor.cursorModel.position = 
			mov.addXOffset(
				cursorFirstPosition,
				cursorOffset.x*cursorPosition);
	}

	Card::CardData* GamePlayer::getCurrentCard(){
		return &(hand[cursorPosition]);
	}
	bool GamePlayer::currentCardGood(){
		return !(hand[cursorPosition].blankCard());
	}

	void GamePlayer::moveCardsToViewPosition(){
		for(int i = 0; i < 5; i++){
			if(human)
				hand[i].smallRender.rotate(pos.atkFaceupFlat,
					pos.wait[2]);
			else
				hand[i].smallRender.rotate(pos.eAtkFaceupFlat,
					pos.wait[2]);
			interpolateToBoardPosition(i);
		}

	}
	void GamePlayer::moveCardsToHandPosition(){
		for(int i = 0; i < 5; i++){
			if(human)
				hand[i].smallRender.rotate(pos.atkFaceupUp,
					pos.wait[2]);
			else
				hand[i].smallRender.rotate(pos.eAtkFaceupUp,
					pos.wait[2]);
			interpolateToHandPosition(i);
		}
	}
	void GamePlayer::moveCardsToHandPosition(float duration){
		for(int i = 0; i < 5; i++){
			if(human)
				hand[i].smallRender.rotate(pos.atkFaceupUp,
					duration);
			else
				hand[i].smallRender.rotate(pos.eAtkFaceupUp,
					duration);
			interpolateToHandPosition(i, duration);
		}
	}


	void GamePlayer::toBoardView(){
		theBoard.battleCursor.turnon();
		
		puzzleCursor.doRender = false;
		theBoard.halfView = true;
		moveCardsToViewPosition();
		theBoard.currentBoardView(pos.wait[2]);
		theBoard.underlay.hideThen(YUG_BOARD_UL_HAND_BOARD);
		theBoard.battleCursorReaction();
		theBoard.battleCursor.move();
		handView = false;
	}
	void GamePlayer::toHandView(){
		theBoard.battleCursor.turnoff();
		puzzleCursor.doRender = true;
		interpolateCursor();
		moveCardsToHandPosition();
		theBoard.underlay.hideThen(YUG_BOARD_UL_HAND);
		handView = true;
	}
	void GamePlayer::interpolateToBoardPosition(int i){
		glm::vec3 basicPosition = mov.addXOffset(
			firstCardBoardPosition, cardBoardOffset.x*i);
		hand[i].smallRender.interpolate(
			mov.addZOffset(basicPosition, getCorrectBoardZ(i))
			,0.21f);
	}
	void GamePlayer::interpolateToHandPosition(int i){
		glm::vec3 basicPosition = mov.addXOffset(
			firstCardPosition, cardOffset.x*i);
		hand[i].smallRender.interpolate(
			mov.addYOffset(
				basicPosition,
				cardOffset.y*fusionMarks[i].upDown),
			pos.wait[2]);
	}
	void GamePlayer::interpolateToHandPosition(int i, float duration){
		glm::vec3 basicPosition = mov.addXOffset(
			firstCardPosition, cardOffset.x*i);
		hand[i].smallRender.interpolate(
			mov.addYOffset(
				basicPosition,
				cardOffset.y*fusionMarks[i].upDown),
			duration);
	}
	float GamePlayer::getCorrectBoardZ(int i){
		float z = 0;
		if(human){
			if(theBoard.cP[1] == YUG_BOARD_PLAYER_MAG_ROW){
				z = cardBoardOffset.z;
			}else if(theBoard.cP[1] == YUG_BOARD_ENEMY_MON_ROW){
				z = cardBoardJumpOffset.z;
			}else if(theBoard.cP[1] == YUG_BOARD_ENEMY_MAG_ROW){
				z = cardBoardJumpOffset.z - cardBoardOffset.z;
			}
		}else{
			if(theBoard.cP[1] == YUG_BOARD_ENEMY_MAG_ROW){
				z = cardBoardOffset.z;
			}else if(theBoard.cP[1] == YUG_BOARD_PLAYER_MON_ROW){
				z = cardBoardJumpOffset.z;
			}else if(theBoard.cP[1] == YUG_BOARD_PLAYER_MAG_ROW){
				z = cardBoardJumpOffset.z + cardBoardOffset.z;
			}
		}
		if(human){
			if(fusionMarks[i].upDown == YUG_MARKINGS_DOWN)
				z += cardOffset.y;
			else if(fusionMarks[i].upDown == YUG_MARKINGS_UP)
				z -= cardOffset.y;
		}else{
			if(fusionMarks[i].upDown == YUG_MARKINGS_DOWN)
				z -= cardOffset.y;
			else if(fusionMarks[i].upDown == YUG_MARKINGS_UP)
				z += cardOffset.y;

		}

		return z;
	}

	void GamePlayer::demarkCard(){
		for(int i = 0; i < 5; i++){
			if(fusionMarks[i].upDown != YUG_MARKINGS_MIDDLE){
				fusionMarks[i].upDown = YUG_MARKINGS_MIDDLE;
				noOfCardsMarked = 0;
				cursorPosition = i;
				interpolateCursor();
				interpolateToHandPosition(i);
				return;
			}
		}
	}

	void GamePlayer::hideHand(){
		float z = 1.0f;
		if(!theBoard.playerControlling()){
			z = -1.0f;
		}
		for(int i = 0 ; i < 5; i++){
			hand[i].smallRender.interpolate(mov.addZOffset(hand[i].smallRender.position, z),pos.wait[2]);
			hand[i].smallRender.doRender = false;

		}
		theBoard.underlay.hide();


	}

	void GamePlayer::removeLP(int amount){
		//std::cout<<"gamPlay removeLP: amount = "<<amount<<std::endl;
		lifepoints -= amount;
		if(lifepoints < 0)
			lifepoints = 0;
		if(lifepoints > 9999)
			lifepoints = 9999;
		if(human)
			theBoard.LP.newPlayerLP(lifepoints, pos.wait[5]);
		else
			theBoard.LP.newEnemyLP(lifepoints, pos.wait[5]);
	}

	void GamePlayer::moveCardsToHiddenPosition(){

		glm::vec3 startPosition = mov.addYOffset(firstCardPosition, -1.0f);
		for(unsigned int i = 0; i < 5; i++){
			if(!hand[i].blankCard()){
				hand[i].smallRender.doRender = false;
				hand[i].smallRender.interpolate(mov.addXOffset(startPosition,cardOffset.x*i), 0.2f);
				if(human){
					hand[i].smallRender.rotate(pos.atkFaceupUp, 0.2f);
				}else{
					hand[i].smallRender.rotate(pos.eAtkFaceupUp, 0.2f);
				}
			}
		}


	}

}

