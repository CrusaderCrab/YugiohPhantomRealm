#pragma once
#include <string>
#include <Game\YugiohUnit.h>
#include <Game\WaitUnit.h>
#define YUG_SCREEN_WINDOW_TEXTURE_PATH "GameData/textures/story/bg/textBox.png"
#define YUG_SCREEN_CH_IDLE 0
#define YUG_SCREEN_CH_START 1
#define YUG_SCREEN_BG_SIZE glm::vec3(0.82f, 0.599f, 1)
#define YUG_SCREEN_BG_POS glm::vec3(0.0f,0.1f, -2.00f)

namespace Screen{


	class BaseScreen : public Game::YugiohUnit, public Game::WaitUnit
	{
	public:
		std::string nextScreenFile;
		virtual void startup();
		virtual void startup(const char* fileLocal) =0;
		virtual void cleanup() =0;
		virtual void refreshScreenUnit();
		virtual void input() =0;
		virtual void render() =0;
		virtual void update() =0;
		virtual void beginScreen(float time);

		virtual void toThisScreen();
		virtual void toThisScreen(const char* nextFile);
		virtual void toNextScreen();
		void fadeIn(float time);
		void fadeOut(float time);
	};

}