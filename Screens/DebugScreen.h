#pragma once
#include <Game\YugiohUnit.h>
#include <Game\WaitUnit.h>
#include <Screens\Panels\LoadPanel.h>

namespace Screen{

	class DebugScreen : public Game::YugiohUnit, Game::WaitUnit
	{
	public:
		int cursorPosition;
		int chain;
		bool blankBoard;
		LoadPanel loadPanel;

		void startup();
		void cleanup();
		void render();
		void input();
		void update();
		void toThisScreen();

		void setupCursor();
		void hideCursor();

		void newDeckCalled();
		void trunkCalled();
		void clearTrunkCalled();

		void moveCursorUp();
		void moveCursorDown();
		void xPressed();
		void triPressed();
		void specialPressed();

		static DebugScreen debugScreenInstance;
	};
#define debugScreen Screen::DebugScreen::debugScreenInstance
}