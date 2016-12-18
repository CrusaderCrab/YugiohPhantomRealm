#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Screens\BaseScreen.h>
#include <Game\ModelLoader.h>


namespace Screen{

	class StoryScene : public BaseScreen
	{
	public:
		int chain;
		int id;
		float cursorTimer;
		int cursorTexPos;
		float textTimer;
		bool doRenderText;
		glm::vec4 textAmtran;
		Game::ModelLoader cursor;
		Game::ModelLoader yesNoCursor;
		GLuint cursorTexs[3];
		Game::ModelLoader bg;
		Game::ModelLoader textBox;
		std::vector<Game::ModelLoader> chars;
		std::vector<std::string> text;
		std::ifstream in;
		char key;
		char nextScreenKey;
		std::string nextFileString;
		std::string yesStr, noStr;
		int yesNoPos;
		std::string musicStr;

		bool isEarthquaking;
		float quakeTimer;
		float shakeTimer;
		void earthquakeUpdate();

		void startup(const char* fileLocal);
		void cleanup();
		void toNextScreen();
		void render();
		void input();
		void update();

		void setupBG(const char* local);
		void setupTextBox();
		void setupCursor();
		void setupYesNoCursor();

		void firstUpdate();
		void endUpdate();
		void cursorUpdate();

		void fadeTextInUpdate();
		void fadeTextOutUpdate();

		void fadeYesNoInUpdate();
		void fadeYesNoOutUpdate();
		void yesNoExitUpdate();

		void talkInput();
		void yesNoInput();

		void readNextPart();
		void renderText();

		void loadNewChar(); 
		void moveCharIn(); 
		void moveCharOut(); 
		void startYesNo(); 
		void jumpCharIn(); 
		void startNewText(); 
		void startFadeOut(); 
		void unlockDuelist(); 
		void assignPlotPoint();
		void startFadeIn(); 
		void hideBg();
		void showBg();
		void startDuel();
		void fadeOutBg();
		void changeBG();
		void readBattleMusicIn();
		void earthquake();
		void zoomInBg();
	};

}