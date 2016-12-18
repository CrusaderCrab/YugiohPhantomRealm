#pragma once
#include <string>
#include <Screens\BaseScreen.h>
#include <Game\ModelLoader.h>

namespace Screen{

	class OptionsScreen : public BaseScreen
	{
	public:
		Game::ModelLoader bg;
		Game::ModelLoader cursor;
		Game::ModelLoader effectMarker;
		Game::ModelLoader musicMarker;
		int chain;
		int cursorPos;
		int effectPos;
		int musicPos;
		bool fromMainMenu;

		void startup(const char* fileLocal);
		void cleanup();
		void input();
		void render();
		void update();
		void toNextScreen();

		 void setupBG();
		 void setupCursor();
		 void setupMarkers();

		 void firstUpdate();
		 void finalUpdate();
		
		 void closeUpdate();

		 void xPressed();
		 void oPressed();
		 void sidePressed(bool left);

		 void resetKey();
		 void setSoundBool();
		 void updateCursorPos();

		 void printKeyValues();

		 static const char* kn[256];
		 static void setupNames();
		 
	};

}