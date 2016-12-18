#pragma once
#include <Game\ModelLoader.h>
#include <Game\YugiohUnit.h>
#include <Game\WaitUnit.h>
#define YUG_LOAD_PANEL_EXT_IDLE 0
#define YUG_LOAD_PANEL_EXT_WORKING 1
#define YUG_LOAD_PANEL_EXT_SUCCESS 2
#define YUG_LOAD_PANEL_EXT_FAIL 3
namespace Screen{
	class LoadPanel : public Game::YugiohUnit, public Game::WaitUnit
	{
	public:
		LoadPanel(): 
			saveMode(false), externalStatus(YUG_LOAD_PANEL_EXT_IDLE){}
		Game::ModelLoader panel;
		Game::ModelLoader cursor;
		int cursorPos;
		int chain;
		int rootChain;
		bool saveMode;
		int externalStatus;
		int chosenNumber;
		glm::vec3 startPosition;
		glm::vec3 endPosition;
		std::string file1, file2, file3;
		std::string* chosenFileString;

		void startup();
		void cleanup();
		void render();
		void input();
		void update();

		void startupPanel();
		void startupCursor();
		void activate(glm::vec3 startPos, glm::vec3 endPos);
		//void deactivate();
		void renderCorrectText();
		void cursorUp();
		void cursorDown();
		void cancel();
		void fileSelected();
		void slideUp();
		void slideDown();
		void correctCursorPosition();

		void downUpdate();
		void chooseUpdate();
		void doneUpdate();
		void cancelUpdate();

		void chooseInput();
		void confirmInput();
		void doneInput();

		void chooseRender();
		void confirmRender();
		void completeRender();
	};
}
