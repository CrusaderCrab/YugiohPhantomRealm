#include <Utility\InputUnit.h>
#include <Utility\BlankUnit.h>
#include <Utility\ErrorHandler.h>
#include <Utility\Clock.h>
#include <definesAndTypedefs.h>
#include <iostream>

namespace Utility{
	BlankUnit BlankUnit::blankUnitInstance;
	bool BlankUnit::initialize(){
		lockedScreen = false;
		timePaused = 0.0f;
		fullDuration = 0.0f;
		isIdling = false;
		return true;
	}
	bool BlankUnit::shutdown(){
		return true;
	}
	void BlankUnit::input(){
		if(lockedScreen && inputUnit.isKeyActive(YUG_KEY_SPECIAL))
			returnInput();
	}
	void BlankUnit::takeInput(){
		//if(inputUnit.oldInput != YUG_NULL_ID)
			//errorHandler.printError("Blank Unit: t1: InputUnit.oldInput not empty and being overwritten.");
		inputUnit.oldInput = inputUnit.currentInput;
		inputUnit.currentInput = this;
	}

	void BlankUnit::takeInput(Game::YugiohUnit* newInput){
		//if(inputUnit.oldInput != YUG_NULL_ID)
			//errorHandler.printError("Blank Unit: t2: InputUnit.oldInput not empty and being overwritten.");
		//std::cout<<"New input unit ready to slot in\n";
		heldInput = inputUnit.currentInput;
		inputUnit.oldInput = newInput;
		inputUnit.currentInput = this;
	}
	void BlankUnit::returnInput(){
		//std::cout<<"Blank Unit returnInput: passing input back\n";
		Game::YugiohUnit* temp = inputUnit.currentInput;
		inputUnit.currentInput = inputUnit.oldInput;
		inputUnit.oldInput = heldInput;
	}
	void BlankUnit::pauseInput(float duration){
		takeInput();
		timePaused = 0.0f;
		fullDuration = duration;
		isIdling = true;
	}
	void BlankUnit::transitionInput(Game::YugiohUnit* newInput, float duration){
		//std::cout<<"Blank Unit: start transition\n";
		takeInput(newInput);
		timePaused = 0.0f;
		fullDuration = duration;
		isIdling = true;
	}
	float timePaused;
	void BlankUnit::update(){
		if(isIdling){
			timePaused += gameClock.lastLoopTime();
			//std::cout<<"blank Unit update2\n";
			if( timePaused > fullDuration){
				returnInput();
				isIdling = false;
			}
		}
	}
	void BlankUnit::holdInputForNow(){
		isIdling = false;
		takeInput();
	}


}
