#include <iostream>
#include <Utility\InputUnit.h>
#include <Screens\MainMenu.h>
#include <DefinesAndTypedefs.h>
#include <Screens\OptionsScreen.h>
#include <Screens\NameScreen.h>
#include <Screens\ScreenUnit.h>
#include <Screens\ShopScreen.h>
#include <Game\PlayerData.h>
#include <Screens\StoryScene.h>
#include <Utility\SoundUnit.h>

//#include <Utility\ConfigUnit.h>
#define ZYUG_CH_IDLE 0
#define ZYUG_CH_START 1
#define ZYUG_CH_LOAD 2
#define ZYUG_CH_ACTIVE 3
#define ZYUG_CH_FADING 4
#define ZYUG_CH_END 5

namespace Screen{

	void MainMenu::toNextScreen(){
		//std::cout<<"MainMenu: toNextScreen() "<<nextScreenFile<<std::endl;
		cleanup();
		screenUnit.comingScreen = (BaseScreen*)nextScreen;
		screenUnit.startUpComingScreen(nextScreenFile.c_str());
	}

	void MainMenu::startup(const char* fileLocal){
		startup();
	}
	void MainMenu::startup(){
		BaseScreen::startup();
		chain = ZYUG_CH_START;
		cursorPosition = 0;
		setupBG();
		setupButtons();
		updateButtons();
		wait(0.2f);
		soundUnit.playLoop("GameData/sounds/music/ready/mainMenu_o.wav");
	}

	void MainMenu::cleanup(){
		bg.cleanup();
		newbutton.cleanup();
		loadButton.cleanup();
		optButton.cleanup();
		soundUnit.stopAll();
	}

	void MainMenu::render(){
		bg.render();
		newbutton.render();
		loadButton.render();
		optButton.render();
		if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.render();
		}
	}

	void MainMenu::update(){
		if(loadPanel.externalStatus==YUG_LOAD_PANEL_EXT_SUCCESS){
			loadPanel.externalStatus=YUG_LOAD_PANEL_EXT_IDLE;
			loadPanel.cleanup();
			fadeOut(0.3f);
			if(playerData.currentPlotPoint != 88){
				nextScreen = new ShopScreen();
				if(playerData.currentPlotPoint < YUG_PLOT_POINT_POST_TOURNEY){
					nextScreenFile = std::string("t");
				}else{
					nextScreenFile = std::string("w");
				}
			}else{
				nextScreen = new StoryScene();
				nextScreenFile = "GameData/Story/ss/01ruins.txt";
			}
			chain = ZYUG_CH_END;
			wait(1.00f);
		}else if(loadPanel.externalStatus==YUG_LOAD_PANEL_EXT_FAIL){
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
				case ZYUG_CH_END: finishUpdate(); break;
				default: break;
				}
			}else{
				continueWaiting();
			}
		}
		newbutton.update();
		loadButton.update();
		optButton.update();
	}

	void MainMenu::firstUpdate(){
		beginScreen(0.1f);
		chain = ZYUG_CH_ACTIVE;
		wait(0.2f);
	}

	void MainMenu::finishUpdate(){
		
		toNextScreen();
	}

	void MainMenu::input(){
		if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.input();
		}else{
			if(chain==ZYUG_CH_ACTIVE && (!isWaiting)){
				if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
					cursorPosition = (cursorPosition+1)%3;
					updateButtons();
					soundUnit.cursorMove();
					wait(0.3f);
				}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
					cursorPosition--;
					if(cursorPosition<0)cursorPosition=2;
					updateButtons();
					soundUnit.cursorMove();
					wait(0.3f);
				}else if(inputUnit.isKeyActive(YUG_KEY_X)){
					soundUnit.cursorSelect();
					buttonPressed();
				}
			}else{
				continueWaiting();
			}
		}
	}

	void MainMenu::updateButtons(){
		newbutton.doRender = false;
		loadButton.doRender = false;
		optButton.doRender = false;
		if(cursorPosition==0) newbutton.doRender = true;
		else if(cursorPosition==1) loadButton.doRender = true;
		else optButton.doRender = true;
	}

	void MainMenu::buttonPressed(){
		if(cursorPosition==0){
			newPressed();
		}else if(cursorPosition==1){
			loadPressed();
		}else{
			optionsPressed();
		}
	}

	void MainMenu::loadPressed(){
		loadPanel.saveMode = false;
		loadPanel.startup();
		glm::vec3 s = bg.position + glm::vec3(0,1.2f,0.005f);
		glm::vec3 e = bg.position + glm::vec3(0,0,0.005f);
		loadPanel.activate(s,e);
		chain = ZYUG_CH_IDLE;
		wait(0.2f);
	}

	void MainMenu::newPressed(){
		fadeOut(0.5f);
		wait(1.01f);
		nextScreen = new NameScreen();
		nextScreenFile = std::string(" ");
		chain = ZYUG_CH_END;
	}
	void MainMenu::optionsPressed(){
		fadeOut(0.3f);
		wait(1.0f);
		nextScreen = new OptionsScreen();
		((OptionsScreen*)nextScreen)->setupNames();
		nextScreenFile = std::string("M");
		chain = ZYUG_CH_END;
	}

	void MainMenu::setupBG(){
		bg.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/Menu/startMenuBG.png");
		bg.doRender = true;
		bg.ignoreCamera = true;
		bg.scale = glm::vec3(0.83f, 0.63f, 1);
		bg.position = glm::vec3(0.0f, 0.05f, -2.0f);
	}

	void MainMenu::setupButtons(){
		newbutton.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/Menu/startNewGameButton.png");
		newbutton.doRender = false; newbutton.ignoreCamera = true;
		newbutton.scale = glm::vec3(0.275f,0.09f, 1.0f);
		newbutton.position = bg.position;
		newbutton.position += glm::vec3(0.0f, 0.365f, 0.001f);

		loadButton.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/Menu/startLoadButton.png");
		loadButton.doRender = false; loadButton.ignoreCamera = true;
		loadButton.scale = glm::vec3(0.193f,0.09f, 1.0f);
		loadButton.position = bg.position;
		loadButton.position += glm::vec3(0.0f, 0.2f, 0.001f);

		optButton.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/Menu/startOptionButton.PNG");
		optButton.doRender = false; optButton.ignoreCamera = true;
		optButton.scale = glm::vec3(0.26f,0.09f, 1.0f);
		optButton.position = bg.position;
		optButton.position += glm::vec3(0.0f, -0.3f, 0.001f);
	}

}