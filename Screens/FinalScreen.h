#pragma once
#include <Screens\BaseScreen.h>
#include <Screens\Panels\LoadPanel.h>

namespace Screen{

	class FinalScreen : public BaseScreen
	{
	public:
		int chain;
		LoadPanel loadPanel;

		void startup();
		void startup(const char* fileLocal);
		void cleanup();
		void render();
		void update();
		void input();
		void toNextScreen();

		void firstUpdate();
		void endUpdate();
	};

}