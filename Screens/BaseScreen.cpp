#include <iostream>

#include <Utility\UpdateUnit.h>
#include <Utility\BlankUnit.h>
#include <Utility\StateUnit.h>
#include <Base\RenderUnit.h>

#include <Game\Animation\FadeUnit.h>
#include <Screens\BaseScreen.h>
#include <Screens\ScreenUnit.h>
#include <Screens\Trunk.h>

namespace Screen{

	void BaseScreen::fadeIn(float time){
		fadeUnit.changeZ(-1.0f);
		fadeUnit.fadeTo(YUG_FADE_BLACK_CLEAR, time);
	}

	void BaseScreen::fadeOut(float time){
		fadeUnit.changeZ(-1.0f);
		fadeUnit.fadeTo(YUG_FADE_BLACK, time);
	}

	void BaseScreen::startup(){
		Game::WaitUnit::startup();
		updateUnit.newUpdater(this);
		stateUnit.renderer->newRenderer(this);
		blankUnit.transitionInput(this, 0.5f);
	}

	void BaseScreen::refreshScreenUnit(){
		if(screenUnit.currentScreen != 0 && screenUnit.currentScreen != &trunkUnit){
			//std::cout<<"BaseScreen: Deleting old screen\n";
			delete screenUnit.currentScreen;
			screenUnit.currentScreen = 0;
		}
		if(screenUnit.comingScreen == this){
			screenUnit.currentScreen = this;
			screenUnit.comingScreen = 0;
		}
	}

	void BaseScreen::beginScreen(float time){
		fadeIn(time);
		refreshScreenUnit();
	}

	void BaseScreen::toNextScreen(){}

	void BaseScreen::toThisScreen(){
		startup();
	}

	void BaseScreen::toThisScreen(const char* nextFile){
		startup(nextFile);
	}

}
