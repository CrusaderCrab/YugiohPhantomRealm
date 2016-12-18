#include <Screens\ShopScreen.h>
#include <Utility\Clock.h>
#include <Utility\TextPrinter.h>
#include <Utility\TextureLoader.h>
#include <Utility\InputUnit.h>
#include <Game\PlayerData.h>
#include <Screens\ScreenUnit.h>
#include <Screens\FreeDuelScreen.h>
#include <Screens\TempleMapScreen.h>
#include <Screens\WorldMapScreen.h>
#include <Screens\OptionsScreen.h>
#include <DefinesAndTypedefs.h>
#include <Utility\SoundUnit.h>
#include <iostream>
#define ZYUG_CH_START 0
#define ZYUG_CH_ACTIVE 1
#define ZYUG_CH_IDLE 2
#define ZYUG_CH_END 3
#define ZYUG_CH_LOAD 4
#define ZYUG_CH_TALK1 5
#define ZYUG_CH_TALK2 6
#define ZYUG_CH_TALK3 7
#define ZYUG_CH_PREEND 8

namespace Screen{

	void ShopScreen::startup(const char* fileLocal){
		BaseScreen::startup();
		miscInfo = fileLocal[0];
		buttonTimer = 0;
		cursorPos = 0;
		doRenderText = false;
		textTimer = 0;
		text.push_back("Welcome.");
		text.push_back("What do you need?");
		setupbg();
		setupbutton();
		setupTextBox();
		setupcursor();
		chain = ZYUG_CH_START;
		soundUnit.playLoop("GameData/sounds/music/ready/cardShopMusic_o.wav");
	}

	void ShopScreen::cleanup(){
		bg.cleanup();
		textBox.cleanup();
		cursor.cleanup();
		button.textureBO = buttonTex[0];
		button.cleanup();
		textureLoader.deleteTexture(&buttonTex[1]);
		textureLoader.deleteTexture(&buttonTex[2]);
		soundUnit.stopAll();
	}

	void ShopScreen::input(){
		if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.input();
		}else if(!isWaiting){
			if(chain==ZYUG_CH_TALK2 && 
				(inputUnit.isKeyActive(YUG_KEY_X)||
				inputUnit.isKeyActive(YUG_KEY_START))){
					soundUnit.cursorSelect();
					chain = ZYUG_CH_TALK3;
			}else{
				normalInputs();
			}
		}
	}

	void ShopScreen::normalInputs(){
		
		if(chain==ZYUG_CH_ACTIVE && (!isWaiting)){
			if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
				downPressed(); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
				upPressed(); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_X)){
				xPressed(); wait(0.2f);
			}
		}
		
	}

	void ShopScreen::xPressed(){
		soundUnit.cursorSelect();
		if(cursorPos>1){
			chain = ZYUG_CH_PREEND;
		}else if(cursorPos==0){
			loadPanel.saveMode = true;
			loadPanel.startup();
			glm::vec3 s = bg.position + glm::vec3(0,1.2f,0.011f);
			glm::vec3 e = bg.position + glm::vec3(0,0,0.011f);
			loadPanel.activate(s,e);
			chain = ZYUG_CH_IDLE;
			wait(0.2f);
		}else{
			loadPanel.saveMode = false;
			loadPanel.startup();
			glm::vec3 s = bg.position + glm::vec3(0,1.2f,0.011f);
			glm::vec3 e = bg.position + glm::vec3(0,0,0.011f);
			loadPanel.activate(s,e);
			chain = ZYUG_CH_IDLE;
			wait(0.2f);
		}
	}

	void ShopScreen::downPressed(){
		soundUnit.cursorMove();
		cursorPos = (cursorPos+1)%5;
		cursor.position = glm::vec3(-0.4f, 0.38f, -1.990f);
		cursor.position.y -= cursorPos*0.0889f;
	}

	void ShopScreen::upPressed(){
		soundUnit.cursorMove();
		cursorPos = (cursorPos-1);
		if(cursorPos<0) cursorPos = 4;
		cursor.position = glm::vec3(-0.4f, 0.38f, -1.990f);
		cursor.position.y -= cursorPos*0.0889f;
	}

	void ShopScreen::render(){
		bg.render();
		textBox.render();
		if(chain==ZYUG_CH_TALK2)button.render();
		if(doRenderText){renderText();}
		cursor.render();
		loadPanel.render();
	}

	void ShopScreen::update(){
		buttonUpdate();
		cursor.update();
		if(loadPanel.externalStatus==YUG_LOAD_PANEL_EXT_FAIL || 
		loadPanel.externalStatus==YUG_LOAD_PANEL_EXT_SUCCESS){
		loadPanel.externalStatus=YUG_LOAD_PANEL_EXT_IDLE;
			loadPanel.cleanup();
			chain = ZYUG_CH_ACTIVE;
			wait(0.1f);
		}else if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.update();
		}else{
			if(!isWaiting){
				switch(chain){
				case ZYUG_CH_START: firstUpdate(); break;
				case ZYUG_CH_PREEND: preEndUpdate(); break;
				case ZYUG_CH_END: endUpdate(); break;
				case ZYUG_CH_TALK1: fadeTextInUpdate(); break;
				case ZYUG_CH_TALK3: fadeTextOutUpdate(); break;
				}
			}else{
				continueWaiting();
			}
		}
	}

	void ShopScreen::toNextScreen(){
		screenUnit.startUpComingScreen(nextScreenFile.c_str());
	}

	void ShopScreen::endUpdate(){
		std::string fs;
		//std::cout<<"shop: "<<cursorPos<<std::endl;
		if(cursorPos==4){
			if(playerData.currentPlotPoint<YUG_PLOT_POINT_POST_TOURNEY){
				screenUnit.comingScreen = new TempleMapScreen();
				nextScreenFile = "b";
				//std::cout<<"shop: made temple map\n";
			}else{
				screenUnit.comingScreen = new WorldMapScreen();
				nextScreenFile = "h";
				//std::cout<<"shop: made world map\n";
			}
		}else{
			if(cursorPos==2){
				screenUnit.comingScreen = new FreeDuelScreen();
				nextScreenFile = "s";
			}
			if(cursorPos==3){
				screenUnit.comingScreen = new OptionsScreen();
				((OptionsScreen*)screenUnit.comingScreen)->setupNames();
				nextScreenFile = "s";
			}
		}
		chain = ZYUG_CH_IDLE;
		cleanup();
		toNextScreen();
	}

	void ShopScreen::preEndUpdate(){
		fadeOut(0.4f);
		wait(0.8f);
		chain = ZYUG_CH_END;
	}

	void ShopScreen::firstUpdate(){
		beginScreen(0.5f);
		wait(0.55f);
		textAmtran = glm::vec4(1,1,1,0);
		buttonTexPos = 0;
		doRenderText = true;
		chain = ZYUG_CH_TALK1;
	}


	void ShopScreen::fadeTextInUpdate(){
		textTimer += gameClock.lastLoopTime()*2.0f;
		if(textTimer >= 1.0f){
			textAmtran = glm::vec4(1,1,1,1);
			chain = ZYUG_CH_TALK2;
			textTimer = 0;
		}else{
			textAmtran.w = textTimer;
		}
	}

	void ShopScreen::fadeTextOutUpdate(){
		textTimer += gameClock.lastLoopTime()*2.5f;
		if(textTimer >= 1.0f){
			textAmtran = glm::vec4(1,1,1,0);
			chain = ZYUG_CH_ACTIVE;
			textTimer = 0;
			doRenderText = false;
			cursor.doRender = true;
			cursor.interpolateAmtran(glm::vec4(1,1,1,0.5f), 0.2f);
			textPrinter.amtran = glm::vec4(1,1,1,1);
		}else{
			textAmtran.w = 1.0f-textTimer;
		}
	}

	void ShopScreen::buttonUpdate(){
		buttonTimer += gameClock.lastLoopTime()*3.2f;
		if(buttonTimer>=1.0f){
			buttonTexPos = (buttonTexPos+1)%3;
			button.textureBO = buttonTex[buttonTexPos];
			buttonTimer = 0;
		}
	}

	void ShopScreen::renderText(){
		textPrinter.leftAlign = true;
		textPrinter.ignoreCamera = true;
		textPrinter.amtran = textAmtran;
		glm::vec3 posi = glm::vec3(-0.7f,-0.363f,-1.991f);
		for(int i = 0; i < text.size(); i++){
			textPrinter.printText(text[i].c_str(), YUG_TEXT_INFO_FONT, 
				glm::vec3(0.3f, 0.7f, 1), posi, glm::mat4());
			posi.y -= 0.07f;
		}
	}

	void ShopScreen::setupbg(){
		bg.startup( YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/singles/shopbg.png");
		bg.doRender = true;
		bg.ignoreCamera = true;
		bg.scale = glm::vec3(0.82f, 0.444f, 1);
		bg.position = glm::vec3(0.0f,0.206f, -2.00f);
	}

	void ShopScreen::setupTextBox(){
		textBox.startup( YUG_PLANE_FILE_PATH, 
			YUG_SCREEN_WINDOW_TEXTURE_PATH);
		textBox.doRender = true;
		textBox.ignoreCamera = true;
		textBox.scale = glm::vec3(0.81f,0.217f,1);
		textBox.position = glm::vec3(0,-0.444f, -1.994f);
	}

	void ShopScreen::setupbutton(){
		button.startup( YUG_PLANE_FILE_PATH, "GameData/textures/screens/singles/textbutton1.png");
		buttonTex[0] = button.textureBO;
		textureLoader.loadTexture("GameData/textures/screens/singles/textbutton2.png", &buttonTex[1]);
		textureLoader.loadTexture("GameData/textures/screens/singles/textbutton3.png", &buttonTex[2]);
		button.doRender = true;
		button.ignoreCamera = true;
		button.scale = glm::vec3(0.03f, 0.03f, 1);
		button.position = glm::vec3(0.7f, -0.55f, -1.992f);
	}

	void ShopScreen::setupcursor(){
		cursor.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/models/positionunit/concursor.png");
		cursor.doRender = false;
		cursor.ignoreCamera = true;
		cursor.amtran = glm::vec4(1,1,1,0);
		cursor.scale = glm::vec3(0.22f, 0.04f, 1);
		cursor.position = glm::vec3(-0.4f, 0.38f, -1.990f);
	}
}
