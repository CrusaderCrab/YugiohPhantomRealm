#include <iostream>
#include <Screens\ScreenUnit.h>
#include <Game\Cursor.h>
#include <Game\Animation\FadeUnit.h>

namespace Screen{

	ScreenUnit ScreenUnit::screenUnitInstance;


	void ScreenUnit::startup(){
		currentScreen = comingScreen = winScreen = loseScreen = 0;
	}

	void ScreenUnit::startUpComingScreen( const char* fileLocal){
		if(!comingScreen){
			std::cout<<"ScreenUnit : sucs : comingScreen null\n";
			return;
		}
		puzzleCursor.doRender = false;
		fadeUnit.doRender = true;
		fadeUnit.changeZ(-1.7f);
		comingScreen->startup(fileLocal);
	}
}