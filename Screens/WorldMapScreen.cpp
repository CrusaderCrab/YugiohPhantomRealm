#include <Screens\WorldMapScreen.h>
#include <Screens\StorySetter.h>
#include <Utility\InputUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Cursor.h>
#include <Screens\ScreenUnit.h>
#include <DefinesAndTypedefs.h>
#include <Utility\SoundUnit.h>
#include <iostream>
#define ZYUG_CH_START 0
#define ZYUG_CH_ACTIVE 1
#define ZYUG_CH_IDLE 2
#define ZYUG_CH_END 3

namespace Screen{

	const char* WorldNames[] = {
		"Village","Temple","The Forest","The Meadow",
		"The Mountain", "The Ocean", "The Desert"
	};

	void WorldMapScreen::toNextScreen(){
		//std::cout<<"cp: "<<cursorPos;
		BaseScreen* cs = StorySetter::getNextScreen(cursorPos);
		std::string fs = StorySetter::getNextFilePath(cursorPos);
		screenUnit.comingScreen = cs;
		screenUnit.startUpComingScreen(fs.c_str());
	}

	void WorldMapScreen::startup(const char* fileLocal){
		BaseScreen::startup();
		cursorPos = fileLocal[0];
		chain = ZYUG_CH_START;
		setupbg();
		setuplegend();
		setupcursor();
		moveCursor();
		soundUnit.playLoop("GameData/sounds/music/ready/mapMusic_o.wav");
	}

	void WorldMapScreen::cleanup(){
		puzzleCursor.doRender = false;
		bg.cleanup();
		legend.cleanup();
		soundUnit.stopAll();
	}

	void WorldMapScreen::input(){
		if(!isWaiting && chain==ZYUG_CH_ACTIVE){
			if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
				downPressed(); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
				upPressed(); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_LEFT)){
				leftPressed(); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){
				rightPressed(); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_X)){
				xPressed();
			}
		}
	}

	void WorldMapScreen::xPressed(){
		chain = ZYUG_CH_END;
		fadeOut(0.4f);
		wait(0.5f);
	}

	void WorldMapScreen::leftPressed(){
		if(cursorPos=='j'||cursorPos=='m') return;
		if(cursorPos=='k') cursorPos = 'j';
		else if(cursorPos=='h') cursorPos = 'k';
		else if(cursorPos=='i') cursorPos = 'k';
		else if(cursorPos=='l') cursorPos = 'h';
		else if(cursorPos=='n') cursorPos = 'm';
		moveCursor();
	}

	void WorldMapScreen::rightPressed(){
		if(cursorPos=='l') return;
		if(cursorPos=='k') cursorPos = 'h';
		else if(cursorPos=='h') cursorPos = 'l';
		else if(cursorPos=='i') cursorPos = 'l';
		else if(cursorPos=='m') cursorPos = 'n';
		else if(cursorPos=='n') cursorPos = 'l';
		else if(cursorPos=='j') cursorPos = 'k';

		moveCursor();
	}

	void WorldMapScreen::upPressed(){
		if(cursorPos=='j') return;
		if(cursorPos=='h') cursorPos = 'i';
		else if(cursorPos=='i') cursorPos = 'j';
		else if(cursorPos=='k') cursorPos = 'j';
		else if(cursorPos=='l') cursorPos = 'i';
		else if(cursorPos=='m') cursorPos = 'k';
		else if(cursorPos=='n') cursorPos = 'h';

		moveCursor();
	}

	void WorldMapScreen::downPressed(){
		if(cursorPos=='m') return;
		if(cursorPos=='h') cursorPos = 'n';
		else if(cursorPos=='i') cursorPos = 'h';
		else if(cursorPos=='k') cursorPos = 'm';
		else if(cursorPos=='l') cursorPos = 'n';
		else if(cursorPos=='j') cursorPos = 'k';
		else if(cursorPos=='n') cursorPos = 'm';
		moveCursor();
	}

	void WorldMapScreen::update(){
		if(!isWaiting){
			switch(chain){
			case ZYUG_CH_START: firstUpdate(); break;
			case ZYUG_CH_END: endUpdate(); break;
			default: break;
			}
		}else{
			continueWaiting();
		}
	}

	void WorldMapScreen::endUpdate(){
		cleanup();
		toNextScreen();
	}

	void WorldMapScreen::firstUpdate(){
		beginScreen(0.3f);
		chain = ZYUG_CH_ACTIVE;
		wait(0.3f);
	}

	void WorldMapScreen::render(){
		bg.render();
		legend.render();
		//puzzleCursor.render();
		renderText();
	}

	void WorldMapScreen::renderText(){
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = false;
		const char* text = WorldNames[cursorPos-'h'];
		textPrinter.printText(text, YUG_TEXT_INFO_FONT, 
			glm::vec3(0.25f, 0.7f, 1.0f), glm::vec3( 0.325f, 0.552f, -1.9998f),
			glm::mat4());
	}

	void WorldMapScreen::moveCursor(){
		glm::vec3 end;
		float z = -1.9f;
		switch(cursorPos){
		case 'h': end = glm::vec3(0.202f , -0.00f  ,z); break;
		case 'i': end = glm::vec3(0.202f ,  0.305f , z); break;
		case 'j': end = glm::vec3(-0.39f , 0.5f  , z); break;
		case 'k': end = glm::vec3(-0.2f, 0.13f  ,z); break;
		case 'l': end = glm::vec3(0.52f  , 0.16f  , z); break;
		case 'm': end = glm::vec3(-0.24f, -0.46f  , z); break;
		case 'n': end = glm::vec3(0.37f  , -0.315f  , z); break;
		default: end = glm::vec3(0,0,z); break;
		}
		puzzleCursor.cursorModel.interpolate(end, 0.2f);
	}

	void WorldMapScreen::setupbg(){
		bg.startup(
			YUG_PLANE_FILE_PATH, 
			"GameData/textures/map/worldmap_01.png");
		bg.doRender = true;
		bg.ignoreCamera = true;
		bg.scale = glm::vec3(0.83f, 0.65f, 1);
		bg.position = glm::vec3(0.0f, 0.00f, -2.0f);
	}

	void WorldMapScreen::setuplegend(){
		legend.startup(
			YUG_PLANE_FILE_PATH, 
			"GameData/textures/map/legend.png");
		legend.doRender = true;
		legend.ignoreCamera = true;
		legend.scale = glm::vec3(0.47f, 0.07f, 1);
		legend.position = glm::vec3(0.31f, 0.55f, -1.9999f);
	}

	void WorldMapScreen::setupcursor(){
		puzzleCursor.doRender = true;
	}

}
