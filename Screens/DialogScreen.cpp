#include <iostream>
#include <sstream>
#include <Utility\UpdateUnit.h>
#include <Utility\BlankUnit.h>
#include <Utility\StateUnit.h>
#include <Base\RenderUnit.h>
#include <Screens\DialogScreen.h>
#include <Screens\ScreenUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <DefinesAndTypedefs.h>

#define YUG_SCREEN_CH_TEXT_IN 2
#define YUG_SCREEN_CH_TEXT_OUT 3
#define YUG_SCREEN_CH_INPUT 4
#define YUG_SCREEN_CH_END 5

namespace Screen{

	void DialogScreen::startup(){
		std::cout<<"Wrong startup for dialogScreen called\n";
	}

	void DialogScreen::startup(const char* fileLocal){
		BaseScreen::startup();
		std::cout<<"Dialog s1\n";
		dialogDepth = 0;
		chain = YUG_SCREEN_CH_START;
		waitingForInput = false;
		timer = 0;
		readFile(fileLocal);
		std::cout<<"Dialog sEnd\n";
	}

	void DialogScreen::readFile(const char* fileLocal){
		std::ifstream in(fileLocal);
		if(!in){
			std::cout<<"DialogScreen::readFile(): Failed to open file: "<<
				fileLocal<<"\nCrashing now.\n";
			return;
		}
		in>>nextScreenFile;
		in>>comingScreenID;
		std::string holderString;
		in>>holderString;
		std::cout<<"dialog s2: "<<holderString<<std::endl;
		setupBg( holderString.c_str());
		in>>holderString;
		std::cout<<"dialog s3: "<<holderString<<std::endl;
		setupChar( holderString.c_str());
		setupWindow(YUG_SCREEN_WINDOW_TEXTURE_PATH);
		std::string concatStr;
		holderString = "";
		in>>concatStr;
		while(concatStr.compare("DONE")){
			if(concatStr.compare("#")==0){
				std::cout<<"dialog s#: "<<holderString<<std::endl;
				dialogStrings.push_back(holderString);
				holderString = "";
			}else{
				holderString += " "+concatStr;
			}
			in>>concatStr;
		}
	}

	void DialogScreen::setupBg(const char* fileLocal){
		bgModel.startup(YUG_PLANE_FILE_PATH, fileLocal);
		bgModel.doRender = true;
		bgModel.ignoreCamera = true;
		bgModel.scale = YUG_SCREEN_BG_SIZE;
		bgModel.position = YUG_SCREEN_BG_POS;
	}
	void DialogScreen::setupChar(const char* fileLocal){
		charModel.startup(YUG_PLANE_FILE_PATH, fileLocal);
		charModel.doRender = true;
		charModel.ignoreCamera = true;
		charModel.scale = YUG_SCREEN_CHAR_REG_SCALE;
		charModel.position = YUG_SCREEN_CHAR_REG_POS;
	}
	void DialogScreen::setupWindow(const char* fileLocal){
		windowModel.startup(YUG_PLANE_FILE_PATH, fileLocal);
		windowModel.doRender = true;
		windowModel.ignoreCamera = true;
		windowModel.scale = YUG_SCREEN_WINDOW_SIZE;
		windowModel.position = YUG_SCREEN_WINDOW_POS;
	}

	void DialogScreen::cleanup(){
		bgModel.cleanup();
		charModel.cleanup();
		windowModel.cleanup();
	}

	void DialogScreen::render(){
		bgModel.render();
		charModel.render();
		windowModel.render();
	}
	void DialogScreen::update(){
		bgModel.update();
		charModel.update();
		windowModel.update();
		if(!isWaiting){
			switch(chain){
			case YUG_SCREEN_CH_START: firstUpdate(); break;
			case YUG_SCREEN_CH_TEXT_IN: textInUpdate(); break;
			case YUG_SCREEN_CH_INPUT: inputUpdate(); break;
			case YUG_SCREEN_CH_TEXT_OUT: textOutUpdate(); break;
			case YUG_SCREEN_CH_END: endUpdate(); break;
			default: break;
			}
		}else{
			continueWaiting();
		}
	}

	void DialogScreen::input(){

	}

	void  DialogScreen::firstUpdate(){ 
		beginScreen(1.0f);
		std::cout<<"Dialog: Screen fading in\n";
		chain = YUG_SCREEN_CH_TEXT_IN;
	}
	void  DialogScreen::textInUpdate(){}
	void  DialogScreen::inputUpdate(){}
	void  DialogScreen::textOutUpdate(){}
	void  DialogScreen::endUpdate(){}

}