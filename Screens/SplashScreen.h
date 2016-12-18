#pragma once
#include <Screens\BaseScreen.h>
#include <Game\ModelLoader.h>

namespace Screen{

	class SplashScreen : public BaseScreen
	{
	public:
		int chain;
		Game::ModelLoader picture;
		float inTime;
		float outTime;
		bool skipPressed;
		void startup(const char* fileLocal);
		void cleanup();
		void render();
		void update();
		void input();
		void readFile(const char* fileLocal);
		void setupPicture(const char* fl);
		void firstUpdate();
		void fadeOutUpdate();
		void finishUpdate();
		virtual void toNextScreen();
	};
}