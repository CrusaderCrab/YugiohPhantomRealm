#include <Screens\DebugScreen.h>
#include <Game\Duel\Board.h>
#include <Game\Cursor.h>
#include <Utility\BlankUnit.h>
#include <Utility\StateUnit.h>
#include <Utility\UpdateUnit.h>
#include <Base\RenderUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Utility\InputUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\PlayerData.h>
#include <Game\EnemyData.h>
#include <Screens\Trunk.h>
#include <Game\Duel\EndUnit.h>
#include <iostream>

#include <Game\Cards\Magic\Forest.h>
#include <Game\Cards\Magic\Sogen.h>
#include <Game\Cards\Magic\Wasteland.h>
#include <Game\Cards\Magic\Mountain.h>
#include <Game\Cards\Magic\Umi.h>
#include <Game\Cards\Magic\Yami.h>

#include <Screens\DialogScreen.h>
#include <Screens\SplashScreen.h>
#include <Screens\ScreenUnit.h>
#include <Screens\MainMenu.h>

#include <GL\glew.h>
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

namespace Screen{

	DebugScreen DebugScreen::debugScreenInstance;

	void DebugScreen::toThisScreen(){
		startup();
	}

	void DebugScreen::startup(){
		Game::WaitUnit::startup();
		updateUnit.newUpdater(this);
		stateUnit.renderer->newRenderer(this);
		blankUnit.transitionInput(this, 0.5f);
		fadeUnit.fadeTo(glm::vec4(0.0001f, 0.0f, 0.0f, 0.0f), 0.5f);
		cursorPosition = 0;
		blankBoard = false;
		setupCursor();
	}

	void DebugScreen::cleanup(){

	}

	void DebugScreen::input(){
		if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.input();
		}else
		if(!isWaiting){
			if(inputUnit.isKeyActive(YUG_KEY_X)){
				xPressed();
			}else if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
				moveCursorDown();
			}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
				moveCursorUp();
			}else if(inputUnit.isKeyActive(YUG_KEY_SPECIAL)){
				specialPressed();
			}else if(inputUnit.isKeyActive(YUG_KEY_TRI)){
				triPressed();
			}else if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){
				cursorPosition = 35;
			}
		}
	}

	void DebugScreen::update(){
		if(loadPanel.externalStatus==YUG_LOAD_PANEL_EXT_SUCCESS||
			loadPanel.externalStatus==YUG_LOAD_PANEL_EXT_FAIL){
				loadPanel.externalStatus=YUG_LOAD_PANEL_EXT_IDLE;
				loadPanel.cleanup();
		}
		else if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.update();
		}else
			continueWaiting();
	}

	void DebugScreen::render(){
		if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.render();
		}
		glm::vec3 posi = glm::vec3(-0.6f,0.6f, -2.75f);
		glm::vec3 scale = glm::vec3(0.3f, 0.7f, 1.0f);
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;
		

		int cursorOff = cursorPosition - 0;
		posi.y += cursorOff*0.2f;
		textPrinter.printText("New Deck",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f;
		cursorOff = cursorPosition - 1;
		posi.y += cursorOff*0.2f;
		textPrinter.printText("Clear Trunk",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f;
		cursorOff = cursorPosition - 2;
		posi.y += cursorOff*0.2f;
		textPrinter.printText("To Trunk",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f;
		cursorOff = cursorPosition - 3;
		posi.y += cursorOff*0.2f;
		textPrinter.printText("Load",
			YUG_TEXT_INFO_FONT ,scale, posi, glm::mat4());

		posi.y = 0.6f;
		cursorOff = cursorPosition - 4;
		posi.y += cursorOff*0.2f;
		textPrinter.printText("Save",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f;
		cursorOff = cursorPosition - 5;
		posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 1 - Simon",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 6; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 2 - Tea",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 7; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 3 - Joey",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 8; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 4 - Fuser",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 9; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 5 - Option Guard",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 10; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 6 - Panik - file 8",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 11; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 7 - Weevil - file 7",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 12; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 8 - Rex - file 6",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 13; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 9 - Mai - file 9",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 14; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 10 - Mako - file 11",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 15; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 11 - Shadi - file 13",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 16; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 12 - Keith - file 10",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 17; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 13 - Bakura - file 12",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 18; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 14 - Duke - file 15",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 19; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 15 - Isis - file 14",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 20; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 16 - Pegasus - file 16",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 21; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 17 - Low Forest - file 17",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 22; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 18 - High Forest - file 18",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 23; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 19 - Low Desert - file 19",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 24; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 20 - High Desert - file 20",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 25; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 21 - Low Ocean - file 21",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 26; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 22 - High Ocean - file 22",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 27; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 23 - Low Field - file 23",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 28; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 24 - High Field - file 24",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 29; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 25 - Low Mount - file 25",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 30; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 26 - High Mount - file 26",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 31; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 27 - Magic Mage - file 27",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 32; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 28 - Heishin - file 28",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 33; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 29 - Seto - file 29",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 34; posi.y += cursorOff*0.2f;
		textPrinter.printText("Duel 30 - DarkNite - file 30",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		posi.y = 0.6f; cursorOff = cursorPosition - 35; posi.y += cursorOff*0.2f;
		textPrinter.printText("Test dialog screen",
			YUG_TEXT_INFO_FONT,scale, posi, glm::mat4());

		textPrinter.ignoreCamera = false;
	}

	void DebugScreen::triPressed(){
		blankBoard = (!blankBoard);
		std::cout<<"DebugScreen::triPressed: blankBoard = "<<blankBoard<<std::endl;
		wait(0.3f);
	}

	void DebugScreen::specialPressed(){
		endUnit.won = true;
		endUnit.pow = true;

		std::string s = "GameData/enemies/1.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/2.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/3.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/4.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/5.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/6.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/7.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/8.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/9.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/11.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}
		s = "GameData/enemies/13.txt"; enemyData.loadData(s);
		for(unsigned int i = 0; i < 5; i++){
			endUnit.resultValue = i; endUnit.getWonCard(); endUnit.giveCardToPlayer();
		}

	}

	void DebugScreen::xPressed(){
		std::string s;
		Card::Forest forest; Card::Sogen sogen; Card::Wasteland waste;
		Card::Mountain mount; Card::Umi umi; Card::Yami yami;

		switch(cursorPosition){
		case 0: 
			playerData.deck.generateNewDeck();
			std::cout<<"DS: new deck made\n";
			break;
		case 1:
			for(unsigned int i = 0; i < YUG_MAX_NO_OF_CARDS; i++){
				playerData.trunk[i] = 0;
			}
			std::cout<<"DS: trunk cleared\n";
			break;
		case 2:
			trunkCalled();
			break;
		case 3:
			/*playerData.loadGame(2);
			break;*/
			loadPanel.saveMode = false;
			loadPanel.startup();
			loadPanel.activate(glm::vec3(0,2,-2.75f),glm::vec3(0,0,-2.75f));
			break;
		case 4:
			/*playerData.saveGame(2);
			std::cout<<playerData.saveDescription(1)<<std::endl<<playerData.saveDescription(2)<<std::endl<<playerData.isEmpty(3)<<std::endl;
			break;*/
			loadPanel.saveMode = true;
			loadPanel.startup();
			loadPanel.activate(glm::vec3(0,2,-2.75f),glm::vec3(0,0,-2.75f));
			break;
		case 5://simon
			s = "GameData/enemies/1.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 6://teana
			s = "GameData/enemies/2.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 7://jono
			s = "GameData/enemies/3.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 8://fuser
			s = "GameData/enemies/4.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 9://guard
			s = "GameData/enemies/5.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 10://panik
			s = "GameData/enemies/8.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 11:
			s = "GameData/enemies/7.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 12:
			s = "GameData/enemies/6.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 13:
			s = "GameData/enemies/9.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 14:
			s = "GameData/enemies/11.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 15:
			s = "GameData/enemies/13.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 16:
			s = "GameData/enemies/10.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 17:
			s = "GameData/enemies/12.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 18:
			s = "GameData/enemies/15.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 19:
			s = "GameData/enemies/14.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 20:
			s = "GameData/enemies/16.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			break;
		case 21://low forest
			s = "GameData/enemies/17.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				forest.setNewFieldTexture(); forest.setFieldBoosts();
				forest.setFieldWeakens(); forest.setFieldAmtran();
			}
			break;
		case 22://high forest
			s = "GameData/enemies/18.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				forest.setNewFieldTexture(); forest.setFieldBoosts();
				forest.setFieldWeakens(); forest.setFieldAmtran();
			}
			break;
		case 23://low desert
			s = "GameData/enemies/19.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				waste.setNewFieldTexture(); waste.setFieldBoosts();
				waste.setFieldWeakens(); waste.setFieldAmtran();
			}
			break;
		case 24://high desert
			s = "GameData/enemies/20.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				waste.setNewFieldTexture(); waste.setFieldBoosts();
				waste.setFieldWeakens(); waste.setFieldAmtran();
			}
			break;
		case 25://low ocean
			s = "GameData/enemies/21.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				umi.setNewFieldTexture(); umi.setFieldBoosts();
				umi.setFieldWeakens(); umi.setFieldAmtran();
			}
			break;
		case 26://high ocean
			s = "GameData/enemies/22.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				umi.setNewFieldTexture(); umi.setFieldBoosts();
				umi.setFieldWeakens(); umi.setFieldAmtran();
			}
			break;
		case 27://low field
			s = "GameData/enemies/23.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				sogen.setNewFieldTexture(); sogen.setFieldBoosts();
				sogen.setFieldWeakens(); sogen.setFieldAmtran();
			}
			break;
		case 28://high field
			s = "GameData/enemies/24.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				sogen.setNewFieldTexture(); sogen.setFieldBoosts();
				sogen.setFieldWeakens(); sogen.setFieldAmtran();
			}
			break;
		case 29://low mount
			s = "GameData/enemies/25.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				mount.setNewFieldTexture(); mount.setFieldBoosts();
				mount.setFieldWeakens(); mount.setFieldAmtran();
			}
			break;
		case 30://high mount
			s = "GameData/enemies/26.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				mount.setNewFieldTexture(); mount.setFieldBoosts();
				mount.setFieldWeakens(); mount.setFieldAmtran();
			}
			break;
		case 31://magic mage
			s = "GameData/enemies/27.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				yami.setNewFieldTexture(); yami.setFieldBoosts();
				yami.setFieldWeakens(); yami.setFieldAmtran();
			}
			break;
		case 32://heishin
			s = "GameData/enemies/28.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			if(!blankBoard){
				yami.setNewFieldTexture(); yami.setFieldBoosts();
				yami.setFieldWeakens(); yami.setFieldAmtran();
			}
			break;
		case 33://seto
			s = "GameData/enemies/29.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			//if(!blankBoard){}
			break;
		case 34://DarkNite
			s = "GameData/enemies/30.txt"; enemyData.loadData(s);
			theBoard.nextScreen = this; cleanup(); theBoard.startup();
			//if(!blankBoard){}
			break;
		case 35:
			/*screenUnit.comingScreen = new DialogScreen();
			cleanup();
			fadeUnit.sheet.amtran = YUG_FADE_BLACK;
			screenUnit.startUpComingScreen(
				"GameData/Story/dialogScreens/testInfo.txt");*/
			/*screenUnit.comingScreen = new SplashScreen();
			screenUnit.comingScreen->nextScreen = new DialogScreen();
			cleanup();
			fadeUnit.sheet.amtran = YUG_FADE_BLACK;
			screenUnit.startUpComingScreen(
				"GameData/Story/splashScreens/crucibleCrabSplash.txt");*/
			screenUnit.comingScreen = new MainMenu();
			cleanup();
			fadeUnit.sheet.amtran = YUG_FADE_BLACK;
			screenUnit.startUpComingScreen(" ");
			break;
		default:
			break;
		}
		wait(0.2f);

		GLint nTotalMemoryInKB = 0;
		glGetIntegerv( GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
                       &nTotalMemoryInKB );
		GLint nCurAvailMemoryInKB = 0;
		glGetIntegerv( GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
                       &nCurAvailMemoryInKB );
		std::cout<<"Total Mem: "<<nTotalMemoryInKB<<" Current Mem: "<<nCurAvailMemoryInKB<<std::endl;
	
	}

	void DebugScreen::trunkCalled(){
		hideCursor();
		trunkUnit.nextScreen = this;
		cleanup();
		trunkUnit.startup();
	}

	void DebugScreen::moveCursorDown(){
		std::cout<<"DS: curs down: "<<cursorPosition<<std::endl;
		if(cursorPosition < 35){
			cursorPosition++;
		}
		wait(0.2f);
	}
	void DebugScreen::moveCursorUp(){
		std::cout<<"DS: curs up: "<<cursorPosition<<std::endl;
		if(cursorPosition > 0){
			cursorPosition--;
		}
		wait(0.2f);
	}

	void DebugScreen::setupCursor(){
		puzzleCursor.doRender = true;
		puzzleCursor.cursorModel.doRender = true;
		puzzleCursor.doRender = true;
		puzzleCursor.cursorModel.ignoreCamera = true;
		puzzleCursor.cursorModel.position = glm::vec3(-0.8f,0.6f, -2.75f);
	}

	void DebugScreen::hideCursor(){
		puzzleCursor.doRender = false;
	}



}
