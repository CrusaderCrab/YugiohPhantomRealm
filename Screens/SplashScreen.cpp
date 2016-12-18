#include <iostream>
#include <sstream>
#include <fstream>
#include <Screens\ScreenUnit.h>
#include <Screens\SplashScreen.h>
#include <Utility\InputUnit.h>
#include <DefinesAndTypedefs.h>
#include <Screens\DialogScreen.h>
#include <Screens\StoryScene.h>
#include <Screens\MainMenu.h>

#define YUG_SPLASH_CH_FIRST 3
#define YUG_SPLASH_CH_SECOND 4
#define YUG_SPLASH_CH_OUT 5
#define YUG_SPLASH_CH_FINISH 6
#define YUG_SPLASH_CH_IDLE 7
namespace Screen{

	void SplashScreen::startup(const char* fileLocal){
		BaseScreen::startup();
		readFile(fileLocal);
		skipPressed = false;
		chain = YUG_SCREEN_CH_START;
	}

	void SplashScreen::readFile(const char* fileLocal){
		std::ifstream in(fileLocal);
		if(!in){
			std::cout<<"SplashScreen::readFile(): Failed to open file: "<<
				fileLocal<<"\nCrashing now.\n";
			return;
		}
		in>>nextScreenFile;
		//std::cout<<"splash: sf: "<<nextScreenFile<<std::endl;
		in>>inTime;
		in>>outTime;
		std::string s;
		in>>s;
		setupPicture(s.c_str());
	}

	void SplashScreen::setupPicture(const char* fl){
		picture.startup(YUG_PLANE_FILE_PATH, fl);
		picture.doRender = true;
		picture.ignoreCamera = true;
		picture.scale = glm::vec3(0.82f, 0.66f, 1);
		picture.position = glm::vec3(0.0f, 0.0f, -2.0f);
	}
	void SplashScreen::cleanup(){
		picture.cleanup();
	}
	void SplashScreen::render(){
		picture.render();
	}
	void SplashScreen::update(){
		picture.update();
		if(!isWaiting){
			switch(chain){
			case YUG_SCREEN_CH_START: firstUpdate(); wait(inTime); break;
			case YUG_SPLASH_CH_FIRST: chain = YUG_SCREEN_CH_IDLE; break;
			case YUG_SPLASH_CH_OUT: fadeOutUpdate(); wait(outTime); break;
			case YUG_SPLASH_CH_FINISH: finishUpdate(); break;
			}
		}else{
			continueWaiting();
		}
	}
	
	void SplashScreen::firstUpdate(){
		beginScreen(inTime);
		chain = YUG_SPLASH_CH_FIRST;
	}
	void SplashScreen::fadeOutUpdate(){
		fadeOut(outTime);
		chain = YUG_SPLASH_CH_FINISH;
	}
	void SplashScreen::finishUpdate(){
		toNextScreen();
	}

	void SplashScreen::toNextScreen(){
		//std::cout<<"SpalshScreen: toNextScreen()\n";
		cleanup();
		if(nextScreenFile[0]=='g'){
			screenUnit.comingScreen = new MainMenu();
			//std::cout<<"Splash: to main menu\n";
			screenUnit.startUpComingScreen(nextScreenFile.c_str());
		}
		else{
			screenUnit.comingScreen = new StoryScene();
			screenUnit.startUpComingScreen(nextScreenFile.c_str());
		}
		chain = YUG_SPLASH_CH_IDLE;
		//screenUnit.comingScreen = (BaseScreen*)nextScreen;
		//screenUnit.startUpComingScreen(nextScreenFile.c_str());
		//((DialogScreen*)nextScreen)->startup(nextScreenFile.c_str());
	}

	void SplashScreen::input(){
		if(chain == YUG_SCREEN_CH_IDLE){
			if(inputUnit.isKeyActive(YUG_KEY_O) ||
			   inputUnit.isKeyActive(YUG_KEY_START) ||
			   inputUnit.isKeyActive(YUG_KEY_X)){
					chain = YUG_SPLASH_CH_OUT;
			}
		}
	}

}