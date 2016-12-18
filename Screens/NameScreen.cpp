#include <iostream>
#include <Utility\InputUnit.h>
#include <Screens\NameScreen.h>
#include <DefinesAndTypedefs.h>
#include <Screens\ScreenUnit.h>
#include <Utility\TextureLoader.h>
#include <Utility\TextPrinter.h>
#include <Utility\Clock.h>
#include <Game\PlayerData.h>
#include <Screens\StoryScene.h>
#include <Utility\SoundUnit.h>

#define ZYUG_CH_START 0
#define ZYUG_CH_IDLE 1
#define ZYUG_CH_ACTIVE 2
#define ZYUG_CH_END 3
#define ZYUG_SP_NO 4
#define ZYUG_SP_LEFT 5
#define ZYUG_SP_RIGHT 6
#define ZYUG_SP_END 7
#define ZYUG_ROW_LENGTH 9

namespace Screen{

	 const char letters[] = {
		 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 
		 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 
		 'W', 'X', 'Y', 'Z', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
		 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 
		 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
		 'w', 'x', 'y', 'z', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
		 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ', 
		 '#', '#', '#', '#', '#', '#', '#', '!', '?', '.', '#' 
	 };

	void NameScreen::startup(const char* fileLocal){
		startup();
	}
	void NameScreen::startup(){
		BaseScreen::startup();
		chain = ZYUG_CH_START;
		vertPos = 0;
		horzPos = 0;
		specialPos = ZYUG_SP_NO;
		name = std::string();
		cursorTimer = 0;
		setupBg();
		setupCursor();
		setupTextures();
		updateCursorPos();
		soundUnit.playLoop("GameData/sounds/music/ready/nameInput_o.wav");
	}

	void NameScreen::cleanup(){
		bg.cleanup();
		cursor.textureBO = textures[0];
		cursor.cleanup();
		textureLoader.deleteTexture(&textures[1]);
		textureLoader.deleteTexture(&textures[2]);
		soundUnit.stopAll();
	}

	void NameScreen::input(){
		if(!isWaiting){
			if(inputUnit.isKeyActive(YUG_KEY_LEFT)){
				moveCursorPos(0, -1); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){
				moveCursorPos(0, 1); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
				moveCursorPos(-1, 0); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
				moveCursorPos(1, 0); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_X)){
				xPressed(); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_O)){
				removeLetter(); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_START)){
				specialPos=ZYUG_SP_END;
				vertPos = 6;
				updateCursorPos();
				wait(0.2f);
			}
		}
	}

	void NameScreen::xPressed(){
		if(specialPos==ZYUG_SP_NO){
			char c = getCorrectChar();
			if(c!='#'){ 
				addLetter(c);
				soundUnit.cursorSelect();
			}else{
				soundUnit.cursorSelectionFail();
			}
		}else if(name.size()>0 && specialPos==ZYUG_SP_END){
			soundUnit.cursorSelect();
			fadeOut(0.1f);
			wait(0.21f);
			chain = ZYUG_CH_END;
		}else{
			soundUnit.cursorSelectionFail();
		}
	}

	void NameScreen::update(){
		animateCursor();
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

	void NameScreen::firstUpdate(){
		beginScreen(0.7f);
		chain = ZYUG_CH_ACTIVE;
		wait(0.8f);
	}

	void NameScreen::finalUpdate(){
		playerData.playerName.clear();
		for(unsigned int i = 0; i < name.size(); i++){
			playerData.playerName.push_back(name[i]);
		}
		playerData.playerName.push_back('\0');
		playerData.deck.generateNewDeck();
		playerData.currentPlotPoint = 0;
		for(int i = 0; i < YUG_NO_OF_FREE_DUELISTS; i++){
			playerData.unlockedDuelists[i] = false;
			playerData.plotUnlockedDuelists[i] = false;
		}
		playerData.unlockedDuelists[YUG_NO_OF_FREE_DUELISTS-1]=true;
		for(int i = 0; i < YUG_MAX_NO_OF_CARDS; i++){
			playerData.trunk[i] = 0;
		}
		chain = ZYUG_CH_IDLE;
		playerData.name = std::string(playerData.playerName.data());
		toNextScreen();
	}

	void NameScreen::render(){
		bg.render();
		cursor.render();
		renderText();
	}

	void NameScreen::renderText(){
		textPrinter.leftAlign = true;
		textPrinter.ignoreCamera = true;
		textPrinter.printText(name.c_str(), YUG_TEXT_INFO_FONT, 
			glm::vec3(0.3f, 0.8f, 1.0f), glm::vec3(-0.215f, -0.5f, -1.99f), 
			glm::mat4());
		textPrinter.ignoreCamera = false;
	}

	void NameScreen::toNextScreen(){
		//std::cout<<"NameScreen: toNextScreen()";
		cleanup();
		screenUnit.comingScreen = new StoryScene();
		screenUnit.startUpComingScreen(
			"GameData/Story/ss/01ruins.txt");
	}

	void NameScreen::setupBg(){
		bg.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/Menu/newnamebg.png");
		bg.doRender = true;
		bg.ignoreCamera = true;
		bg.scale = glm::vec3(0.83f, 0.65f, 1);
		bg.position = glm::vec3(0.0f, 0.00f, -2.0f);
	}

	void NameScreen::setupCursor(){
		cursor.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/screens/Menu/newnamecursor1.png");
		cursor.doRender = true;
		cursor.ignoreCamera = true;
		cursor.scale = glm::vec3(0.07f, 0.07f, 1.0f);
	}

	void NameScreen::setupTextures(){
		textureLoader.deleteTexture(&cursor.textureBO);
		textureLoader.loadTexture(
			"GameData/textures/screens/Menu/newnamecursor1.png",
			&textures[0]);
		textureLoader.loadTexture(
			"GameData/textures/screens/Menu/newnamecursor2.png",
			&textures[1]);
		textureLoader.loadTexture(
			"GameData/textures/screens/Menu/newnamecursor3.png",
			&textures[2]);
		cursor.textureBO = textures[0];
		textureNo = 0;
	}

	void NameScreen::addLetter(char c){
		if(name.size()>10) return;
		name += c;
	}

	void NameScreen::removeLetter(){
		if(name.size()<=0) return;
		soundUnit.cursorCancel();
		name.erase(name.end()-1);
	}

	void NameScreen::animateCursor(){
		cursorTimer+=gameClock.lastLoopTime();
		if(cursorTimer>0.4f){
			textureNo = (textureNo+1)%3;
			cursor.textureBO = textures[textureNo];
			cursorTimer=0;
		}
	}

	char NameScreen::getCorrectChar(){
		if(specialPos!=ZYUG_SP_NO) return ' ';
		else
			return(letters[vertPos*11+horzPos]);
	}

	void NameScreen::updateCursorPos(){
		if(specialPos==ZYUG_SP_LEFT){
			cursor.scale = glm::vec3(0.052f, 0.052f, 1.0f);
			cursor.position = glm::vec3(0.57f, 0.088f, -1.99f);
		}else if(specialPos==ZYUG_SP_RIGHT){
			cursor.scale = glm::vec3(0.052f, 0.052f, 1.0f);
			cursor.position = glm::vec3(0.675f, 0.088f, -1.99f);
		}else if(specialPos==ZYUG_SP_END){
			cursor.scale = glm::vec3(0.104f, 0.052f, 1.0f);
			cursor.position = glm::vec3(0.62f, -0.108f, -1.99f);
		}else{
			cursor.scale = glm::vec3(0.052f, 0.052f, 1.0f);
			cursor.position = glm::vec3(-0.67f, 0.48f, -1.99f);//start pos
			cursor.position.x += horzPos*0.1038f;
			cursor.position.y -= vertPos*0.098f;
		}
	}

	void NameScreen::moveCursorPos(int upDown, int leftRight){
		if(specialPos==ZYUG_SP_RIGHT){
			if(leftRight==-1) 
				specialPos= ZYUG_SP_LEFT;
			else if(leftRight==1){ 
				specialPos = ZYUG_SP_NO;
				horzPos = 0;
			}else{
				specialPos = ZYUG_SP_END;
			}
		}else if(specialPos==ZYUG_SP_LEFT){
			if(leftRight==1){
				specialPos = ZYUG_SP_RIGHT;
			}else if(leftRight==-1){
				specialPos = ZYUG_SP_NO;
				horzPos = 10;
			}else{
				specialPos = ZYUG_SP_END;
				vertPos = 6;
			}
		}else if(specialPos==ZYUG_SP_END){
			if(leftRight==1){
				specialPos = ZYUG_SP_NO; horzPos=0;
			}else if(leftRight==-1){
				specialPos = ZYUG_SP_NO; horzPos=10;
			}else{
				specialPos = ZYUG_SP_LEFT; vertPos = 4;
			}
		}else{
			horzPos += leftRight;
			vertPos += upDown;
			if(horzPos>=11 && vertPos==4){
				specialPos = ZYUG_SP_LEFT; 
			}else if(horzPos<0 && vertPos==4){
				specialPos = ZYUG_SP_RIGHT;
			}else if((horzPos>=11||horzPos<0)&&vertPos==6){
				specialPos= ZYUG_SP_END;
			}else{
				horzPos %= 11; if(horzPos<0)horzPos=10;
				vertPos %= 8; if(vertPos<0)vertPos=7;
			}
		}
		soundUnit.cursorMove();
		updateCursorPos();
	}

}
