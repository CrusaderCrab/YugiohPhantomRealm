#include <Game\Duel\Board.h>//HAS glew.h, glm
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Duel\FusionUnit.h>
#include <Utility\TextPrinter.h>
#include <Utility\BlankUnit.h>
#include <Utility\InputUnit.h>
#include <Base\RenderUnit.h>
#include <Utility\StateUnit.h>
#include <Utility\Clock.h>
#include <Utility\UpdateUnit.h>
#include <Game\Cursor.h>
#include <Game\Animation\Camera.h>
#include <gtc\matrix_transform.hpp>
#include <Game\Cards\CardDisplayUnit.h>
#include <Game\Duel\PositionUnit.h>
#include <Game\Duel\BattleUnit.h>
#include <Utility\SoundUnit.h>
#include <Game\VectorUnit.h>
#include <Game\Duel\markingStruct.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Duel\Parts\SwordUnit.h>
#include <Utility\TextureLoader.h>


#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
#include <iostream>

#define YUG_BOARD_BOARD_POSITION glm::vec3(0.0f, -5.0f, 0.0f);
#define YUG_BOARD_BOARD_ROTATION glm::vec3(0.0f, 1.0f, 0.0f)
#define YUG_BOARD_BOARD_SCALE glm::vec3(0.4f, 0.4f, 0.4f)
#define YUG_BOARD_UNDERLAY_POSITION glm::vec3(0.0f, -0.97f, -3.75f)//glm::vec3(0.0f, -0.22f, -0.9f);
#define YUG_BOARD_UNDERLAY_HIDDEN_POSITION glm::vec3(0.0f, -20.0f, -3.75f)
#define YUG_BOARD_UNDERLAY_SCALE glm::vec3(1.53f, 0.25f, 1.0f);
#define YUG_BOARD_LIFEPOINTS_POSITION glm::vec3(0.25f, 0.22f, -0.9f);
#define YUG_BOARD_LIFEPOINTS_SCALE glm::vec3(0.07f, 0.04f, 0.2f);
#define YUG_BOARD_FIELD_WIDGET_POSITION glm::vec3(-0.25f, 0.22f, -0.9f);
#define YUG_BOARD_FIELD_WIDGET_SCALE glm::vec3(0.07f, 0.05f, 0.2f);
#define YUG_PLANE_FILE_PATH "GameData/models/plane02.obj"
#define YUG_BOARD_WIDGET_ROTATE glm::mat4(0,0,1,0, 1,0,0,0, 0,1,0,0, 0,0,0,1);

#define YUG_BOARD_CURSOR_PLAYER_ORIGIN glm::vec3(-0.45f, -0.29f, -2.0f)
#define YUG_BOARD_CURSOR_ENEMY_ORIGIN glm::vec3(-0.7f, -0.31f, -2.0f)
#define YUG_BOARD_CURSOR_PLAYER_OFFSET glm::vec3(0.18f, 0.15f, 0.0f)
#define YUG_BOARD_CURSOR_ENEMY_OFFSET glm::vec3(0.1f, 0.1f, 0.0f)

#define YUG_BOARD_CARD_ORIGIN glm::vec3(-0.72f,-0.37f,0.75f)
#define YUG_BOARD_CARD_OFFSET glm::vec3(0.32f, 0.0f, -0.5f)
#define YUG_BOARD_CARD_SCALE glm::vec3(0.75f,0.75f,1.0f);
namespace Duel{

	Board Board::boardInstance;
	Card::CardData Board::board[YUG_BOARD_CARDS_PER_ROW][YUG_BOARD_CARDS_PER_COL];
	int Board::cP[2];
	int Board::aP[2];

	bool Board::initialize(){
		boardModel.startup("GameData/models/board2.obj", 
			"GameData/textures/board/boardUV2.png");
		textureLoader.deleteTexture(&boardModel.textureBO);
		setupModels();
		return true;
		
	}
	void Board::setupModels(){
		boardModel.position = YUG_BOARD_BOARD_POSITION;
		boardModel.rotationMatrix = glm::mat4();
		boardModel.scale = YUG_BOARD_BOARD_SCALE;
		boardModel.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	}

	bool Board::shutdown(){
		boardModel.cleanup();
		return true;
	}

	void Board::toThisScreen(){
		startup();
	}

	void Board::beginGame(){
		//startup();
		closed = false;
		updateUnit.newUpdater(this);
		stateUnit.renderer->currentRenderer = &theBoard;
		//std::cout<<"Board: Game started\n";
		//animationSwoopIn();
		//std::cout<<"board::beginGame: prepareBoard\n";
		prepareBoardModel();
		chain = YUG_BOARD_CH_PANIN;
		inputUnit.currentInput = this;
		wait(0.5f);
		//std::cout<<"board::beginGame: done\n";
		//player.newTurn();

	}

	bool Board::startup(char miscInf){
		miscInfo = miscInf;
		return startup();
	}

	bool Board::startup(){
		for(int col = 0; col < YUG_BOARD_CARDS_PER_ROW; col++){
			for(int row = 0; row < YUG_BOARD_CARDS_PER_COL; row++){
				board[col][row] = cardCreator.blankCard();
				//board[col][row] = cardCreator.createCard(400);
				//cardCreator.setupParents(&board[col][row]);
			}
		}
		for(unsigned int i = 0; i <  player.hand.size(); i++){
			player.hand[i] = cardCreator.blankCard();
		}
		for(unsigned int i = 0; i <  enemy.hand.size(); i++){
			enemy.hand[i] = cardCreator.blankCard();
		}


		doRender = true;
		cP[0] = 0;
		cP[1] = YUG_BOARD_PLAYER_MON_ROW;
		currentPlayer = &player;
		halfView = false;
		magicBeforeParticles = false;
		trapBeforeParticles = false;
		puzzleCursor.cursorModel.position = glm::vec3(2.0f, 0.0f, -5.0f);
		puzzleCursor.doRender = true;
		puzzleCursor.cursorModel.doRender = true;
		//std::cout<<"Prev Board TexID: "<<boardModel.textureBO<<std::endl;
		//std::cout<<"Prev field model texID: "<<field.fieldBackground.textureBO<<std::endl;
		//std::cout<<"Prev field board texture texID: "<<field.currentTexture<<std::endl;
		field.startup();
		//std::cout<<"new field model texID: "<<field.fieldBackground.textureBO<<std::endl;
		//std::cout<<"new field board texture texID: "<<field.currentTexture<<std::endl;

		boardModel.textureBO = field.currentTexture;
		//std::cout<<"new Board TexID: "<<boardModel.textureBO<<std::endl;

		//std::cout<<"prev LP texID: "<<LP.LPbackground.textureBO<<std::endl;
		LP.startup();
		//std::cout<<"new LP texID: "<<LP.LPbackground.textureBO<<std::endl;
		//std::cout<<"final Board TexID: "<<boardModel.textureBO<<std::endl;

		underlay.startup();			//std::cout<<"Board start 01\n";
		battleCursor.startup();			//std::cout<<"Board start 02\n";
		player.startup(YUG_GP_HUMAN);			//std::cout<<"Board start 03\n";
		enemy.startup(YUG_GP_AI);			//std::cout<<"Board start 04\n";
		endUnit.startup();			//std::cout<<"Board start 05\n";
		magicUnit.startup();			//std::cout<<"Board start 06\n";
		trapUnit.startup();			//std::cout<<"Board start 07\n";
		swordUnit.startup();			//std::cout<<"Board start 08\n";
		beginGame();
		return true;
	}
	bool Board::cleanup(){
		swordUnit.cleanup();
		trapUnit.cleanup();
		magicUnit.cleanup();
		endUnit.cleanup();
		player.cleanup();
		enemy.cleanup();
		battleCursor.cleanup();
		LP.cleanup();
		field.cleanup();
		underlay.cleanup();

		for(int col = 0; col < YUG_BOARD_CARDS_PER_ROW; col++){
			for(int row = 0; row < YUG_BOARD_CARDS_PER_COL; row++){
				if(!board[col][row].blankCard())
					board[col][row].cleanup();
			}
		}
		for(unsigned int i = 0; i <  player.hand.size(); i++){
			if(!player.hand[i].blankCard())
				player.hand[i].cleanup();
		}
		for(unsigned int i = 0; i <  enemy.hand.size(); i++){
			if(!enemy.hand[i].blankCard())
				enemy.hand[i].cleanup();
		}


		return true;
	}

	void Board::pressNextEnemyInput(){
		if(currentPlayer == &enemy){
				//std::cout<<"Board: getting enemy input\n";
				if(enemy.ai->hasMovesLeft()){
					enemy.ai->pressNextKey();
				}else{
					enemy.ai->calcBoardMove();
				}
			}
	}


	void Board::input(){
		if(!isWaiting){
			if(chain == YUG_BOARD_CH_VIEWONLY){
				pressNextEnemyInput();
				normalInputs();
				viewOnlyInputs();
			}else if(chain == YUG_BOARD_CH_CHOOSINGPOSITION){
				pressNextEnemyInput();
				normalInputs();
				choosePositionInputs();
			}else if(chain == YUG_BOARD_CH_PLAYERTURN){
				pressNextEnemyInput();
				normalInputs();
				playerTurnInputs();
			}else if(chain == YUG_BOARD_CH_CHOOSEENEMY){
				pressNextEnemyInput();
				normalInputs();
				chooseEnemyInputs();
			}else if(chain == YUG_BOARD_CH_MAGICHCHOOSE){
				pressNextEnemyInput();
				magicChooseInputs();
			}else if(chain == YUG_BOARD_CH_EQUIPCHOOSE){
				pressNextEnemyInput();
				normalInputs();
				equipChooseInputs();
			}
			inputUnit.releaseAllComputerKeys();
		}
		
	}
	void Board::update(){
		for(int i = 0; i < 5; i++){
			for(int j = 0; j <4; j++){
				if(!board[i][j].blankCard()){
					board[i][j].smallRender.update();
				}
			}
		}
		boardModel.update();
		field.update();
		LP.update();
		player.update();
		enemy.update();
		battleCursor.update();
		fusionUnit.update();
		//cardDisplayUnit.update();
		positionUnit.update();
		battleUnit.update();
		underlay.update();
		particleUnit.update();
		endUnit.update();
		magicUnit.update();
		trapUnit.update();
		swordUnit.update();
		if(!isWaiting){
			if(chain == YUG_BOARD_CH_ENDTURN){
				endTurnUpdate();
			}else if(chain == YUG_BOARD_CH_MAGICEFFECT ){
				magicEffectUpdate();
			}else if(chain == YUG_BOARD_CH_START ){
				startUp();
			}else if(chain == YUG_BOARD_CH_FINISH){
				finishUpdate();
			}else if(chain == YUG_BOARD_CH_BATTLEUNIT){
				battleUnitUpdate();
			}else if(chain == YUG_BOARD_CH_EQUIPUNIT){
				equipUnitUpdate();

			}else if(chain == YUG_BOARD_CH_TURNOVER1){
				turnover1Update();
			}else if(chain == YUG_BOARD_CH_TURNOVER1HALF){
				turnover1HalfUpdate();
			}else if(chain == YUG_BOARD_CH_TURNOVER2){
				turnover2Update();
			}else if(chain == YUG_BOARD_CH_TURNOVER3){
				turnover3Update();
			}else if(chain == YUG_BOARD_CH_PANIN){
				panInUpdate();
			}else if(chain == YUG_BOARD_CH_STARTNORM){
				panFinishUpdate();
			}

		}else{
			continueWaiting();
		}
	}







	void Board::normalInputs(){
		if(inputUnit.isKeyActive(YUG_KEY_TRI)){
			if( !((cP[1] == YUG_BOARD_ENEMY_MON_ROW|| //not( (on EnemySide)and(faceDown) )
				cP[1] == YUG_BOARD_ENEMY_MAG_ROW) &&
				!board[cP[0]][cP[1]].faceUp) )
			{
				if(!board[cP[0]][cP[1]].blankCard())
					cardDisplayUnit.displayCard( board[cP[0]][cP[1]].bigRender );
			}
		}else if(inputUnit.isKeyActive(YUG_KEY_UP) && !lockRowMovement){
			if( cP[1] != 3){//4 rows, 0->3
				soundUnit.cursorMove();
				cP[1]++;
				battleCursor.move();
				battleCursorReaction();
				moveCamera();
				moveHandCardsIfNeccessary();
				wait(YUG_WAIT_INPUT_WAIT);
			}
		}else if(inputUnit.isKeyActive(YUG_KEY_DOWN)  && !lockRowMovement){
			if( cP[1] != 0){//4 rows, 0->3
				soundUnit.cursorMove();
				cP[1]--;
				battleCursor.move();
				battleCursorReaction();
				moveCamera();
				moveHandCardsIfNeccessary();
				wait(YUG_WAIT_INPUT_WAIT);
			}
		}else if(inputUnit.isKeyActive(YUG_KEY_LEFT)){
			if( cP[0] != 0){//5 cols, 0->4
				soundUnit.cursorMove();
				cP[0]--;
				battleCursor.move();
				battleCursorReaction();
				moveCamera();
				
				wait(YUG_WAIT_INPUT_WAIT);
			}
		}else if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){
			if( cP[0] != 4){//5 cols, 0->4
				soundUnit.cursorMove();
				cP[0]++;
				battleCursor.move();
				battleCursorReaction();
				moveCamera();
				wait(YUG_WAIT_INPUT_WAIT);
			}
		}
	}
	
	void Board::viewOnlyInputs(){
		if(inputUnit.isKeyActive(YUG_KEY_O)||inputUnit.isKeyActive(YUG_KEY_L1)||inputUnit.isKeyActive(YUG_KEY_R1))
		{
			blankUnit.transitionInput(&player, YUG_WAIT_INPUT_WAIT);
			cameraToPlayer();
			wait(YUG_WAIT_INPUT_WAIT);
			chain = YUG_BOARD_CH_IDLE;
			//player.showCards();
			player.interpolateCursor();
			player.toHandView();
			player.wait(pos.wait[5]);
		}
	}
	void Board::choosePositionInputs(){
		if(inputUnit.isKeyActive(YUG_KEY_O)){
			soundUnit.cursorCancel();
			if(!positionUnit.multiCards){
				positionUnit.chain = YUG_POSITION_CH_FACE;
				lockRowMovement = false;
				currentPlayer->toHandView();
				cameraToPlayer();
				positionUnit.returnToChooseFace();
				puzzleCursor.doRender = false;
				chain = YUG_BOARD_CH_IDLE;
				wait(0.2f);
			}else{
				battleCursor.turnoff();
				currentPlayer->toHandView();
				cameraToPlayer();
				blankUnit.transitionInput(currentPlayer, pos.wait[2]);
				currentPlayer->currentChainPosition = YUG_GAME_PLAYER_CHAIN_TAKING_INPUTS;
				chain = YUG_BOARD_CH_IDLE;
				wait(0.2f);
			}
		}else if(inputUnit.isKeyActive(YUG_KEY_X)){
			if(currentPlayer->hand[positionUnit.cP].monMagTrap==YUG_EQUIP_CARD &&
			positionUnit.isSpaceEmpty() && theBoard.currentPlayer->noOfCardsMarked == 0 &&
			(cP[1] == YUG_BOARD_PLAYER_MON_ROW)){
				soundUnit.cursorSelectionFail();
				return;
			}
			soundUnit.cursorSelect();
			underlay.hide();
			lockRowMovement = false;
			battleCursor.turnoff();
			chain = YUG_BOARD_CH_IDLE;
			//std::cout<<"Board: choose position: X PRESSED\n";
			//Fuse with board
			bool noMarkFuse = false;
			if(!positionUnit.isSpaceEmpty()){
				positionUnit.addBoardCardToFusion();
				if(positionUnit.noMarkUps()){
					positionUnit.addHandCardToFusion();
					noMarkFuse = true;
				}
			}

			//normal fusion
			//std::cout<<"current Board pos:" <<cP[0]<<" "<<cP[1]<<std::endl;
			//std::cout<<"board:pos:space empty = "<<positionUnit.isSpaceEmpty()<<std::endl;
			if(positionUnit.fusionNeeded() || !(positionUnit.isSpaceEmpty()) || noMarkFuse){
				//std::cout<<"board: choose pos: trying to fuse\n";
				cameraToPlayer();
				if(currentPlayer == &player)
					fusionUnit.handFunction(YUG_GAME_PLAYER_PLAYER);
				else
					fusionUnit.handFunction(YUG_GAME_PLAYER_ENEMY);
				theBoard.underlay.hide();
				for(int i = 0; i <currentPlayer->hand.size(); i++){
					currentPlayer->hand[i].smallRender.interpolate(
					stateUnit.relativeInterpolate(
					currentPlayer->hand[i].smallRender.position,
					0.0f,-2.0f,0.0f),
				0.1f);
				}
			}else{
				//no fusion
				//std::cout<<"board: choose pos: no fuse\n";
				positionUnit.passCardToPositionUnit();
				if(positionUnit.isMonster()){
					chain = YUG_BOARD_CH_IDLE;
					blankUnit.transitionInput(&(positionUnit),0.2f);
					positionUnit.chain = YUG_POSITION_CH_CONS;
					cameraToPlayer();
					positionUnit.conPosition();
					positionUnit.wait(0.5f);
					positionUnit.doRender = true;
					//battleCursor.turnoff();
					currentPlayer->hideHand();
					
				}else{//no fusion non-monster
					if(isMagicCard(&positionUnit.card)){
						if(positionUnit.card.faceUp){
							//activate magic card
						}else{
							positionUnit.playMagicFaceDown();
						}
					}else{
						positionUnit.playMagicFaceDown();
					}
				}
			}
		}
	}
	void Board::playerTurnInputs(){
		if(inputUnit.isKeyActive(YUG_KEY_START)){



		/*	GLint nTotalMemoryInKB = 0;
		glGetIntegerv( GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
                       &nTotalMemoryInKB );
		GLint nCurAvailMemoryInKB = 0;
		glGetIntegerv( GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
                       &nCurAvailMemoryInKB );
		std::cout<<"Total Mem: "<<nTotalMemoryInKB<<" Current Mem: "<<nCurAvailMemoryInKB<<std::endl;
		*/


		for(unsigned int i = 0; i < 5; i++){
			currentPlayer->fusionMarks[i].position = 0;
			currentPlayer->fusionMarks[i].upDown = YUG_MARKINGS_MIDDLE;
		}
			swordUnit.currentPlayerTurnEnd();
			newTurn();

		}else if((!board[cP[0]][cP[1]].blankCard()) &&
			(!board[cP[0]][cP[1]].hasAttacked) &&
			overPlayerCard()){
			if(inputUnit.isKeyActive(YUG_KEY_L1) && overMonsterCard()){
				changeCardMode();
				soundUnit.cardSwivel();
				wait(YUG_WAIT_INPUT_WAIT);
			}else if(inputUnit.isKeyActive(YUG_KEY_R1) && overMonsterCard()){
				soundUnit.cardSwivel();
				changeCardMode();
				wait(YUG_WAIT_INPUT_WAIT);
			}else if(inputUnit.isKeyActive(YUG_KEY_X)){//checks hassAttacked in outer if
				board[cP[0]][cP[1]].hidden = false;
				if(overMonsterCard() && 
				   board[cP[0]][cP[1]].attackMode && 
				   swordUnit.currentPlayerCanAttack() &&
				   (theBoard.player.deck.cardsLeft!=35 || !playerControlling()))
					{
					soundUnit.cursorSelect();
					chain = YUG_BOARD_CH_CHOOSEENEMY;
					aP[0] = cP[0];
					aP[1] = cP[1];
					moveCursorToAttackRow();
					battleCursor.move();
					battleCursorReaction();
					moveCamera();
					underlay.viewingBoardAttack();
					lockRowMovement = true;
					wait(0.2f);
				}else if (isMagicCard(&board[cP[0]][cP[1]]) ){
					confirmMagicCard();
				}else if(board[cP[0]][cP[1]].monMagTrap == YUG_EQUIP_CARD){
					chooseEquipCard();
				}
			}

		}
	}
	void Board::chooseEnemyInputs(){
		updateGuardWarnings();
		if(inputUnit.isKeyActive(YUG_KEY_X)){
			//std::cout<<"\n###\nBoard Input: Attack Confirmed\n";
			//if(trapUnit.enemyListen(YUG_TRAP_ATTACK) ){
				//std::cout<<"Board Input: attack trap active\n";
				//lockRowMovement = false;
				//trapUnit.start(YUG_TRAP_ATTACK);
			//}else{
			if(!board[cP[0]][cP[1]].blankCard()){
				if(theBoard.playerControlling()){
					endUnit.noOfAttacks++;
				}
				if(trapUnit.enemyListen(YUG_TRAP_ATTACK) ){
					//std::cout<<"Board Input: attack trap active\n";
					lockRowMovement = false;
					trapUnit.start(YUG_TRAP_ATTACK);
				}else{
					soundUnit.cursorSelect();
					battleUnit.startBattle(false);
					lockRowMovement = false;
				}
			}else if(enemyHasNoCards()){
				if(theBoard.playerControlling()){
					endUnit.noOfAttacks++;
				}
				if(trapUnit.enemyListen(YUG_TRAP_ATTACK) ){
					//std::cout<<"Board Input: attack trap active\n";
					lockRowMovement = false;
					trapUnit.start(YUG_TRAP_ATTACK);
				}else{
					//std::cout<<"Board: direct atk\n";
					battleUnit.startBattle(true);
					lockRowMovement = false;
				}
			}
			//}
		}else if(inputUnit.isKeyActive(YUG_KEY_O)){
			soundUnit.cursorCancel();
			lockRowMovement = false;
			cP[1] = aP[1];
			cP[0] = aP[0];
			battleCursor.move();
			moveCamera();
			underlay.viewingBoard();
			chain = YUG_BOARD_CH_PLAYERTURN;
			wait(pos.wait[2]);
			battleCursorReaction();
		}
	}
	void Board::magicChooseInputs(){
		if(inputUnit.isKeyActive(YUG_KEY_X)){
			preMagicPosition = YUG_BOARD_CH_IDLE;
			interpolateBoardAndCardAmtran(getCurrentOverallAmtran(),0.001f);
			magicUnit.startUpdate(YUG_MAG_BOARD);
		}else if(inputUnit.isKeyActive(YUG_KEY_O)){
			soundUnit.cursorCancel();
			float aniTime = 0.2f;
			pullCardToBoard(cP[0],cP[1],aniTime);
			rotateCardToBoard(cP[0],cP[1],aniTime);
			interpolateBoardAndCardAmtran(getCurrentOverallAmtran(),0.001f);
			chain = YUG_BOARD_CH_PLAYERTURN;
			wait(aniTime);
		}
	}
	void Board::equipChooseInputs(){
		if(inputUnit.isKeyActive(YUG_KEY_X)){
			if(positionUnit.isSpaceEmpty()){
				soundUnit.cursorSelectionFail();
				return;
			}
			boardFusion();
			blankUnit.holdInputForNow();
			lockRowMovement = false;
		}else if(inputUnit.isKeyActive(YUG_KEY_O)){
			soundUnit.cursorCancel();
			lockRowMovement = false;
			float aniTime = 0.2f;
			cP[1] = equipRow;
			cP[0] = equipColumn;
			moveCamera();
			battleCursor.move();
			battleCursorReaction();
			chain = YUG_BOARD_CH_PLAYERTURN;
			wait(aniTime);
		}
	}

	void Board::endTurnUpdate(){

	}
	void Board::magicEffectUpdate(){

	}
	void Board::startUp(){

	}
	void Board::finishUpdate(){

	}
	void Board::battleUnitUpdate(){

	}
	void Board::equipUnitUpdate(){

	}

	void Board::interpolateCursor(){
		puzzleCursor.cursorModel.interpolate(
			stateUnit.relativeInterpolate(
				YUG_BOARD_CURSOR_PLAYER_ORIGIN, 
				cP[0]*YUG_BOARD_CURSOR_PLAYER_OFFSET.x,
				cP[1]*YUG_BOARD_CURSOR_PLAYER_OFFSET.y,
				0.0f),
			0.1f);
	}
	void Board::cameraToBoardCursor(){
		camera.interpolate(
			stateUnit.relativeInterpolate(
			pos.bPlayerCamPosOrigin,
			0.0*cP[0], 0.0f,-0.2*cP[1]),
		0.2f);
		camera.interpolateViewDirection(
			pos.bPlayerCamViewDirection,
			0.2f);

	}
	void Board::cameraToPlayer(){
		currentPlayerView(pos.wait[2]);
	}

	
	void Board::changeCardMode(){
		if(playerControlling()){
			if(board[cP[0]][cP[1]].attackMode){
				board[cP[0]][cP[1]].attackMode = false;
				if(board[cP[0]][cP[1]].faceUp){
					board[cP[0]][cP[1]].smallRender.rotate(stateUnit.defenseFaceupBoardRotation,0.2f);
				}else{
					board[cP[0]][cP[1]].smallRender.rotate(stateUnit.defenseFacedownBoardRotation,0.2f);
				}
			}else{
				board[cP[0]][cP[1]].attackMode = true;
				if(board[cP[0]][cP[1]].faceUp){
					board[cP[0]][cP[1]].smallRender.rotate(stateUnit.attackFaceupBoardRotation,0.2f);
				}else{
					board[cP[0]][cP[1]].smallRender.rotate(stateUnit.attackFacedownBoardRotation,0.2f);
				}
			}
		}else{
			if(board[cP[0]][cP[1]].attackMode){
				board[cP[0]][cP[1]].attackMode = false;
				if(board[cP[0]][cP[1]].faceUp){
					board[cP[0]][cP[1]].smallRender.rotate(pos.eDefFaceupFlat,0.2f);
				}else{
					board[cP[0]][cP[1]].smallRender.rotate(pos.eDefFacedownFlat,0.2f);
				}
			}else{
				board[cP[0]][cP[1]].attackMode = true;
				if(board[cP[0]][cP[1]].faceUp){
					board[cP[0]][cP[1]].smallRender.rotate(pos.eAtkFaceupFlat,0.2f);
				}else{
					board[cP[0]][cP[1]].smallRender.rotate(pos.eAtkFacedownFlat,0.2f);
				}
			}
		}
	}


	void Board::moveCursorToAttackRow(){
		cP[0] = 0;
		if(playerControlling())
			cP[1] = YUG_BOARD_ENEMY_MON_ROW;
		else
			cP[1] = YUG_BOARD_PLAYER_MON_ROW;
	}


	void Board::flipCard(int col, int row){
		if(playerControlling()){
			if(board[col][row].faceUp){
				if(board[col][row].attackMode){
					board[col][row].smallRender.rotate(stateUnit.attackFacedownBoardRotation, YUG_WAIT_INPUT_WAIT);
				}else{
					board[col][row].smallRender.rotate(stateUnit.defenseFacedownBoardRotation, YUG_WAIT_INPUT_WAIT);
				}
			}else{
				if(board[col][row].attackMode){
					board[col][row].smallRender.rotate(stateUnit.attackFaceupBoardRotation, YUG_WAIT_INPUT_WAIT);
				}else{
					board[col][row].smallRender.rotate(stateUnit.defenseFaceupBoardRotation, YUG_WAIT_INPUT_WAIT);
				}
			}
		}else{
			if(board[col][row].faceUp){
				if(board[col][row].attackMode){
					board[col][row].smallRender.rotate(pos.eAtkFacedownFlat, YUG_WAIT_INPUT_WAIT);
				}else{
					board[col][row].smallRender.rotate(pos.eDefFacedownFlat, YUG_WAIT_INPUT_WAIT);
				}
			}else{
				if(board[col][row].attackMode){
					board[col][row].smallRender.rotate(pos.eAtkFaceupFlat, YUG_WAIT_INPUT_WAIT);
				}else{
					board[col][row].smallRender.rotate(pos.eDefFaceupFlat, YUG_WAIT_INPUT_WAIT);
				}
			}
		}
		board[col][row].faceUp = !(board[col][row].faceUp);
	}

	void Board::newTurn(){
		if(playerControlling()){
			endUnit.noOfTurns++;
		}
		battleCursor.turnoff();
		cameraToPlayer();
		wait(0.3f);
		chain = YUG_BOARD_CH_TURNOVER1;
	}

	void Board::confirmMagicCard(){
		float aniTime = 0.2f;
		Card::CardData* c = &(board[cP[0]][cP[1]]);
		chain = YUG_BOARD_CH_MAGICHCHOOSE;
		interpolateBoardAndCardAmtran(getCurrentOverallDarkenedAmtran(),0.001f);
		c->smallRender.interpolateAmtran(getCurrentOverallAmtran(),0.001f);
		if(playerControlling()){
			c->smallRender.rotate(pos.atkFaceupFlat, aniTime);
			c->smallRender.interpolate(pos.bPlayerMagicConfirmPos, aniTime);
		}else{
			c->smallRender.rotate(pos.eAtkFaceupFlat, aniTime);
			c->smallRender.interpolate(pos.bEnemyMagicConfirmPos, aniTime);
		}
		wait(aniTime);
	}

	void Board::chooseEquipCard(){
		chain = YUG_BOARD_CH_EQUIPCHOOSE;
		lockRowMovement = true;
		equipRow = cP[1];
		equipColumn = cP[0];
		playerTurnStartPos();
		moveCamera();
		battleCursor.move();
		battleCursorReaction();
		wait(0.2f);
	}

	void Board::boardFusion(){
		fusionUnit.cards.push_back(board[equipColumn][equipRow]);
		board[equipColumn][equipRow] = cardCreator.blankCard();
		FusionMarkings f;
		f.upDown = YUG_MARKINGS_UP;
		f.position = 1;
		fusionUnit.markings.push_back(f);
		fusionUnit.cards.push_back(board[cP[0]][cP[1]]);
		board[cP[0]][cP[1]] = cardCreator.blankCard();
		f.position++;
		fusionUnit.markings.push_back(f);

		battleCursor.turnoff();
		underlay.hide();
		cameraToPlayer();
		fusionUnit.boardFusion = true;
		if(playerControlling())
			fusionUnit.handFunction(YUG_GAME_PLAYER_PLAYER);
		else
			fusionUnit.handFunction(YUG_GAME_PLAYER_ENEMY);
	}
}