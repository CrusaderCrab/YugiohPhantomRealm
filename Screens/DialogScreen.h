#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <Game\ModelLoader.h>
#include <Screens\BaseScreen.h>

#define YUG_SCREEN_WINDOW_SIZE glm::vec3(0.81f,0.217f,1)
#define YUG_SCREEN_WINDOW_POS glm::vec3(0,-0.444f, -1.996f)
#define YUG_SCREEN_CHAR_REG_SCALE glm::vec3(0.65f, 0.45f, 1)
#define YUG_SCREEN_CHAR_REG_POS glm::vec3(-0.2f, 0.2f, -1.998f)


namespace Screen{

	class DialogScreen : public BaseScreen
	{
	public:
		Game::ModelLoader bgModel;
		Game::ModelLoader charModel;
		Game::ModelLoader windowModel;
		//Next button
		std::vector<std::string> dialogStrings;
		int dialogDepth;

		int comingScreenID;
		int chain;
		bool waitingForInput;
		float timer;

		void startup();
		virtual void startup(const char* fileLocal);
		virtual void cleanup();
		virtual void input();
		virtual void render();
		virtual void update();

		//virtual void fadeIn();
		virtual void readFile(const char* fileLocal);
		virtual void setupBg(const char* texLocal);
		virtual void setupChar(const char* texLocal);
		virtual void setupWindow(const char* texLocal);

		char* getCurrentDialog();
		void toNextDialog();

		virtual void firstUpdate();
		virtual void textInUpdate();
		virtual void inputUpdate();
		virtual void textOutUpdate();
		virtual void endUpdate();

	};

}