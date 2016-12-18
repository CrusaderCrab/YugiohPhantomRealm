#include <Screens\TempleMapScreen.h>
#include <Screens\StorySetter.h>
#include <Utility\InputUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Cursor.h>
#include <Screens\ScreenUnit.h>
#include <DefinesAndTypedefs.h>
#include <Utility\SoundUnit.h>
#define ZYUG_CH_START 0
#define ZYUG_CH_ACTIVE 1
#define ZYUG_CH_IDLE 2
#define ZYUG_CH_END 3

namespace Screen{

	const char* templeNames[] = {
		"Cell","Shop","Arena","Exit","?"
	};

	void TempleMapScreen::toNextScreen(){
		BaseScreen* cs = StorySetter::getNextScreen(cursorPos);
		std::string fs = StorySetter::getNextFilePath(cursorPos);
		screenUnit.comingScreen = cs;
		screenUnit.startUpComingScreen(fs.c_str());
	}

	void TempleMapScreen::startup(const char* fileLocal){
		BaseScreen::startup();
		cursorPos = fileLocal[0];
		chain = ZYUG_CH_START;
		setupbg();
		setuplegend();
		setupcursor();
		moveCursor();
		soundUnit.playLoop("GameData/sounds/music/ready/mapMusic_o.wav");
	}

	void TempleMapScreen::cleanup(){
		puzzleCursor.doRender = false;
		bg.cleanup();
		legend.cleanup();
		soundUnit.stopAll();
	}

	void TempleMapScreen::input(){
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

	void TempleMapScreen::xPressed(){
		chain = ZYUG_CH_END;
		fadeOut(0.4f);
		wait(0.5f);
	}

	void TempleMapScreen::leftPressed(){
		if(cursorPos=='a' || cursorPos=='c'){ return; }
		if(cursorPos=='d'){
			cursorPos = 'c';
		}else if(cursorPos=='b'){
			cursorPos = 'a';
		}else if(cursorPos=='e'){
			cursorPos = 'b';
		}

		moveCursor();
	}

	void TempleMapScreen::rightPressed(){
		if(cursorPos=='d' || cursorPos=='e'){ return; }
		if(cursorPos=='a'){
			cursorPos = 'b';
		}else if(cursorPos=='c'){
			cursorPos = 'b';
		}else if(cursorPos=='b'){
			cursorPos = 'e';
		}

		moveCursor();
	}

	void TempleMapScreen::upPressed(){
		if(cursorPos=='a' || cursorPos=='e'){ return; }
		if(cursorPos=='d'){
			cursorPos = 'c';
		}else if(cursorPos=='b'){
			cursorPos = 'e';
		}else if(cursorPos=='c'){
			cursorPos = 'a';
		}

		moveCursor();
	}

	void TempleMapScreen::downPressed(){
		if(cursorPos=='d'){ return; }
		if(cursorPos=='a'){
			cursorPos = 'c';
		}else if(cursorPos=='c' || cursorPos=='b'){
			cursorPos = 'd';
		}else if(cursorPos=='e'){
			cursorPos = 'b';
		}

		moveCursor();
	}

	void TempleMapScreen::update(){
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

	void TempleMapScreen::endUpdate(){
		cleanup();
		toNextScreen();
	}

	void TempleMapScreen::firstUpdate(){
		beginScreen(0.3f);
		chain = ZYUG_CH_ACTIVE;
		wait(0.3f);
	}

	void TempleMapScreen::render(){
		bg.render();
		legend.render();
		//puzzleCursor.render();
		renderText();
	}

	void TempleMapScreen::renderText(){
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = false;
		const char* text = templeNames[cursorPos-'a'];
		textPrinter.printText(text, YUG_TEXT_INFO_FONT, 
			glm::vec3(0.3f, 0.7f, 1.0f), glm::vec3( 0, 0.552f, -1.9998f),
			glm::mat4());
	}

	void TempleMapScreen::moveCursor(){
		glm::vec3 end;
		float z = -1.9f;
		switch(cursorPos){
		case 'a': end = glm::vec3(-0.53f, 0.3f, z); break;
		case 'b': end = glm::vec3( 0.05f, 0.18f, z); break;
		case 'c': end = glm::vec3(-0.69f,-0.13f, z); break;
		case 'd': end = glm::vec3(-0.2f,-0.53f, z); break;
		case 'e': end = glm::vec3(0.34f, 0.33f, z); break;
		default: end = glm::vec3(0,0,z); break;
		}
		puzzleCursor.cursorModel.interpolate(end, 0.2f);
	}

	void TempleMapScreen::setupbg(){
		bg.startup(
			YUG_PLANE_FILE_PATH, 
			"GameData/textures/map/templemap_01.png");
		bg.doRender = true;
		bg.ignoreCamera = true;
		bg.scale = glm::vec3(0.83f, 0.65f, 1);
		bg.position = glm::vec3(0.0f, 0.00f, -2.0f);
	}

	void TempleMapScreen::setuplegend(){
		legend.startup(
			YUG_PLANE_FILE_PATH, 
			"GameData/textures/map/legend.png");
		legend.doRender = true;
		legend.ignoreCamera = true;
		legend.scale = glm::vec3(0.6f, 0.07f, 1);
		legend.position = glm::vec3(0.0f, 0.55f, -1.9999f);
	}

	void TempleMapScreen::setupcursor(){
		puzzleCursor.doRender = true;
	}

}