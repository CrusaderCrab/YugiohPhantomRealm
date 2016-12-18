#pragma once
#include <string>
#include <vector>
#include <Screens\BaseScreen.h>
#include <Game\ModelLoader.h>
namespace Screen{

	class NameScreen : public BaseScreen
	{
	public:
		int chain;
		Game::ModelLoader bg;
		Game::ModelLoader cursor;
		GLuint textures[3];
		int vertPos;
		int horzPos;
		int specialPos;
		int textureNo;
		std::string name;
		float cursorTimer;

		void startup();
		void startup(const char* fileLocal);
		void cleanup();
		void render();
		void update();
		void input();
		void toNextScreen();

		void xPressed();
		void startPressed();
		void oPressed();

		void renderText();

		void firstUpdate();
		void finalUpdate();
		void updateCursorPos();
		void animateCursor();
		void moveCursorPos(int upDown, int leftRight);

		void setupBg();
		void setupCursor();
		void setupTextures();

		char getCorrectChar();
		void addLetter(char c);
		void removeLetter();

	};

}