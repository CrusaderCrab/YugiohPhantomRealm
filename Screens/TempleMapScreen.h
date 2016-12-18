#pragma once
#include <Screens\BaseScreen.h>
#include <Game\ModelLoader.h>
namespace Screen{

	class TempleMapScreen : public BaseScreen
	{
	public:
		int chain;
		Game::ModelLoader bg;
		Game::ModelLoader legend;
		char cursorPos;

		void startup(const char* fileLocal);
		void cleanup();
		void render();
		void input();
		void update();
		void toNextScreen();

		void firstUpdate();
		void endUpdate();

		void xPressed();
		void leftPressed();
		void rightPressed();
		void upPressed();
		void downPressed();

		void renderText();

		void moveCursor();
		void toCellPos(); //a
		void toShopPos(); //b
		void toDuelPos(); //c
		void toExitPos(); //d
		void toDoorPos(); //e

		void callStorySetter();

		void setupbg();
		void setuplegend();
		void setupcursor();
	};


}