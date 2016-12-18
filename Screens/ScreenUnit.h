#pragma once
#include <Screens\BaseScreen.h>
#include <string>

namespace Screen{

	class ScreenUnit
	{
	public:
		BaseScreen* currentScreen;
		BaseScreen* comingScreen;
		BaseScreen* winScreen;
		BaseScreen* loseScreen;

		std::string winStr;
		std::string loseStr;

		void startup();
		void startUpComingScreen( const char* fileLocal);



		static ScreenUnit screenUnitInstance;
	};
#define screenUnit Screen::ScreenUnit::screenUnitInstance
}