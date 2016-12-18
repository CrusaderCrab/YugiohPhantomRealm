#pragma once
#include <string>
#include <vector>
#include <Screens\BaseScreen.h>
#include <Game\ModelLoader.h>
#include <DefinesAndTypedefs.h>
namespace Screen{
	class FreeDuelScreen : public BaseScreen
	{
	public:
		int chain;
		char miscInfo;
		Game::ModelLoader bg;
		Game::ModelLoader cursor;
		Game::ModelLoader scrollBar;
		Game::ModelLoader pics[YUG_NO_OF_FREE_DUELISTS+1];
		int vertPos;
		int horzPos;
		int screenLevel;//0-6
		std::string details;

		void startup();
		void startup(const char* fileLocal);
		void cleanup();
		void render();
		void input();
		void update();
		void toNextScreen();

		void xPressed();
		void oPressed();
		void directionPressed(int leftRight, int upDown);

		void setupBG();
		void setupCursor();
		void setupScrollBar();
		void setupPictures();
		void updateCursor();
		void updateBar();
		void updatePictures();

		void renderText();
		void renderPictures();

		void firstUpdate();
		void finalUpdate();
	};
}
