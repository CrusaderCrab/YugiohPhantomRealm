#include <Game\PlayerData.h>
#include <Utility\InputUnit.h>
#include <Utility\TextPrinter.h>
#include <Screens\Panels\LoadPanel.h>
#include <Utility\SoundUnit.h>
#include <DefinesAndTypedefs.h>

#define ZYUG_CH_IDLE 0
#define ZYUG_CH_START 1
#define ZYUG_CH_DOWN 2
#define ZYUG_CH_CHOOSE 3
#define ZYUG_CH_CONFIRM 4
#define ZYUG_CH_COMPLETE 5
#define ZYUG_CH_UP 6
#define ZYUG_CH_END 7
#define ZYUG_CH_CANCEL 8

namespace Screen{
	glm::vec3 topTextOffset(0,0,0);
	glm::vec3 centreTextOffset(0,0,0);
	glm::vec3 firstFile(0,0,0);
	float fileYOffset = 1.0f;
	char* top[] = {"LOAD","SAVE"};
	char* warning[] = {"LOAD THIS FILE", "OVERWRITE THIS FILE"};
	char* complete[] = {"FILE LOADED", "FILE SAVED"};

	void LoadPanel::startup(){
		Game::WaitUnit::startup();
		chain = ZYUG_CH_IDLE;
		rootChain = ZYUG_CH_IDLE;
		externalStatus = YUG_LOAD_PANEL_EXT_WORKING;
		chosenNumber = -1;
		cursorPos = 0;
		startupPanel();
		startupCursor();
		file1 = playerData.saveDescription(1);
		file2 = playerData.saveDescription(2);
		file3 = playerData.saveDescription(3);
	}

	void LoadPanel::cleanup(){
		panel.cleanup();
		cursor.cleanup();
		cursorPos = 0;
	}

	void LoadPanel::downUpdate(){
		chain = ZYUG_CH_CHOOSE;
		cursor.doRender = true;
		correctCursorPosition();
	}

	void LoadPanel::render(){
		panel.render();
		switch(chain){
		case ZYUG_CH_DOWN: chooseRender(); break;
		case ZYUG_CH_CHOOSE: chooseRender(); break;
		case ZYUG_CH_CONFIRM: confirmRender(); break;
		case ZYUG_CH_COMPLETE: completeRender(); break;
		}
		cursor.render();
	}

	void LoadPanel::update(){
		panel.update();
		cursor.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_CH_DOWN: downUpdate(); break;
			case ZYUG_CH_COMPLETE: doneUpdate(); break;
			case ZYUG_CH_CANCEL: cancelUpdate(); break;
			case ZYUG_CH_END: externalStatus = YUG_LOAD_PANEL_EXT_SUCCESS; break;
			default: break;
			}
		}else{
			continueWaiting();
		}
	}
	void LoadPanel::input(){
		if(!isWaiting){
			switch(chain){
			case ZYUG_CH_CHOOSE: chooseInput(); break;
			case ZYUG_CH_CONFIRM: confirmInput(); break;
			case ZYUG_CH_COMPLETE: doneInput(); break;
			default: break;
			}
		}
	}

	void LoadPanel::chooseInput(){
		if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
			cursorPos = (cursorPos+1)%3;
			soundUnit.cursorMove();
			correctCursorPosition();
			wait(0.2f);
		}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
			cursorPos = cursorPos-1;
			if(cursorPos < 0)cursorPos = 2;
			soundUnit.cursorMove();
			correctCursorPosition();
			wait(0.2f);
		}else if(inputUnit.isKeyActive(YUG_KEY_O)){
			chain = ZYUG_CH_CANCEL;
			soundUnit.cursorCancel();
			slideUp();
			wait(1.11f);
		}else if(inputUnit.isKeyActive(YUG_KEY_X)){
			if(saveMode || !playerData.isEmpty(cursorPos+1)){
				chain = ZYUG_CH_CONFIRM;
			}else{

			}
			soundUnit.cursorSelect();
			wait(0.2f);
		}
	}
	void LoadPanel::confirmInput(){
		if(inputUnit.isKeyActive(YUG_KEY_O)){
			soundUnit.cursorCancel();
			chain = ZYUG_CH_CHOOSE;
			wait(0.2f);
		}else if(inputUnit.isKeyActive(YUG_KEY_X)){
			if(saveMode){
				playerData.saveGame(cursorPos+1);
			}else{
				playerData.loadGame(cursorPos+1);
			}
			soundUnit.cursorSelect();
			chain = ZYUG_CH_COMPLETE;
			cursor.doRender = false;
			wait(0.2f);
		}
	}
	void LoadPanel::doneInput(){
		if(inputUnit.isKeyActive(YUG_KEY_X)){
			chain = ZYUG_CH_END;
			soundUnit.cursorSelect();
			slideUp();
			wait(1.11f);
		}
	}

//	void LoadPanel::downUpdate(){}
	void LoadPanel::chooseUpdate(){}
	void LoadPanel::doneUpdate(){}
	void LoadPanel::cancelUpdate(){
		externalStatus = YUG_LOAD_PANEL_EXT_FAIL;
		chain = ZYUG_CH_IDLE;
	}

	void LoadPanel::chooseRender(){
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;
		textPrinter.printText(top[saveMode], YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
			panel.position+glm::vec3(-0.45f,0.09f,0.007f), glm::mat4());

		textPrinter.printText(file1.c_str(), YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
			panel.position+glm::vec3(-0.45f,0.03f,0.007f), glm::mat4());
		textPrinter.printText(file2.c_str(), YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
			panel.position+glm::vec3(-0.45f,-0.03f,0.007f), glm::mat4());
		textPrinter.printText(file3.c_str(), YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
			panel.position+glm::vec3(-0.45f,-0.09f,0.007f), glm::mat4());

		cursor.render();

		textPrinter.leftAlign = false;
	}
	void LoadPanel::confirmRender(){
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;
		textPrinter.printText(warning[saveMode], YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
			panel.position+glm::vec3(-0.45f,0.09f,0.007f), glm::mat4());
		if(cursorPos==0){
			textPrinter.printText(file1.c_str(), YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
				panel.position+glm::vec3(-0.45f,0.03f,0.007f), glm::mat4());
		}else if(cursorPos==1){
			textPrinter.printText(file2.c_str(), YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
				panel.position+glm::vec3(-0.45f,-0.03f,0.007f), glm::mat4());
		}else{
			textPrinter.printText(file3.c_str(), YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
				panel.position+glm::vec3(-0.45f,-0.09f,0.007f), glm::mat4());
		}
		textPrinter.leftAlign = false;
	}
	void LoadPanel::completeRender(){
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = false;
		textPrinter.printText(complete[saveMode], YUG_TEXT_INFO_FONT, glm::vec3(0.3f,0.5f,1),
			panel.position+glm::vec3(0.0f,0.03f,0.007f), glm::mat4());
		textPrinter.leftAlign = false;
	}
		
	void LoadPanel::correctCursorPosition(){
		cursor.position = panel.position + glm::vec3(0,0.03f,0.01f);
		cursor.position.y -= cursorPos*0.06f;
	}

	void LoadPanel::activate(glm::vec3 startPos, glm::vec3 endPos){
		startPosition = startPos;
		endPosition = endPos;
		chain = ZYUG_CH_DOWN;
		slideDown();
		wait(1.4f);
	}

	void LoadPanel::startupPanel(){
		panel.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/loadPanel.png");
		panel.doRender = false;
		panel.ignoreCamera = true;
		panel.scale = glm::vec3(0.5f, 0.18f, 1.0f);
	}

	void LoadPanel::startupCursor(){
		cursor.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/models/positionunit/concursor.png");
		cursor.doRender = false;
		cursor.ignoreCamera = true;
		cursor.scale = glm::vec3(0.48f, 0.03f, 1.0f);
		cursor.amtran = glm::vec4(1.0f,1.0f,1.0f,0.5f);
	}

	void LoadPanel::slideDown(){
		panel.position = startPosition;
		panel.doRender = true;
		panel.interpolate(endPosition, 1.3f);
	}

	void LoadPanel::slideUp(){
		cursor.doRender = false;
		panel.interpolate(startPosition, 1.1f);
	}

}