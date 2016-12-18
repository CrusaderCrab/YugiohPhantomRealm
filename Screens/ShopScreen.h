#pragma once
#include <vector>
#include <string>
#include <Screens\BaseScreen.h>
#include <Game\ModelLoader.h>
#include <Screens\Panels\LoadPanel.h>

namespace Screen{
	class ShopScreen : public BaseScreen
	{
	public:
		int chain;
		int cursorPos;
		char miscInfo;
		float buttonTimer;
		Game::ModelLoader bg;
		Game::ModelLoader textBox;
		Game::ModelLoader cursor;
		Game::ModelLoader button;
		LoadPanel loadPanel;
		GLuint buttonTex[3];
		int buttonTexPos;
		std::vector<std::string> text;
		glm::vec4 textAmtran;
		bool doRenderText;
		float textTimer;

		void startup(const char* fileLocal);
		void cleanup();
		void toNextScreen();
		void input();
		void render();
		void update();

		void firstUpdate();
		void preEndUpdate();
		void endUpdate();
		void buttonUpdate();

		void renderText();
		void fadeTextInUpdate();
		void fadeTextOutUpdate();

		void normalInputs();
		void xPressed();
		void upPressed();
		void downPressed();

		void setupbg();
		void setupcursor();
		void setupbutton();
		void setupTextBox();

	};
}