#include <Utility\Clock.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Game\Duel\Board.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Duel\PositionUnit.h>
#include <Utility\BlankUnit.h>
#include <Game\Cursor.h>
#include <Utility\TextPrinter.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Duel\EndUnit.h>
#include <iostream>
#include <Utility\DebugUnit.h>


#include <Game\Cards\Magic\ChangeOfHeart.h>
#include <Game\Cards\Magic\GracefulDice.h>
#include <Game\Cards\Magic\SkullDice.h>
#include <Game\Cards\Magic\ShadowSpell.h>
#include <Game\Cards\Magic\SpellCircle.h>
#include <Game\Cards\Magic\DarkHole.h>
#include <Game\Cards\Magic\Raigeki.h>
#include <Game\Cards\Magic\StopDefense.h>
#include <Game\Cards\Magic\TypeDestroyer.h>
#include <Game\Cards\Magic\HealMagic.h>
#include <Game\Cards\Magic\DamageMagic.h>
#include <Game\Cards\Magic\DarkLight.h>
#include <Game\Cards\Magic\Curse.h>
#include <Game\Cards\Magic\Harpie.h>
#include <Game\Cards\Magic\SwordsMagic.h>
#include <Game\Cards\Magic\BlankMagic.h>
#include <Game\Cards\Magic\Sogen.h>
#include <Game\Cards\Magic\Forest.h>
#include <Game\Cards\Magic\Umi.h>
#include <Game\Cards\Magic\Wasteland.h>
#include <Game\Cards\Magic\Yami.h>
#include <Game\Cards\Magic\Mountain.h>


namespace Card{

	MagicUnit MagicUnit::magicUnitInstance;

	void MagicUnit::startup(){
		hasCard = false;
		amtranUpdating = 0.0f;
		currentAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		chain = YUG_MAG_CH_IDLE;
	}
	void MagicUnit::cleanup(){

	}

	void MagicUnit::update(){
		if(amtranUpdating){
			continueInterpTextAmtran();
		}
		if(hasCard){
			magicCard->bigRender.update();
		}
		if(!isWaiting){
			switch(chain){
			case YUG_MAG_CH_SMALLUP:
				smallUpUpdate();
				break;
			case YUG_MAG_CH_SMALLROT:
				smallRotUpdate();
				break;
			case YUG_MAG_CH_BIGROT:
				bigRotUpdate();
				break;
			case YUG_MAG_CH_PHASEFOR:
				phaseForUpdate();
				break;
			case YUG_MAG_CH_SPECIFIC:
				callCorrectUpdate();
				break;
			case YUG_MAG_CH_SPECIFC_FINISHED:
				deleteUpdate();
				break;
			case YUG_MAG_CH_RETURN:
				returnUpdate();
				break;
			case YUG_MAG_CH_END:
				endUpdate();
				break;
			case YUG_MAG_CH_TRAP1:
				trapUpdate1();
			case YUG_MAG_CH_TRAP2:
				trapUpdate2();
			default:
				break;
			}
		}else{
			continueWaiting();
		}

	}

	void MagicUnit::render(){
		if(hasCard){
			magicCard->bigRender.render();
		}
		if(chain == YUG_MAG_CH_SPECIFIC){
			callCorrectRender();
		}
	}
	void MagicUnit::input(){//mightn't be used
		if(chain == YUG_MAG_CH_SPECIFIC){
			callCorrectInput();
		}
	}

	//basic animation

	void MagicUnit::interpolateSelfUp(float duration){
		if(theBoard.playerControlling()){
			magicCard->smallRender.interpolate(pos.fusCentre,duration);
		}else{
			magicCard->smallRender.interpolate(pos.fusEnCentre,duration);
		}
	}
	void MagicUnit::rotateSelfUp(float duration){
		if(theBoard.playerControlling()){
			magicCard->smallRender.rotate(pos.atkFaceupUp, duration);
		}else{
			magicCard->smallRender.rotate(pos.eAtkFaceupUp, duration);
		}
	}
	void MagicUnit::setUpBigCard(){
		magicCard->bigRender.cleanup();
		magicCard->bigRender.startup();
		magicCard->bigRender.doRender = false;
		magicCard->bigRender.ignoreCamera = false;
		magicCard->bigRender.scale = pos.fusBigCardScale;
		if(theBoard.playerControlling()){
			magicCard->bigRender.rotationMatrix = pos.cardPBackLeft;
			magicCard->bigRender.position = pos.fusCentre;
		}else{
			magicCard->bigRender.rotationMatrix = pos.cardEBackLeft;
			magicCard->bigRender.position = pos.fusEnCentre;
		}
	}

	void MagicUnit::cardReveal1(float duration){
		if(theBoard.playerControlling()){
			magicCard->smallRender.rotate(pos.cardPBackRight,duration);
		}else{
			magicCard->smallRender.rotate(pos.cardEBackRight,duration);
		}
		wait(duration);
	}
	void MagicUnit::cardReveal2(float duration){
		magicCard->smallRender.doRender = false;
		magicCard->bigRender.doRender = true;
		if(theBoard.playerControlling()){
			magicCard->bigRender.rotate(pos.atkFaceupUp,duration);
		}else{
			magicCard->bigRender.rotate(pos.eAtkFaceupUp,duration);
		}
		wait(duration+0.3f);
	}
	void MagicUnit::phaseForwards(float duration){
		float z = (theBoard.playerControlling()?0.5f:-0.5f);
		magicCard->bigRender.interpolate(mov.addZOffset(magicCard->bigRender.position, z),duration);
		magicCard->bigRender.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		magicCard->bigRender.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),duration);
		wait(duration);
	}
	//updates
	void MagicUnit::startUpdate(int info){
		chain = YUG_MAG_CH_SMALLUP;
		source = info;
		if(theBoard.playerControlling()){
			endUnit.noOfMagicsUsed++;
		}
		if(info == YUG_MAG_BOARD){
			copyCardFromBoard();
		}if(info == YUG_MAG_POSITION){
			copyCardFromPosition();
		}if(info == YUG_MAG_HAND){
			copyCardFromPosition();
		}
		theBoard.chain = YUG_BOARD_CH_IDLE;
		theBoard.underlay.hide();
		theBoard.battleCursor.turnoff();
		theBoard.cameraToPlayer();
		theBoard.currentPlayer->moveCardsToHiddenPosition();
		interpolateSelfUp();
		rotateSelfUp();
		wait(0.3f);

	}

	void MagicUnit::deleteUpdate(){
		magicSource->cleanup();
		delete magicSource;
		chain = YUG_MAG_CH_RETURN;
		hasCard = false;
	}

	void MagicUnit::smallUpUpdate(){
		setUpBigCard();
		cardReveal1();
		chain = YUG_MAG_CH_SMALLROT;
	}
	void MagicUnit::smallRotUpdate(){
		cardReveal2();
		chain = YUG_MAG_CH_BIGROT;
		wait(0.5f);
	}
	void MagicUnit::bigRotUpdate(){
		phaseForwards(0.3f);
		chain = YUG_MAG_CH_PHASEFOR;
		wait(0.5f);
	}
	void MagicUnit::phaseForUpdate(){
		dontRenderEitherParts();
		callCorrectStartup();
		removeMagicCard();
		hasCard = false;
		chain = YUG_MAG_CH_SPECIFIC;
		//std::cout<<"Mag: is it blank yet?\n";
		if(trapUnit.enemyListen(YUG_TRAP_MAGIC_ACTIVATE)){
			chain = YUG_MAG_CH_IDLE;
			trapUnit.start(YUG_TRAP_MAGIC_ACTIVATE);
		}
		wait(0.2f);
	}
	void MagicUnit::returnUpdate(){
		if(source != YUG_MAG_BOARD){
			theBoard.playerTurnStartPos();
		}
		if(YUG_END_CONTINUE_DUEL==endUnit.checkLifepoints())
			theBoard.currentBoardView(0.2f);
		chain = YUG_MAG_CH_END;
		wait(0.2f);
	}
	void MagicUnit::endUpdate(){
		chain = YUG_MAG_CH_IDLE;
		int gameStatus = endUnit.checkLifepoints();
		 if(gameStatus == YUG_END_PLAYER_LOST){
			 endUnit.playerLost();
		 }else if(gameStatus == YUG_END_PLAYER_WIN){
			 endUnit.playerWin();
		 }else{
			theBoard.chain = YUG_BOARD_CH_PLAYERTURN;
			theBoard.underlay.viewingBoard();
			if(source != YUG_MAG_BOARD){
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

	void MagicUnit::trapUpdate1(){
		deleteUpdate();
		chain = YUG_MAG_CH_IDLE;
	}
	void MagicUnit::trapUpdate2(){
	}

	//utility
	void MagicUnit::dontRenderEitherParts(){
		magicCard->smallRender.doRender = false;
		magicCard->bigRender.doRender = false;
	}
	void MagicUnit::copyCardFromBoard(){
		//std::cout<<"MG U: cP[0] = "<<theBoard.cP[0]<<" cP[1] = "<<theBoard.cP[1]<<std::endl;
		hasCard = true;
		oldRow = theBoard.cP[1];
		oldColumn = theBoard.cP[0];
		magicCard = &(theBoard.board[theBoard.cP[0]][theBoard.cP[1]]);
		cardNo = magicCard->cardNumber;
		getCorrectMagicSource();
	}

	void MagicUnit::copyCardFromPosition(){
		hasCard = true;
		//std::cout<<"magic: copying card: "<<positionUnit.card.name.data()<<std::endl;
		if(positionUnit.card.name[0] == 'E' && positionUnit.card.name[1] == 'l'){//elegant egotist
			positionUnit.card.cardNumber = 828;
		}
		if(theBoard.playerControlling()){
			endUnit.noOfMagicsPlayed++;
		}
		magicCard = &(positionUnit.card);
		cardNo = magicCard->cardNumber;
		getCorrectMagicSource();
	}

	void MagicUnit::copyCardFromHand(){
		hasCard = true;
		magicCard = &(theBoard.currentPlayer->hand[theBoard.currentPlayer->cursorPosition]);
		cardNo = magicCard->cardNumber;
		getCorrectMagicSource();

	}

	void MagicUnit::removeMagicCard(){
		hasCard = false;
		if(source == YUG_MAG_BOARD){
			theBoard.board[oldColumn][oldRow].cleanup();
			theBoard.board[oldColumn][oldRow] = cardCreator.blankCard();
		}
		if(source == YUG_MAG_POSITION || source == YUG_MAG_HAND){
			positionUnit.card.cleanup();
			positionUnit.card = cardCreator.blankCard();
		}
	}

	//text stuff
	void MagicUnit::interpTextAmtran(glm::vec4 endAmtran, float duration){
		
		startingAmtran = currentAmtran;
		endingAmtran = endAmtran;
		amtranSpeed = 1.0f/duration;
		currentAmtranDuration = 0.0f;
		amtranUpdating = true;
	}
	void MagicUnit::continueInterpTextAmtran(){
		currentAmtranDuration += (amtranSpeed*gameClock.lastLoopTime());
		if(currentAmtranDuration < 1.0f){
			currentAmtran = glm::mix(
				startingAmtran, 
				endingAmtran, 
				currentAmtranDuration);
			textPrinter.magicAmtran = currentAmtran;
		}else{
			currentAmtran = endingAmtran;
			textPrinter.magicAmtran = currentAmtran;
			amtranUpdating = false;
		}
	}

	//card finding
	std::vector<CardData*> MagicUnit::returnAllPlayerMonsters(){
		std::vector<CardData*>monsters;
		int j = (theBoard.playerControlling()?YUG_BOARD_PLAYER_MON_ROW:YUG_BOARD_ENEMY_MON_ROW);
		for(unsigned int i =0 ;i < 5; i++){
			if(!theBoard.board[i][j].blankCard()){
				monsters.push_back(&(theBoard.board[i][j]));
			}
		}
		return monsters;
	}
	std::vector<CardData*> MagicUnit::returnAllEnemyMonsters(){
		std::vector<CardData*>monsters;
		int j = (theBoard.playerControlling()?YUG_BOARD_ENEMY_MON_ROW:YUG_BOARD_PLAYER_MON_ROW);
		for(unsigned int i =0 ;i < 5; i++){
			if(!theBoard.board[i][j].blankCard()){
				monsters.push_back(&(theBoard.board[i][j]));
			}
		}
		return monsters;
	}


	glm::vec3 MagicUnit::getCorrectFusCentre(){
		if(theBoard.playerControlling()){
			return pos.fusCentre;
		}else{
			return pos.fusEnCentre;
		}
	}

	void MagicUnit::getCorrectMagicSource(){
		//std::cout<<"Magic source choosing "<<cardNo<<std::endl;
		switch(cardNo){
		case 338: magicSource = new Sogen(); break;

		case 779: magicSource = new ChangeOfHeart(); break;
		case 781: magicSource = new GracefulDice(); break;
		case 782: magicSource = new SkullDice(); break;
		case 801: magicSource = new  StopDefense(); break;
		case 802: magicSource = new  SpellCircle(); break;
		case 803: magicSource = new  DarkLight(); break;
		case 804: magicSource = new  DarkHole(); break;//??
		case 805: magicSource = new  Raigeki(); break;//??
		case 806: magicSource = new  ShadowSpell(); break;
		case 807: magicSource = new  Harpie(); break;
		case 808: magicSource = new  Curse(); break;
		case 838: magicSource = new SwordsMagic(); break;

		case 809: magicSource = new  TypeDestroyer(11 ,glm::vec4(1.0f,0.7f,0.5f,1.0f)); break;
		case 810: magicSource = new  TypeDestroyer(14 ,glm::vec4(1.0f,0.2f,0.2f,1.0f)); break;
		case 811: magicSource = new  TypeDestroyer(13 ,glm::vec4(0.2f,1.0f,0.7f,1.0f)); break;
		case 812: magicSource = new  TypeDestroyer(25 ,glm::vec4(0.3f,0.3f,1.0f,1.0f)); break;
		case 813: magicSource = new  TypeDestroyer(20 ,glm::vec4(0.6f,1.0f,0.6f,1.0f)); break;
		case 814: magicSource = new  TypeDestroyer(29 ,glm::vec4(0.7f,0.7f,1.0f,1.0f)); break;
		case 815: magicSource = new  TypeDestroyer(23 ,glm::vec4(1.0f,1.0f,0.0f,1.0f)); break;
		case 816: magicSource = new  TypeDestroyer(0 ,glm::vec4(1.0f,0.5f,1.0f,1.0f)); break;

		case 817: magicSource = new HealMagic( 200 ); break;
		case 818: magicSource = new HealMagic( 500 ); break;
		case 819: magicSource = new HealMagic( 1000 ); break;
		case 820: magicSource = new HealMagic( 2000 ); break;
		case 821: magicSource = new HealMagic( 5000 ); break;

		case 822: magicSource = new DamageMagic( 50 ); break;
		case 823: magicSource = new DamageMagic( 200 ); break;
		case 824: magicSource = new DamageMagic( 500 ); break;
		case 825: magicSource = new DamageMagic( 800 ); break;
		case 826: magicSource = new DamageMagic( 1000 ); break;

		case 839: magicSource = new Forest(); break;
		case 840: magicSource = new Wasteland(); break;
		case 841: magicSource = new Mountain(); break;
		case 842: magicSource = new Umi(); break;
		case 843: magicSource = new Yami(); break;

		default: magicSource = new BlankMagic(); break;
		}
	}
	void MagicUnit::callCorrectStartup(){
		magicSource->startup();
	}
	void MagicUnit::callCorrectRender(){
		magicSource->render();
	}
	void MagicUnit::callCorrectUpdate(){
		magicSource->update();
	}
	void MagicUnit::callCorrectInput(){
		magicSource->input();
	}
	void MagicUnit::callCorrectCleanup(){
		magicSource->cleanup();
	}

}