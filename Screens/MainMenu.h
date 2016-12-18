#pragma once
#include <Screens\BaseScreen.h>
#include <Game\ModelLoader.h>
#include <Screens\Panels\LoadPanel.h>

namespace Screen{

	class MainMenu : public BaseScreen
	{
	public:
		int chain;
		Game::ModelLoader bg;
		Game::ModelLoader newbutton;
		Game::ModelLoader loadButton;
		Game::ModelLoader optButton;
		LoadPanel loadPanel;
		int cursorPosition;

		void startup();
		void startup(const char* fileLocal);
		void cleanup();
		void render();
		void update();
		void input();
		void readFile(const char* fileLocal);
		void setupBG();
		void setupButtons();
		void firstUpdate();
		void fadeOutUpdate();
		void finishUpdate();
		void buttonPressed();
		void updateButtons();
		void loadPressed();
		void newPressed();
		void optionsPressed();
		virtual void toNextScreen();

	};
}