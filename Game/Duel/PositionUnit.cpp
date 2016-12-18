#include <Game\Duel\PositionUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\Clock.h>
#include <Utility\InputUnit.h>
#include <Utility\BlankUnit.h>
#include <Utility\TextPrinter.h>
#include <Utility\StateUnit.h>
#include <Game\Cursor.h>
#include <Utility\SoundUnit.h>
#include <iostream>
#include <Game\VectorUnit.h>
#include <DefinesAndTypedefs.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\Cards\Trap\TrapUnit.h>


#define YUG_POSITION_P_CARD glm::vec3(0.0f,0.0f,0.5f)
#define YUG_POSITION_E_CARD glm::vec3(0.0f,0.0f,-2.0f)
#define YUG_POSITION_P_CONS glm::vec3(0.5f,-0.5f,0.6f)
#define YUG_POSITION_E_CONS glm::vec3(0.0f,-0.5f,-2.1f)

namespace Duel{
	PositionUnit PositionUnit::positionUnitInstance;
	
	bool  PositionUnit::initialize(){
		card = cardCreator.blankCard();
		chain = YUG_POSITION_CH_IDLE;
		hasFocus = false;
		humanPlayer = true;
		doRender = false;
		waitDuration = 0.0f;
		isWaiting = false;
		currentlyWaited = 0.0f;
		conPos = 0;
		initializeArrows();
		initializeConParts();
		return true;
	}
	bool  PositionUnit::shutdown(){
		leftArrow.cleanup();
		rightArrow.cleanup();
		return true;
	}

	void PositionUnit::initializeArrows(){
		leftArrow.startup(YUG_PLANE_FILE_PATH,"GameData/textures/models/positionunit/posarrow1.png");
		leftArrow.addTexture("GameData/textures/models/positionunit/posarrow2.png");
		rightArrow.startup(YUG_PLANE_FILE_PATH,"GameData/textures/models/positionunit/posarrow1.png");
		rightArrow.addTexture("GameData/textures/models/positionunit/posarrow2.png");

		leftArrow.ignoreCamera = true;
		rightArrow.ignoreCamera = true;
		leftArrow.doRender = false;
		rightArrow.doRender = false;
		renderBothArrows = false;
		leftArrow.startFlickering(pos.wait[1]);
		rightArrow.startFlickering(pos.wait[1]);

		leftArrow.position = pos.posLeftArrowPos;
		rightArrow.position = mov.addXOffset(pos.posLeftArrowPos,pos.posRightArrowXOffset);
		leftArrow.scale = pos.posArrowScale;
		rightArrow.scale = pos.posArrowScale;
		rightArrow.rotationMatrix = glm::mat4(-1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

	}

	void PositionUnit::initializeConParts(){
		conBG.startup(YUG_PLANE_FILE_PATH,"GameData/textures/models/positionunit/conbg.png");
		conCursor.startup(YUG_PLANE_FILE_PATH,"GameData/textures/models/positionunit/concursor.png");

		conBG.ignoreCamera = true;
		conCursor.ignoreCamera = true;
		conBG.doRender = false;
		conCursor.doRender = false;
		conBG.position = pos.posConBgPos;
		conBG.scale = pos.posConBgScale;
		conCursor.position = pos.posConCursorPos;
		conCursor.scale = pos.posConCursorScale;
	}

	void PositionUnit::input(){
		if(!isWaiting){
			if(chain == YUG_POSITION_CH_FACE){
				if(theBoard.currentPlayer == &(theBoard.enemy)){
					//std::cout<<"pos: getting enemy input\n"; 
					theBoard.enemy.ai->pressNextKey();
					//inputUnit.wait(0.2f);
				}
				faceInput();
			}
			if(chain == YUG_POSITION_CH_CONS){
				if(theBoard.currentPlayer == &(theBoard.enemy)){
					//std::cout<<"pos: getting enemy input\n"; 
					theBoard.enemy.ai->pressNextKey();
					//inputUnit.wait(0.2f);
				}
				consInput();
			}
		}
		inputUnit.releaseAllComputerKeys();
	}
	void PositionUnit::render(){
		if(doRender){
			leftArrow.render();
			rightArrow.render();
			if(!card.blankCard()){
				card.smallRender.render();
			}
			if(chain == YUG_POSITION_CH_CONS){
				renderCons();
			}
		}
	}
	void PositionUnit::update(){
		if(!card.blankCard()){
			card.smallRender.update();
		}
		if(renderBothArrows){
			leftArrow.update();
			rightArrow.update();
		}
		conCursor.update();
		if(!isWaiting){
			if(chain == YUG_POSITION_CH_PLACING){
				finishUpdate();
			}else if(chain == YUG_POSITION_CH_FINISH){
				finishUpdate();
			}else if(chain == YUG_POSITION_CH_MAGIC_CONS){
				magicConsUpdate();
			}
		}else{
			continueWaiting();
		}
	}

	void PositionUnit::renderCons(){
		conCursor.render();
		conBG.render();
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = false;
		textPrinter.printText("SELECT A GUARDIAN STAR",YUG_TEXT_INFO_FONT, pos.posConGuardScale,
			mov.addOffset(pos.posConBgPos,pos.posPConGuardOffset), glm::mat4());
		textPrinter.leftAlign = true;
		glm::vec3 symPos = mov.addOffset(pos.posConBgPos, pos.posConSymbolOffset);
		textPrinter.printSymbol(card.constellations[0], pos.posConSymbolScale, symPos, glm::mat4());
		textPrinter.printText(stateUnit.getConstellationName(card.constellations[0]), YUG_TEXT_INFO_FONT,
			pos.posConTextScale,mov.addXOffset(symPos, pos.posConTextXOffset),glm::mat4());

		symPos = mov.addYOffset(symPos, pos.posConSymbolYOffset);
		textPrinter.printSymbol(card.constellations[1], pos.posConSymbolScale, symPos, glm::mat4());
		textPrinter.printText(stateUnit.getConstellationName(card.constellations[1]), YUG_TEXT_INFO_FONT,
			pos.posConTextScale,mov.addXOffset(symPos, pos.posConTextXOffset),glm::mat4());
		
	}

	void PositionUnit::faceInput(){
		if(inputUnit.isKeyActive(YUG_KEY_LEFT)){
			flipCard();
			wait(pos.wait[2]);
			//inputUnit.wait(pos.wait[2]);
		}else if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){
			flipCard();
			wait(pos.wait[2]);
			//inputUnit.wait(pos.wait[2]);
		}else if(inputUnit.isKeyActive(YUG_KEY_O)){
			soundUnit.cursorCancel();
			brightenOutOtherCards();
			stopRenderArrows();
			chain = YUG_POSITION_CH_IDLE;
			theBoard.currentPlayer->currentChainPosition = YUG_GAME_PLAYER_CHAIN_TAKING_INPUTS;
			blankUnit.transitionInput(theBoard.currentPlayer,0.2f);
			theBoard.currentPlayer->interpolateToHandPosition(cP);
			theBoard.currentPlayer->hand[cP].faceUp = false;
			flipCard();
			puzzleCursor.doRender = true;
			theBoard.currentPlayer->interpolateCursor();
			wait(0.2f);
			//inputUnit.wait(pos.wait[2]);

		}else if(inputUnit.isKeyActive(YUG_KEY_X)){
			stopRenderArrows();
			soundUnit.cursorSelect();
			chain = YUG_POSITION_CH_IDLE;
			if((theBoard.currentPlayer->hand[cP].monMagTrap == YUG_MAGIC_CARD || theBoard.currentPlayer->hand[cP].monMagTrap == YUG_TRAP_CARD)
				&& theBoard.currentPlayer->hand[cP].faceUp){
				passCardToPositionUnit();
				puzzleCursor.doRender = false;
				theBoard.underlay.hide();
				theBoard.currentPlayer->moveCardsToHiddenPosition();
				brightenOutOtherCards();
				blankUnit.holdInputForNow();
				magicUnit.startUpdate(YUG_MAG_HAND);
			}else{
				theBoard.chain = YUG_BOARD_CH_CHOOSINGPOSITION;
				placeInMagicRow = singleShouldPlaceInMagicRow();
				//std::cout<<"Pos: card face up = "<<theBoard.currentPlayer->hand[cP].faceUp<<std::endl;
				//std::cout<<"Pos: wantFusion = "<<wantFusion()<<std::endl;
				//std::cout<<"Pos: equip Card? = "<<(theBoard.currentPlayer->hand[cP].monMagTrap == YUG_EQUIP_CARD)<<std::endl;
				//std::cout<<"Pos: magic Row? = "<<placeInMagicRow<<std::endl;
				bool upEquip = ((!wantFusion()) && (theBoard.currentPlayer->hand[cP].monMagTrap == YUG_EQUIP_CARD && (theBoard.currentPlayer->hand[cP].faceUp)));
				if(placeInMagicRow && !upEquip){
					theBoard.cP[1] = (theBoard.currentPlayer->human?YUG_BOARD_PLAYER_MAG_ROW:YUG_BOARD_ENEMY_MAG_ROW);
				}else{
					theBoard.cP[1] = (theBoard.currentPlayer->human?YUG_BOARD_PLAYER_MON_ROW:YUG_BOARD_ENEMY_MON_ROW);
				}
				theBoard.cP[0] = theBoard.getFirstEmptyColumn(theBoard.cP[1]);
				theBoard.lockRowMovement = true;
				blankUnit.transitionInput(&(theBoard),0.2f);
				wait(0.2f);
				theBoard.currentPlayer->interpolateToHandPosition(cP);
				theBoard.currentPlayer->toBoardView();
				keepCorrectBoardFace();
				inputUnit.releaseAllComputerKeys();
				//std::cout<<"Pos: going to board choose\n";
			}
		}
	}
	void PositionUnit::consInput(){
		if(inputUnit.isKeyActive(YUG_KEY_UP)){
			if(conPos == 1){
				conPos--;
				soundUnit.cursorMove();
				wait(0.2f);
			}
			moveConCursor();
		}else if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
			if(conPos == 0){
				conPos++;
				soundUnit.cursorMove();
				wait(0.2f);
			}
			moveConCursor();
			inputUnit.releaseAllComputerKeys();
		}else if(inputUnit.isKeyActive(YUG_KEY_X)){
			//std::cout<<"----------Pos: Con X pressed ----------\n";
			chain = YUG_POSITION_CH_PLACING;
			soundUnit.cursorSelect();
			if(theBoard.playerControlling() && card.monMagTrap==YUG_MONSTER_CARD){
				endUnit.noOfMonstersPlayed++;
				endUnit.totalAttackValue += card.attack;
				endUnit.totalDefenseValue += card.defense;
			}
			card.currentConstellation = card.constellations[conPos];
			giveCardtoBoard();
			theBoard.pullCardToBoard(theBoard.cP[0], theBoard.cP[1]);
			wait(pos.wait[2]);
			if(trapUnit.enemyListen(YUG_TRAP_MON_PLAYED)){
				chain = YUG_POSITION_CH_IDLE;
				trapUnit.start(YUG_TRAP_MON_PLAYED);
			}
		}
	}
		
	void PositionUnit::placingUpdate(){
		//...
	}
	void PositionUnit::finishUpdate(){
		toPlayerTurn();
	}
	void PositionUnit::start(){
		doRender = true;
		boardFuse = false;
		fusion_Needed = false;
		cleanOutSingleCard();
		multiCards = wantFusion();
		if(multiCards){
			placeInMagicRow = multiShouldPlaceInMagicRow();
			choosePosition();
		}else{
			cP = theBoard.currentPlayer->cursorPosition;
			chooseFace();
		}
		wait(0.3f);
	}
		
	void PositionUnit::addCardToFusion(){
		boardFuse = true;
	}
	bool PositionUnit::isSpaceEmpty(){
		return((theBoard.board[theBoard.cP[0]][theBoard.cP[1]]).blankCard() );
	}
	bool PositionUnit::wantFusion(){
		return(theBoard.currentPlayer->noOfCardsMarked >= 1 ); 
	}
	void PositionUnit::singleCardsFusion(){

	}
	bool PositionUnit::allMagics(){
		int noMarks = 0;
		for(int i = 0; i<5; i++){
			if(theBoard.currentPlayer->hand[i].monMagTrap == YUG_MONSTER_CARD &&
				theBoard.currentPlayer->fusionMarks[i].upDown != YUG_MARKINGS_MIDDLE){
					return false;
			}
		}
		return (wantFusion());
	}
	bool PositionUnit::isMagic(){
		return (!wantFusion()) && (theBoard.currentPlayer->hand[theBoard.currentPlayer->cursorPosition].monMagTrap != YUG_MONSTER_CARD);
	}	 

	void PositionUnit::wait(float duration){
		//if(isWaiting)
		//	std::cout<<"pos unit: one wait time overriding another\n";
		waitDuration = duration;
		currentlyWaited = 0;
		isWaiting = true;
	}
	void PositionUnit::continueWaiting(){
		currentlyWaited += gameClock.lastLoopTime();
		if(currentlyWaited > waitDuration){
			isWaiting = false;
		}
	}

	void PositionUnit::returnCardToHand(){
		
	}
	bool PositionUnit::noMarkUps(){
		for(int i = 0; i< 5; i++){
			if(theBoard.currentPlayer->fusionMarks[i].upDown != YUG_MARKINGS_MIDDLE &&
				!theBoard.currentPlayer->hand[theBoard.currentPlayer->cursorPosition].blankCard()){
					return false;
			}
		}
		return true;
	}
	void PositionUnit::markHandCardForFusion(){
		fusion_Needed = true;
		//fusionUnit.cards.push_back(card);
		//DOES NOTHING action in fusion unit handfunstion()
		//theBoard.player.fusionMarks[theBoard.player.cursorPosition].upDown = YUG_MARKINGS_UP;
		//theBoard.player.fusionMarks[theBoard.player.cursorPosition].position = 1;

	}
	bool PositionUnit::fusionNeeded(){
		return !(isSpaceEmpty() && (theBoard.currentPlayer->noOfCardsMarked == 0));
	}
	void PositionUnit::interpolateCursor(){
		if(conPos == 0)
			puzzleCursor.cursorModel.position.y += 0.1f;
		else
			puzzleCursor.cursorModel.position.y -= 0.1f;
	}
	void PositionUnit::interpolateFinalCard(){//QAZWSX
		//std::cout<<"pos final: faceup = "<<card.faceUp;
		/*if(card.faceUp)
			card.smallRender.rotate(stateUnit.attackFaceupBoardRotation,0.2f);
		else
			card.smallRender.rotate(stateUnit.attackFacedownBoardRotation,0.2f);
		card.smallRender.interpolate(stateUnit.relativeInterpolate(glm::vec3(-0.72f,-0.37f,0.75f),
			theBoard.cP[0]*0.32f, 0.0f, theBoard.cP[1]*-0.5f),0.2f);
		theBoard.board[theBoard.cP[0]][theBoard.cP[1]] = card;
		cardCreator.setupParents(&theBoard.board[theBoard.cP[0]][theBoard.cP[1]]);
		card = cardCreator.blankCard();*/
	}
	void PositionUnit::interpolateUpCard(){//QAZWSX
		/*card = theBoard.currentPlayer->hand[theBoard.currentPlayer->cursorPosition];
		cardCreator.setupParents(&card);
		theBoard.currentPlayer->hand[theBoard.currentPlayer->cursorPosition] = cardCreator.blankCard();
		card.smallRender.interpolate(YUG_POSITION_P_CARD, 0.2f);*/
		//card.smallRender.position = YUG_POSITION_P_CARD;
		//std::cout<<"Pos: got card:"<<card.name.data()<<std::endl;
		//std::cout<<"pos: card is: "<<card.faceUp<<std::endl;
	}



	void  PositionUnit::cleanOutSingleCard(){
		if(theBoard.currentPlayer->noOfCardsMarked == 1){
			
			theBoard.currentPlayer->demarkCard();
		}
	}
	bool  PositionUnit::multiShouldPlaceInMagicRow(){
		for(unsigned int i = 0; i < 5; i++){
			if(theBoard.currentPlayer->hand[i].monMagTrap == YUG_MONSTER_CARD &&
				theBoard.currentPlayer->fusionMarks[i].upDown != YUG_MARKINGS_MIDDLE){
					return false;
			}
		}
		return true;
	}

	bool PositionUnit::singleShouldPlaceInMagicRow(){
		return (theBoard.currentPlayer->hand[cP].monMagTrap == YUG_MAGIC_CARD ||
			(theBoard.currentPlayer->hand[cP].monMagTrap == YUG_EQUIP_CARD && (!theBoard.currentPlayer->hand[cP].faceUp))
			|| theBoard.currentPlayer->hand[cP].monMagTrap == YUG_TRAP_CARD);
	}

	void  PositionUnit::choosePosition(){
		chain = YUG_POSITION_CH_IDLE;
		theBoard.chain = YUG_BOARD_CH_CHOOSINGPOSITION;
		
		if(placeInMagicRow){
			theBoard.cP[1] = (theBoard.currentPlayer->human?YUG_BOARD_PLAYER_MAG_ROW:YUG_BOARD_ENEMY_MAG_ROW);
		}else{
			theBoard.cP[1] = (theBoard.currentPlayer->human?YUG_BOARD_PLAYER_MON_ROW:YUG_BOARD_ENEMY_MON_ROW);
		}
		theBoard.cP[0] = theBoard.getFirstEmptyColumn(theBoard.cP[1]);
		theBoard.lockRowMovement = true;
		blankUnit.transitionInput(&(theBoard),0.5f);
		wait(0.5f);
		theBoard.currentPlayer->interpolateToHandPosition(cP);
		theBoard.currentPlayer->toBoardView();
	}
	void  PositionUnit::chooseFace(){
		//placeInMagicRow = singleShouldPlaceInMagicRow();
		interpolateToFacePosition();
		rotateToFacePosition();
		darkenOutOtherCards();
		renderArrows();
		chain = YUG_POSITION_CH_FACE;
		blankUnit.transitionInput(this, 0.2f);
		wait(0.5f/*pos.wait[2]*/);
	}
	void PositionUnit::returnToChooseFace(){
		interpolateToFacePosition();
		keepCorrectUpFace();
		renderArrows();
		chain = YUG_POSITION_CH_FACE;
		blankUnit.transitionInput(this, 0.2f);
		wait(pos.wait[2]);

	}

	void PositionUnit::interpolateToFacePosition(){
		if(theBoard.playerControlling()){
			theBoard.currentPlayer->hand[cP].smallRender.interpolate(pos.posPCardPosition, pos.wait[2]);
		}else{
			theBoard.currentPlayer->hand[cP].smallRender.interpolate(pos.posECardPosition, pos.wait[2]);
		}
	}
	void PositionUnit::rotateToFacePosition(){
		theBoard.currentPlayer->hand[cP].faceUp = false;
		if(theBoard.playerControlling()){
			theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.atkFacedownUp, pos.wait[2]);
		}else{
			theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.eAtkFacedownUp, pos.wait[2]);
		}
	}
	
	void PositionUnit::darkenOutOtherCards(){
		for(unsigned int i = 0; i < 5; i++){
			if(i != cP){
				theBoard.currentPlayer->hand[i].smallRender.interpolateAmtran(glm::vec4(0.5f,0.5f,0.5f,1.0f),0.01f);
			}
		}
	}

	void PositionUnit::brightenOutOtherCards(){
		for(unsigned int i = 0; i < 5; i++){
			//if(i != cP){
				theBoard.currentPlayer->hand[i].smallRender.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f),0.01f);
			//}
		}
	}

	void PositionUnit::renderArrows(){
		leftArrow.doRender = true;
		rightArrow.doRender = true;
		renderBothArrows = true;
	}

	void PositionUnit::stopRenderArrows(){
		leftArrow.doRender = false;
		rightArrow.doRender = false;
		renderBothArrows = false;
	}

	void PositionUnit::passCardToPositionUnit(){
		card = theBoard.currentPlayer->hand[cP];
		cardCreator.setupParents(&card);
		theBoard.currentPlayer->hand[cP] = cardCreator.blankCard();
	}
	bool PositionUnit::isMonster(){
		return (card.monMagTrap == YUG_MONSTER_CARD);
	}
	void PositionUnit::conPosition(){
		theBoard.underlay.hide();
		interpolateCardToConPosition();
		rotateCardToConPosition();
		startRenderCons();
		wait(0.5f);
	}

	void PositionUnit::startRenderCons(){
		conBG.doRender = true;
		conCursor.doRender = true;
		conCursor.position = pos.posConCursorPos;
		conPos = 0;

	}

	void PositionUnit::interpolateCardToConPosition(){
		if(theBoard.playerControlling())
			card.smallRender.interpolate(pos.posPCardConPos, pos.wait[2]);
		else
			card.smallRender.interpolate(pos.posECardConPos, pos.wait[2]);
	}

	void PositionUnit::rotateCardToConPosition(){
		if(!card.faceUp){
			if(theBoard.playerControlling())
				card.smallRender.rotate(pos.atkFacedownUp, pos.wait[2]);
			else
				card.smallRender.rotate(pos.eAtkFacedownUp, pos.wait[2]);
		}else{
			if(theBoard.playerControlling())
				card.smallRender.rotate(pos.atkFaceupUp, pos.wait[2]);
			else
				card.smallRender.rotate(pos.eAtkFaceupUp, pos.wait[2]);
		}

	}
	 void PositionUnit::moveConCursor(){
		 conCursor.interpolate(mov.addYOffset(pos.posConCursorPos, pos.posConCursorYOffset*conPos),0.001f);
	 }

	 void PositionUnit::addBoardCardToFusion(){
		theBoard.board[theBoard.cP[0]][theBoard.cP[1]].faceUp = true;
		fusionUnit.cards.push_back(theBoard.board[theBoard.cP[0]][theBoard.cP[1]]);
		theBoard.board[theBoard.cP[0]][theBoard.cP[1]] = cardCreator.blankCard();
		FusionMarkings f;
		f.upDown = YUG_MARKINGS_UP;
		f.position = 1;
		fusionUnit.markings.push_back(f);
	 }
	void PositionUnit::addHandCardToFusion(){
		GamePlayer* g = theBoard.currentPlayer;
		theBoard.currentPlayer->hand[cP].faceUp = true;
		fusionUnit.cards.push_back(theBoard.currentPlayer->hand[cP]);
		theBoard.currentPlayer->hand[cP] = cardCreator.blankCard();/*
		g->hand[g->cursorPosition].faceUp = true;
		fusionUnit.cards.push_back(g->hand[g->cursorPosition]);
		g->hand[g->cursorPosition] = cardCreator.blankCard();*/
		FusionMarkings f;
		f.upDown = YUG_MARKINGS_UP;
		f.position = 1;
		fusionUnit.markings.push_back(f);
	}
	

	void PositionUnit::flipCard(){
		theBoard.currentPlayer->hand[cP].hidden = false;
		if(theBoard.currentPlayer->hand[cP].faceUp){
			theBoard.currentPlayer->hand[cP].faceUp = false;
			if(theBoard.playerControlling())
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.atkFacedownUp, pos.wait[2]);
			else
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.eAtkFacedownUp, pos.wait[2]);
		}else{
			theBoard.currentPlayer->hand[cP].faceUp = true;
			if(theBoard.playerControlling())
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.atkFaceupUp, pos.wait[2]);
			else
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.eAtkFaceupUp, pos.wait[2]);
		}
	}

	void PositionUnit::keepCorrectBoardFace(){
		if(!theBoard.currentPlayer->hand[cP].faceUp){
			if(theBoard.playerControlling())
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.atkFacedownFlat, pos.wait[2]);
			else
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.eAtkFacedownFlat, pos.wait[2]);
		}else{
			if(theBoard.playerControlling())
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.atkFaceupFlat, pos.wait[2]);
			else
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.eAtkFaceupFlat, pos.wait[2]);
		}
	}
	void PositionUnit::keepCorrectUpFace(){
		if(!theBoard.currentPlayer->hand[cP].faceUp){
			if(theBoard.playerControlling())
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.atkFacedownUp, pos.wait[2]);
			else
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.eAtkFacedownUp, pos.wait[2]);
		}else{
			if(theBoard.playerControlling())
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.atkFaceupUp, pos.wait[2]);
			else
				theBoard.currentPlayer->hand[cP].smallRender.rotate(pos.eAtkFaceupUp, pos.wait[2]);
		}

	}

	void PositionUnit::giveCardtoBoard(){
		if(card.monMagTrap == YUG_TRAP_CARD){
			trapUnit.cardNumber = card.cardNumber;
			trapUnit.addTrap(theBoard.cP[0],theBoard.playerControlling());
			if(theBoard.playerControlling()){
				endUnit.noOfTrapsPlayed++;
			}
		}
		theBoard.currentCard = card;
		cardCreator.setupParents(&theBoard.currentCard);
		card = cardCreator.blankCard();
	}
	
	void PositionUnit::toPlayerTurn(){
		//std::cout<<"&&----------------------------------------&&\n";
		//std::cout<<"\t\tPos: Passing over control\t\t";
		theBoard.halfView = false;
		puzzleCursor.doRender = false;
		chain = YUG_POSITION_CH_IDLE;
		theBoard.chain = YUG_BOARD_CH_PLAYERTURN;
		theBoard.enemy.ai->moveList.clear();
		theBoard.enemy.ai->moveIndex = 0;
		blankUnit.transitionInput(&(theBoard),0.2f);
		theBoard.playerTurnStartPos();
		theBoard.currentPlayer->toBoardView();
		theBoard.currentPlayer->moveCardsToHiddenPosition();
		theBoard.underlay.viewingBoard();
		theBoard.battleCursor.turnon();
		theBoard.battleCursor.move();
		theBoard.battleCursorReaction();
		for(int i = 0; i < 5; i++){
			theBoard.board[i][2].hidden = false;
			theBoard.board[i][3].hidden = false;
		}
	}

	void PositionUnit::playMagicFaceDown(){
		theBoard.underlay.hide();
		theBoard.cameraToPlayer();
		theBoard.currentPlayer->hideHand();
		interpolateCardToConPosition();
		rotateCardToConPosition();
		if(card.monMagTrap != YUG_TRAP_CARD){
			endUnit.noOfMagicsPlayed++;
		}
		chain = YUG_POSITION_CH_MAGIC_CONS;
		wait(0.5f);
	}

	void PositionUnit::magicConsUpdate(){
		chain = YUG_POSITION_CH_PLACING;
			giveCardtoBoard();
			theBoard.pullCardToBoard(theBoard.cP[0], theBoard.cP[1]);
			wait(pos.wait[2]);
			inputUnit.releaseAllComputerKeys();
	}

	void PositionUnit::playingMagic(){
		blankUnit.holdInputForNow();
		chain = YUG_POSITION_CH_IDLE;
		magicUnit.startUpdate(YUG_MAG_POSITION);

	}
}