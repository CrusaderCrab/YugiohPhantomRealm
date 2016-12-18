#include <string>
#include <sstream>
#include <Screens\FreeDuelScreen.h>
#include <Game\PlayerData.h>
#include <Game\EnemyData.h>
#include <Utility\InputUnit.h>
#include <Screens\Trunk.h>
#include <Screens\ScreenUnit.h>
#include <Screens\ShopScreen.h>
#include <Utility\TextPrinter.h>
#include <Utility\StateUnit.h>
#include <DefinesAndTypedefs.h>
#include <Game\Duel\Board.h>
#include <Utility\SoundUnit.h>
#include <iostream>

#define ZYUG_CH_IDLE 0
#define ZYUG_CH_START 1
#define ZYUG_CH_ACTIVE 2
#define ZYUG_CH_END 3
#define ZYUG_A
#define ZYUG_B
#define ZYUG_C
#define ZYUG_D

namespace Screen{

	const char* names[] = {
		"Simon", "Teana", "Jono", "Miko Jr", "Guard", "Ramma",
		"Weevil", "Rex", "Mai", "Osir", "Shadi", "Bandit Keith", 
		"Bakura", "Setha", "Isis", "Pegasus", "Low Forest Mage", "High Forest Mage",
		"Low Desert Mage", "High Desert Mage", "Low Ocean Mage", "High Ocean Mage", "Low Field Mage",
		"High Field Mage", "Low Mount. Mage", "High Mount. Mage", "Magic Mage",
		"Heishin", "Seto", "DarkNite", "Master C", "Simon 2", "The Shopkeeper", "Master K" 
	}; 

	/*const int fdMap[] = {
		1,2,3,4,5,
		8,7,6,9,11,13,10,12,14,15,16,
		17,18,19,20,21,22,23,24,
		25,26,27,28,29,30,31
	};*/

	void FreeDuelScreen::startup(const char* fileLocal){
		startup();
	}

	void FreeDuelScreen::startup(){
		BaseScreen::startup();
		chain = ZYUG_CH_START;
		vertPos = stateUnit.freeDuelVerts;
		horzPos = stateUnit.freeDuelHorz;
		screenLevel = vertPos; if(screenLevel>4)screenLevel=4;
		setupBG();
		setupCursor();
		setupScrollBar();
		setupPictures();
		updateCursor();
		updateBar();
		updatePictures();
		soundUnit.playLoop("GameData/sounds/music/ready/freeDuelMenu_o.wav");
		theBoard.leadInLength = 1.724f;
		theBoard.leadInTrack = "GameData/sounds/music/ready/freeDuel_s.wav";
		theBoard.mainTrack = "GameData/sounds/music/ready/freeDuel_l.wav";
	}

	void FreeDuelScreen::cleanup(){
		bg.cleanup();
		cursor.cleanup();
		scrollBar.cleanup();
		for(int i = 0; i < 35; i++) pics[i].cleanup();//i < 32
		soundUnit.stopAll();
	}

	void FreeDuelScreen::update(){
		if(!isWaiting){
			switch(chain){
			case ZYUG_CH_START: firstUpdate(); break;
			case ZYUG_CH_END: finalUpdate(); break;
			default: break;
			}
		}else{
			continueWaiting();
		}
	}

	void FreeDuelScreen::finalUpdate(){
		//std::cout<<"FreeDuel: finalUpdate\n";
		chain = ZYUG_CH_IDLE;
		toNextScreen();
	}

	void FreeDuelScreen::firstUpdate(){
		beginScreen(0.5f);
		chain = ZYUG_CH_ACTIVE;
		wait(0.55f);
	}

	void FreeDuelScreen::input(){
		if(!isWaiting){
			if(inputUnit.isKeyActive(YUG_KEY_X)){xPressed(); wait(1.0f);}
			if(inputUnit.isKeyActive(YUG_KEY_O)){oPressed(); wait(1.0f);}
			if(inputUnit.isKeyActive(YUG_KEY_DOWN)){directionPressed(0,1);wait(0.2f);}
			if(inputUnit.isKeyActive(YUG_KEY_UP)){directionPressed(0,-1);wait(0.2f);}
			if(inputUnit.isKeyActive(YUG_KEY_LEFT)){directionPressed(-1,0);wait(0.2f);}
			if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){directionPressed(1,0);wait(0.2f);}
			if(inputUnit.isKeyActive(YUG_KEY_START)){
				if(playerData.currentSaveFile==YUG_PLAYER_DATA_NO_ACTIVE_SAVE_FILE){
					soundUnit.cursorSelectionFail();
					std::cout<<"Save failed: no chosen file. Please save or load a file\n"<<
						"in the shop to choose one\n";
				}else{
					soundUnit.cursorSelect();
					playerData.saveGame(playerData.currentSaveFile);
					std::cout<<"Saving over file "<<playerData.currentSaveFile<<"\nSave Successful\n";
				}
				wait(0.2f);
			}
		}
	}

	void FreeDuelScreen::toNextScreen(){
		if(miscInfo=='r' || miscInfo=='f'){
			//fadeIn(0.2f);
			cleanup();
			std::stringstream ss;
			ss<<miscInfo;
			screenUnit.startUpComingScreen(ss.str().c_str());
			//std::cout<<"Freduel:: toNext\n";
		}else{
			cleanup();
			screenUnit.startUpComingScreen(" ");
			//std::cout<<"Freduel:: toNext2\n";
		}
	}

	void FreeDuelScreen::xPressed(){
		int posi = (vertPos*5+horzPos)-1;
		if(posi==-1){
			soundUnit.cursorSelect();
			screenUnit.comingScreen = &trunkUnit;
			miscInfo = 'r';
			chain = ZYUG_CH_END;
			fadeOut(0.18f);
			wait(0.42f);
			stateUnit.freeDuelHorz = horzPos;
			stateUnit.freeDuelVerts = vertPos;
		}else if(posi<33 && playerData.unlockedDuelists[posi].hasUnlocked){//posi<30
			soundUnit.cursorSelect();
			screenUnit.comingScreen = &trunkUnit;
			std::stringstream ss;
			ss<<"GameData/enemies/"<<posi+1<<".txt";
			enemyData.loadData(ss.str() );
			miscInfo = 'f';
			trunkUnit.fieldType = 0;
			chain = ZYUG_CH_END;
			fadeOut(0.18f);
			wait(0.42f);
			stateUnit.freeDuelHorz = horzPos;
			stateUnit.freeDuelVerts = vertPos;
		}else if(posi==33){//posi==30
			soundUnit.cursorSelect();
			screenUnit.comingScreen = &trunkUnit;
			std::stringstream ss;
			ss<<"GameData/enemies/"<<1<<".txt";
			enemyData.loadData(ss.str() );
			enemyData.aiNumber = 3;
			enemyData.loadCorrespondingAI();
			for(int i = 0; i < 40; i++){
				enemyData.deck.internalDeck[i] = playerData.deck.getNoAt(i);
			}
			miscInfo = 'f';
			trunkUnit.fieldType = 0;
			chain = ZYUG_CH_END;
			fadeOut(0.18f);
			wait(0.42f);
			stateUnit.freeDuelHorz = horzPos;
			stateUnit.freeDuelVerts = vertPos;
		}else{
			soundUnit.cursorSelectionFail();
		}
	}
	void FreeDuelScreen::oPressed(){
		soundUnit.cursorCancel();
		screenUnit.comingScreen = new ShopScreen();
		chain = ZYUG_CH_END;
		miscInfo = 's';
		fadeOut(0.5f);
		wait(5.01f);
		stateUnit.freeDuelHorz = 0;
		stateUnit.freeDuelVerts = 0;
		//std::cout<<"FreeDuel: oPressed\n";
	}
	void FreeDuelScreen::directionPressed(int leftRight, int upDown){
		soundUnit.cursorMove();
		horzPos += leftRight;
		if(horzPos<0)horzPos=0; if(horzPos>4)horzPos=4;
		vertPos += upDown;
		if(vertPos<0)vertPos=0; if(vertPos>6)vertPos=6;
		if(vertPos-2>screenLevel) screenLevel=vertPos-2;
		if(vertPos<screenLevel) screenLevel=vertPos;
		if(4<screenLevel) screenLevel=4;

		updateCursor();
		updateBar();
		updatePictures();
	}

	void FreeDuelScreen::render(){
		bg.render();
		renderPictures();
		scrollBar.render();
		cursor.render();
		renderText();
	}

	void FreeDuelScreen::renderText(){
		int posi = vertPos*5+horzPos;
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;
		if(posi==0){
			textPrinter.printText("Edit Deck",YUG_TEXT_INFO_FONT, 
				glm::vec3(0.3f, 0.7f, 1), glm::vec3(-0.7f, -0.495f, -1.989f),
				glm::mat4());
		}else if(posi<35 && playerData.unlockedDuelists[posi-1].hasUnlocked){//posi<32
			std::stringstream ss;
			ss<<"Wins "<<playerData.unlockedDuelists[posi-1].winNumber<<
				" Loses "<<playerData.unlockedDuelists[posi-1].loseNumber;
			textPrinter.printText(names[posi-1],YUG_TEXT_INFO_FONT, 
				glm::vec3(0.3f, 0.7f, 1), glm::vec3(-0.7f, -0.495f, -1.989f),
				glm::mat4());
			textPrinter.printText(ss.str().c_str(),YUG_TEXT_INFO_FONT, 
				glm::vec3(0.3f, 0.7f, 1), glm::vec3(0.1f, -0.495f, -1.989f),
				glm::mat4());
		}
		textPrinter.ignoreCamera = false;
	}

	void FreeDuelScreen::renderPictures(){
		int start = screenLevel*5;
		int end = (start+15<35? start+15 : 35);//(start+15<32? start+15 : 32)
		for(int i = start; i < end; i++){
			pics[i].render();
		}
	}

	void FreeDuelScreen::updatePictures(){
		int start = screenLevel*5;
		int end = (start+15<35? start+15 : 35);//(start+15<32? start+15 : 32);
		glm::vec3 topLeft = glm::vec3(-0.57f, 0.27f, -1.99f);
		for(int i = start; i < end; i++){
			int j = i-start;
			glm::vec3 posi = topLeft + glm::vec3(
				(j%5)*0.28f, (j/5)*-0.27f, 0);
			pics[i].position = posi;
		}
	}

	void FreeDuelScreen::updateBar(){
		glm::vec3 posi = glm::vec3(0.721f, 0.285f, -1.987f);
		posi.y -= screenLevel*0.135f;
		scrollBar.position = posi;
	}

	void FreeDuelScreen::updateCursor(){
		int vert = vertPos-screenLevel;
		glm::vec3 topLeft = glm::vec3(-0.57f, 0.27f, -1.985f);
		topLeft.x += horzPos*0.28f;
		topLeft.y -= vert*0.27f;
		cursor.position = topLeft;
	}

	void FreeDuelScreen::setupBG(){
		bg.startup(
			YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/singles/freeduelbg.png");
		bg.doRender = true;
		bg.ignoreCamera = true;
		bg.scale = glm::vec3(0.83f, 0.65f, 1);
		bg.position = glm::vec3(0.0f, 0.00f, -2.0f);
	}

	void FreeDuelScreen::setupCursor(){
		cursor.startup(
			YUG_PLANE_FILE_PATH,
			"GameData/textures/screens/singles/freeduelcursor.png");
		cursor.doRender = true;
		cursor.ignoreCamera = true;
		cursor.scale = glm::vec3(0.13f, 0.13f, 1);
	}

	void FreeDuelScreen::setupScrollBar(){
		scrollBar.startup(
			YUG_PLANE_FILE_PATH,
			"GameData/textures/screens/singles/freeduelbar.png");
		scrollBar.doRender = true;
		scrollBar.ignoreCamera = true;
		scrollBar.scale = glm::vec3(0.024f, 0.16f, 1);
	}

	void FreeDuelScreen::setupPictures(){
		for(int i = 1; i < YUG_NO_OF_FREE_DUELISTS; i++){
			std::stringstream ss;
			ss<<"GameData/textures/screens/duelpictures/";
			ss<<i;
			ss<<".png";
			//std::cout<<"got pic name: "<<ss.str()<<std::endl;
			pics[i].startup(YUG_PLANE_FILE_PATH,
				ss.str().c_str());
			pics[i].doRender = playerData.unlockedDuelists[i-1].hasUnlocked;
			pics[i].scale = glm::vec3(0.13f,0.13f,1);
		}
		pics[0].startup(YUG_PLANE_FILE_PATH,
				"GameData/textures/screens/duelpictures/0.png");
		pics[0].doRender = true;
		pics[0].scale = glm::vec3(0.13f,0.13f,1);
		pics[34].startup(YUG_PLANE_FILE_PATH,//31
				"GameData/textures/screens/duelpictures/34.png");
		pics[34].doRender = true;
		pics[34].scale = glm::vec3(0.12f,0.12f,1);
	}













}

