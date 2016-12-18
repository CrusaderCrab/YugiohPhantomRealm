#include <Screens\FinalScreen.h>
#include <Screens\ScreenUnit.h>
#include <Screens\MainMenu.h>
#include <Game\PlayerData.h>
#include <iostream>
#include <DefinesAndTypedefs.h>
namespace Screen{

	void FinalScreen::startup(const char* fileLocal){
		startup();
	}

	void FinalScreen::startup(){
		BaseScreen::startup();
		chain = 0;
		wait(1.0f);
		//std::cout<<"finalScreen: startup()\n";
	}

	void FinalScreen::cleanup(){
		loadPanel.cleanup();
	}

	void FinalScreen::update(){
		if(loadPanel.externalStatus==YUG_LOAD_PANEL_EXT_SUCCESS ||
			loadPanel.externalStatus==YUG_LOAD_PANEL_EXT_FAIL){
				loadPanel.externalStatus=YUG_LOAD_PANEL_EXT_IDLE;
				chain = 1;
				wait(0.5f);
		}else if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.update();
		}else{
			if(!isWaiting){
				switch (chain){
				case 0: firstUpdate(); break;
				case 1: endUpdate(); break;
				default: break;
				}
			}else{
				continueWaiting();
			}
		}
	}

	void FinalScreen::firstUpdate(){
		beginScreen(0.1f);
		for(unsigned int i = 0; i < YUG_NO_OF_FREE_DUELISTS; i++){
			playerData.plotUnlockedDuelists[i] = false;
		}
		playerData.currentPlotPoint = 88;
		chain = 2;
		loadPanel.startup();
		loadPanel.saveMode = true;
		glm::vec3 s = glm::vec3(0,3,-1.995f);
		glm::vec3 e = glm::vec3(0, 0, -1.995f);
		loadPanel.activate(s,e);
		wait(0.2f);
	}

	void FinalScreen::endUpdate(){
		toNextScreen();
		chain = 5;
	}

	void FinalScreen::toNextScreen(){
		cleanup();
		screenUnit.comingScreen = new MainMenu();
		screenUnit.startUpComingScreen(" ");
	}

	void FinalScreen::render(){
		if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.render();
		}
	}

	void FinalScreen::input(){
		if(loadPanel.externalStatus!=YUG_LOAD_PANEL_EXT_IDLE){
			loadPanel.input();
		}
	}

}
