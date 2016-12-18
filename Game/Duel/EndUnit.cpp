#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <chrono>
#include <Game\Duel\EndUnit.h>
#include <Game\Duel\Board.h>
#include <Utility\BlankUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\DebugUnit.h>
#include <Game\EnemyData.h>
#include <Game\PlayerData.h>
#include <Game\VectorUnit.h>
#include <Utility\InputUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Screens\ScreenUnit.h>
#include <Utility\StateUnit.h>
#include <Utility\SoundUnit.h>
#define GLM_FORCE_RADIANS
#include <gtc\matrix_transform.hpp>
#include <Screens\StoryScene.h>

#define ZYUG_IDLE 0
#define ZYUG_REVEAL_LETTERS 1
#define ZYUG_FADE_IN 2
#define ZYUG_RESULTS 3
#define ZYUG_FADE_OUT 4
#define ZYUG_PRE_END 6
#define ZYUG_END 5

#define ZYUG_WIN_COLOR glm::vec4(1.0f, 0.3f, 0.3f, 1.0f)
#define ZYUG_WIN_NO_COL glm::vec4(1.0f , 0.3f, 0.3f, 0.0f)
#define ZYUG_LOSE_COLOR glm::vec4(0.3f, 0.3f, 1.0f, 1.0f)
#define ZYUG_LOSE_NO_COL glm::vec4(0.3f , 0.3f, 1.0f, 0.0f)
#define ZYUG_ABC_Z -1.0f
#define ZYUG_ABC_SCALE glm::vec3(0.7f, 0.7f, 1.0f)
#define ZYUG_SHE_Z -1.1f
#define ZYUG_SHE_SCALE glm::vec3(0.5f, 0.5f, 1.0f)
#define ZYUG_CARD_POS glm::vec3(-0.3f, 0.3f, -1.0f)
#define ZYUG_CARD_SCALE glm::vec3(0.5f, 0.5f, 1.0f)
#define ZYUG_RATE_SIZE glm::vec3(0.1f, 0.25f, 1.0f)
#define ZYUG_RL -0.055f
#define ZYUG_RM 0.2f
#define ZYUG_RR 0.28f

namespace Duel{


	EndUnit EndUnit::EndUnitInstance;


	void EndUnit::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_IDLE;
		doRender = false;
		doUpdate = false;
		drawLetters = false;
		//get textures and models
		setupRotationThinker();
		setUpSheet();
		wonCard = cardCreator.blankCard();

		setupRatingValues();
	}
	void EndUnit::cleanup(){
		if(!wonCard.blankCard()){
			wonCard.bigRender.cleanup();
			wonCard.cleanup();
		}
		resultSheet.cleanup();
		bgSheet.cleanup();
		rotationThinker.cleanup();
	}
	void EndUnit::playerWin(){
		//std::cout<<"\n<---PLAYER WIN--->\n";
		takeInput();
		won = true;
		calculateRating();
		getWonCard();
		giveCardToPlayer();
		setupResultSheet();
		setUpCard();
		theBoard.currentPlayerView(1.0f);
		rotationThinker.amtran = ZYUG_WIN_COLOR;
		doRender = true;
		doUpdate = true;
		startUpdate();
		soundUnit.musicCounting = false;
		soundUnit.stopAll();
		soundUnit.playOnce("GameData/sounds/music/ready/winTrack_o.wav");
	}
	void EndUnit::playerLost(){
		//std::cout<<"\n<---PLAYER LOST--->\n";
		won = false;
		takeInput();
		calculateRating();
		getWonCard();
		setupResultSheet();
		setUpCard();
		theBoard.currentPlayerView(1.0f);
		rotationThinker.amtran = ZYUG_LOSE_COLOR;
		doRender = true;
		doUpdate = true;
		startUpdate();
		soundUnit.musicCounting = false;
		soundUnit.stopAll();
		soundUnit.playOnce("GameData/sounds/music/ready/loseTrack_o.wav");
	}
	int EndUnit::checkLifepoints(){
		if(theBoard.player.lifepoints <= 0){
			return YUG_END_PLAYER_LOST;
		}else if(theBoard.enemy.lifepoints <= 0){
			return YUG_END_PLAYER_WIN;
		}
		return YUG_END_CONTINUE_DUEL;
	}
	void EndUnit::playerOutOfCards(){
		theBoard.currentPlayer->currentChainPosition = YUG_GAME_PLAYER_CHAIN_IDLE;
		takeInput();
		if(theBoard.playerControlling()){
			playerLost();
		}else{
			playerWin();
		}

	}
	void EndUnit::takeInput(){
		blankUnit.transitionInput(this, 0.2f);
	}
	void EndUnit::update(){
		if(doUpdate){
			rotationThinker.update();
			bgSheet.update();
			resultSheet.update();
			if(won){
				wonCard.bigRender.update();
			}
			if(!isWaiting){
				switch(chain){
				case ZYUG_REVEAL_LETTERS:
					lettersUpdate(); break;
				case ZYUG_FADE_IN:
					changeFadeUpdate(); break;
				case ZYUG_FADE_OUT:
					fadeoutUpdate(); break;
				case ZYUG_PRE_END:
					preEndUpdate(); break;
				case ZYUG_END:
					endUpdate(); break;
				default:
					break;
				}
			}else{
				continueWaiting();
			}
		}
	}

	void EndUnit::preEndUpdate(){
		fadeUnit.changeZ(-0.7f);
		fadeUnit.fadeTo(glm::vec4(0.0001f,0,0,1),0.5f);
		chain = ZYUG_END;
		wait(0.5f);
	}

	void EndUnit::endUpdate(){
		theBoard.closed = true;
		theBoard.cleanup();
		if(theBoard.miscInfo=='f'){
			//std::cout<<"EndUnit: return to free duel\n";
			screenUnit.comingScreen = screenUnit.winScreen;
			int posi = stateUnit.freeDuelVerts*5+stateUnit.freeDuelHorz-1;
			if(won)playerData.unlockedDuelists[posi].winNumber++;
			else playerData.unlockedDuelists[posi].loseNumber++;
			screenUnit.startUpComingScreen("");
		}else if(theBoard.miscInfo=='d'){
			//std::cout<<"EndUnit: going to win/lose screen\n";
			screenUnit.comingScreen = new Screen::StoryScene();
			if(won){
				screenUnit.startUpComingScreen(screenUnit.winStr.c_str());
			}else{
				screenUnit.startUpComingScreen(screenUnit.loseStr.c_str());
			}
		}
	}

	void EndUnit::startUpdate(){
		//std::cout<<"End Unit: start update\n";
		fadeUnit.changeZ(pos.nc_z[0]);
		inputUnit.playerInputAllowed = true;
		inputUnit.computerInputAllowed = false;
		theBoard.field.hide();
		theBoard.LP.hide();
		theBoard.underlay.hide();
		theBoard.battleCursor.turnoff();
		soundUnit.stopAll();
		wait(2.2f);
		chain = ZYUG_REVEAL_LETTERS;
	}
	void EndUnit::lettersUpdate(){
		//std::cout<<"End Unit: letters update\n";
		rotationThinker.rotate(glm::mat4(), 2.0f);
		chain = ZYUG_FADE_IN;
		drawLetters = true;
		wait(3.5f);
	}
	void EndUnit::changeFadeUpdate(){
		//std::cout<<"End Unit: change Fade Update\n";
		chain = ZYUG_FADE_OUT;
		wait(0.5f);
	}
	void EndUnit::fadeoutUpdate(){
		//std::cout<<"End Unit: fade out update\n";
		float time = 0.5f;
		if(won)
			rotationThinker.interpolateAmtran(glm::vec4(ZYUG_WIN_NO_COL), time/2 );
		else
			rotationThinker.interpolateAmtran(glm::vec4(ZYUG_LOSE_NO_COL), time/2 );
		bgSheet.interpolateAmtran(glm::vec4(1,1,1,1), time);
		bgSheet.doRender = true;
		resultSheet.interpolateAmtran(glm::vec4(1,1,1,1), time);
		resultSheet.doRender = true;
		if(won){
			wonCard.bigRender.interpolateAmtran(glm::vec4(1,1,1,1), time);
			wonCard.bigRender.rotate(glm::mat4(), 2.0f);
			wonCard.bigRender.doRender = true; 
		}
		wait(2.0f);
		chain = ZYUG_RESULTS;
	}

	void EndUnit::render(){
		if(doRender){
			if( drawLetters ){
				textPrinter.ignoreCamera = true;
				textPrinter.leftAlign = true;
				glm::mat4 rot = rotationThinker.rotationMatrix;
				glm::vec4 amtran = rotationThinker.amtran;
				glm::vec4 oldAm = textPrinter.amtran;
				textPrinter.amtran = amtran;
				//debugUnit.printMat4(rot);
				textPrinter.printText("Y", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3(-0.1f,0.1f,-1.0f), rot);
				textPrinter.printText("O", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3( 0.0f,0.1f,-1.0f), rot);
				textPrinter.printText("U", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3( 0.1f,0.1f,-1.0f), rot);
				if(won){
					textPrinter.printText("W", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3(-0.1f,-0.0f,-1.0f), rot);
					textPrinter.printText("I", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3( 0.0f,-0.0f,-1.0f), rot);
					textPrinter.printText("N", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3( 0.1f,-0.0f,-1.0f), rot);
				}else{
					textPrinter.printText("L", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3(-0.15f,-0.0f,-1.0f), rot);
					textPrinter.printText("O", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3(-0.05f,-0.0f,-1.0f), rot);
					textPrinter.printText("S", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3( 0.05f,-0.0f,-1.0f), rot);
					textPrinter.printText("E", YUG_TEXT_INFO_FONT, ZYUG_ABC_SCALE,glm::vec3( 0.15f,-0.0f,-1.0f), rot);
				}
				textPrinter.amtran = oldAm;
				textPrinter.ignoreCamera = false;
			}
			if( chain == ZYUG_RESULTS || chain==ZYUG_FADE_OUT || 
				chain == ZYUG_END || chain == ZYUG_PRE_END){
				bgSheet.render();
				if(bgSheet.doRender){
					renderRatings();
				}
				resultSheet.render();
				if(won){
					wonCard.bigRender.render();
				}
			}
		}
	}
	void EndUnit::input(){
		if(chain == ZYUG_RESULTS && (!isWaiting || currentlyWaited>1.0f) ){
			if(inputUnit.isKeyActive(YUG_KEY_X) 
				|| inputUnit.isKeyActive(YUG_KEY_O)
				|| inputUnit.isKeyActive(YUG_KEY_START)){
				fadeUnit.changeZ(-0.85f);
				fadeUnit.sheet.amtran = glm::vec4(0.001f,0,0,0);
				fadeUnit.fadeTo(glm::vec4(0.001f,0,0,1), 1.0f);
				chain = ZYUG_PRE_END;
				drawLetters = false;
			}
		}
	}

	void EndUnit::calculateRating(){
		//std::cout<<"End: calcing Rating\n";
		powValue = YUG_END_D;
		tecValue = YUG_END_D;
		float toNextPowLevel = 0.0f;
		float toNextTecLevel = 0.0f;
		//pow values
		toNextPowLevel += (noOfAttacks * 0.01f); //std::cout<<"noOfAttacks: "<<toNextPowLevel<<std::endl;
		toNextPowLevel += (noOfSuccessfulAttacks * 0.01f); //std::cout<<"noOfSucessAttacks: "<<toNextPowLevel<<std::endl;
		toNextPowLevel += (enemyCardsDestroyed * 0.01f); //std::cout<<"cards dead: "<<toNextPowLevel<<std::endl;
		if(noOfMonstersPlayed != 0){
			toNextPowLevel += (( ((float)totalAttackValue)/noOfMonstersPlayed) * (1.0f/enemyData.high));
			averAtk = totalAttackValue/noOfMonstersPlayed;
		}
		//std::cout<<"average attack: "<<toNextPowLevel<<std::endl;
		if(noOfTurns < 5)
			toNextPowLevel += 1.5;
		if(noOfTurns < 10)
			toNextPowLevel += 0.5;
		//std::cout<<"noOfTurns: "<<toNextPowLevel<<std::endl;
		toNextPowLevel += theBoard.player.lifepoints*(1.0f/8000.0f);
		//std::cout<<"Leftover lifepoints: "<<toNextPowLevel<<std::endl;

		//tec values
		toNextTecLevel += magicCardDamage * 0.0005f; //std::cout<<"magicDamage: "<<toNextTecLevel<<std::endl;
		toNextTecLevel += magicCardHealing * 0.0005f; //std::cout<<"magicHeal: "<<toNextTecLevel<<std::endl;
		toNextTecLevel += noOfSuccessfulDefenses * 0.05f; //std::cout<<"noOfSuccDefense: "<<toNextTecLevel<<std::endl;
		toNextTecLevel += noOfMagicsPlayed * 0.01f; //std::cout<<"magicsPlayed: "<<toNextTecLevel<<std::endl;
		toNextTecLevel += noOfMagicsUsed * 0.01f; //std::cout<<"magicsUsed: "<<toNextTecLevel<<std::endl;
		if(noOfMonstersPlayed != 0){
			toNextTecLevel += (( ((float)totalDefenseValue)/noOfMonstersPlayed) * (1.0f/enemyData.high));
			averDef = totalDefenseValue/noOfMonstersPlayed;
		}
		//std::cout<<"averDef: "<<toNextTecLevel<<std::endl;
		toNextTecLevel += noOfTrapsPlayed * 0.01f; //std::cout<<"trapsPlayed: "<<toNextTecLevel<<std::endl;
		toNextTecLevel += trapsUsed * 0.01f; //std::cout<<"trapsUsed: "<<toNextTecLevel<<std::endl;
		if(theBoard.enemy.deck.cardsLeft < 11 )
			toNextTecLevel += 1;
		if(theBoard.enemy.deck.cardsLeft < 2 )
			toNextTecLevel += 5;
		if(theBoard.enemy.deck.cardsLeft == 0)
			toNextTecLevel += 15;
		//std::cout<<"deck left: "<<toNextTecLevel<<std::endl;

		for(float i = 1; i < toNextPowLevel ; i++)
			powValue++;
		for(float i = 1; i < toNextTecLevel ; i++)
			tecValue++;
		//std::cout<<" tecValue = "<<tecValue<<" powValue = "<<powValue<<std::endl;

		if( powValue < tecValue){
			resultValue = tecValue;
			pow = false;
		}else{
			resultValue = powValue;
			pow = true;
		}
	
	}
	char EndUnit::getMarkingChar(){
		if(pow){
			switch (resultValue)
			{
			case YUG_END_D: return 'D';
			case YUG_END_C: return 'C';
			case YUG_END_B: return 'B';
			case YUG_END_A: return 'A';
			default: return 'S';
			}
		}else{
			switch (resultValue)
			{
			case YUG_END_D: return 'd';
			case YUG_END_C: return 'c';
			case YUG_END_B: return 'b';
			case YUG_END_A: return 'a';
			default: return 's';
			}
		}
	}
	void EndUnit::getWonCard(){
		//std::cout<<"End Unit: get won card\n";
		if(!won){
			//std::cout<<"\tDidn't win\n";
			wonCard = cardCreator.blankCard();
			return;
		}
		char mark = getMarkingChar();
		//std::cout<<"\tmarking char: "<<mark<<std::endl;
		std::ostringstream ss;
		ss<<"GameData/enemies/"<<enemyData.enemyNumber<<"rewards.txt";
		//std::cout<<"\tstring created: "<<ss.str()<<std::endl;
		std::ifstream input(ss.str().c_str());
		waitForSymbol(mark, &input);
		int cardNo = -1;
		while(cardNo != 0){
			(input)>>cardNo;
			if(cardNo != 0)
				rewardCards.push_back(cardNo);
		}
		std::srand(time(NULL));
		int index = 0; int loopCount = 0;
		while(true){
			index = rand()%rewardCards.size();
			if((playerData.trunk[rewardCards[index]]+countDeckAmount(rewardCards[index]) )< 3 
				|| loopCount > 15){
				break;
			}
			loopCount++;
		}
		//std::cout<<"\tcardSize: "<<rewardCards.size()<<" index: "<<index<<std::endl;
		wonCard = cardCreator.createCard(rewardCards[index]);
		//std::cout<<"\tCreated card: "<<wonCard.name.data()<<std::endl;
		rewardCards.clear();
	}
	void EndUnit::waitForSymbol(char mark, std::ifstream* in){
		char ch;
		while(true){
			ch = in->get();
			if(ch == mark || in->eof() ){
				return;
			}
		}
	}
	int EndUnit::countDeckAmount(int cardNo){
		int count = 0;
		for(unsigned int i = 0; i < playerData.deck.internalDeck.size(); i++){
			if(cardNo == playerData.deck.getNoAt(i)){
				count++;
			}
		}
		return count;
	}
	void EndUnit::giveCardToPlayer(){
		if(!wonCard.blankCard()){
			//std::cout<<"EndUnit::gCTP: "<<wonCard.cardNumber<<" added to trunk\n";
			playerData.trunk[(wonCard.cardNumber-100)]++;

		}
	}

	void EndUnit::setupRatingValues(){
		//rating calculation values
		noOfMagicsPlayed = 0;//
		noOfMagicsUsed = 0;//
		noOfAttacks = 0;//
		noOfSuccessfulAttacks = 0;//
		noOfSuccessfulDefenses = 0;//
		noOfTurns = 0;//
		enemyCardsDestroyed = 0;
		noOfTrapsPlayed = 0;//
		trapsUsed = 0;//
		totalAttackValue = 0;//
		totalDefenseValue = 0;//
		noOfMonstersPlayed = 0;//
		magicCardDamage = 0;
		magicCardHealing = 0;
		averAtk = 0;
		averDef = 0;
	}
	void EndUnit::setUpSheet(){
		bgSheet.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/EndCards/resultpage2.png");
		bgSheet.doRender = false;
		bgSheet.ignoreCamera = true;
		bgSheet.amtran = glm::vec4(1,1,1,0);
		bgSheet.scale = glm::vec3(0.225f, 0.3f, 1.0f);
		bgSheet.position = glm::vec3(0.14f, 0.0f, -0.9f);
	}
	void EndUnit::setUpCard(){
		if(!wonCard.blankCard()){
			cardCreator.setupParents(&wonCard);
			wonCard.bigRender.startup();
			wonCard.bigRender.ignoreCamera = true;
			wonCard.bigRender.scale = glm::vec3(0.28f,0.28f,1.0f);
			wonCard.bigRender.position = glm::vec3(-0.225f, 0.085f,-0.899f);
			wonCard.bigRender.doRender = false;
			wonCard.bigRender.amtran = glm::vec4(1,1,1,0);
			wonCard.bigRender.rotationMatrix = pos.atkFacedownUp;
		}else{
			wonCard.bigRender.doRender = false;
		}
	}
	void EndUnit::setupRotationThinker(){
		rotationThinker.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/littleShield.png");
		rotationThinker.doRender = false;
		rotationThinker.rotationMatrix = glm::rotate(glm::mat4(),(YUG_PI/2.0f),glm::vec3(0,1,0));
		//std::cout<<"End Unit: letter start Rot matrix\n";
		debugUnit.printMat4(rotationThinker.rotationMatrix);
	}

	void EndUnit::setupResultSheet(){
		resultSheet.startup(YUG_PLANE_FILE_PATH, 
			getResultTexturePath().c_str());
		resultSheet.doRender = false;
		resultSheet.ignoreCamera = true;
		resultSheet.amtran = glm::vec4(1,1,1,0);
		resultSheet.scale = glm::vec3(0.15f, 0.075f, 1.0f);
		resultSheet.position = glm::vec3(-0.225f, -0.21f, -0.897f);
	}

	void EndUnit::renderRatings(){
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;
		glm::vec4 amtran = bgSheet.amtran;
		glm::vec4 oldAm = textPrinter.amtran;
		textPrinter.amtran = amtran;
				
		textPrinter.printText("Lifepoints Left",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, 0.198f, -0.896f), glm::mat4());
		textPrinter.printNumber(theBoard.player.lifepoints, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RM, 0.198f, -0.896f), glm::mat4());
		textPrinter.printNumber(theBoard.enemy.lifepoints, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, 0.198f, -0.896f), glm::mat4());

		textPrinter.printText("Cards Left",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, 0.14f, -0.896f), glm::mat4());
		textPrinter.printNumber(theBoard.player.deck.cardsLeft, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RM, 0.142f, -0.896f), glm::mat4());
		textPrinter.printNumber(theBoard.enemy.deck.cardsLeft, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, 0.142f, -0.896f), glm::mat4());

		textPrinter.printText("Turns passed",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, 0.086f, -0.896f), glm::mat4());
		textPrinter.printNumber(noOfTurns, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, 0.086f, -0.896f), glm::mat4());

		textPrinter.printText("No of attacks",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, 0.034f, -0.896f), glm::mat4());
		textPrinter.printNumber(noOfAttacks, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, 0.034f, -0.896f), glm::mat4());
		textPrinter.printText("Successful attacks",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, -0.021f, -0.896f), glm::mat4());
		textPrinter.printNumber(noOfSuccessfulAttacks, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, -0.021f, -0.896f), glm::mat4());

		textPrinter.printText("Average Attack",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, -0.078f, -0.896f), glm::mat4());
		textPrinter.printNumber(averAtk, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, -0.078f, -0.896f), glm::mat4());
		textPrinter.printText("Average Defense",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, -0.135f, -0.896f), glm::mat4());
		textPrinter.printNumber(averDef, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, -0.135f, -0.896f), glm::mat4());

		textPrinter.printText("Magics used",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, -0.19f, -0.896f), glm::mat4());
		textPrinter.printNumber(noOfMagicsUsed, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, -0.19f, -0.896f), glm::mat4());
		textPrinter.printText("Traps used",YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RL, -0.245f, -0.896f), glm::mat4());
		textPrinter.printNumber(trapsUsed, YUG_TEXT_INFO_FONT,ZYUG_RATE_SIZE,glm::vec3(ZYUG_RR, -0.245f, -0.896f), glm::mat4());

		textPrinter.amtran = oldAm;
		textPrinter.ignoreCamera = false;
	}

	std::string EndUnit::getResultTexturePath(){
		std::ostringstream ss;
		ss<<"GameData/textures/EndCards/";
		if(!won){
			ss<<"loser.png";
		}else if(pow){
			switch (resultValue)
			{
			case YUG_END_D: ss<<"dpow.png"; break;
			case YUG_END_C: ss<<"cpow.png"; break;
			case YUG_END_B: ss<<"bpow.png"; break;
			case YUG_END_A: ss<<"apow.png"; break;
			default: ss<<"spow.png"; break;
			}
		}else{
			switch (resultValue)
			{
			case YUG_END_D: ss<<"dtec.png"; break;
			case YUG_END_C: ss<<"ctec.png"; break;
			case YUG_END_B: ss<<"btec.png"; break;
			case YUG_END_A: ss<<"atec.png"; break;
			default: ss<<"stec.png"; break;
			}
		}
		return ss.str();
	}

}