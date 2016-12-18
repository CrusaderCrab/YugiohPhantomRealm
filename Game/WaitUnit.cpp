#include <Game/WaitUnit.h>
#include <iostream>
#include <Utility/clock.h>

namespace Game{

	void WaitUnit::startup(){
		waitDuration = 0.0f;
		currentlyWaited = 0.0f;
		isWaiting = false;
	}

	void WaitUnit::wait(float duration){
		//if(isWaiting)
		//	std::cout<<"Wait unit: one wait time overriding another\n";
		waitDuration = duration;
		currentlyWaited = 0;
		isWaiting = true;

	}
	void WaitUnit::continueWaiting(){
		currentlyWaited += gameClock.lastLoopTime();
		if(currentlyWaited > waitDuration){
			isWaiting = false;
		}
	}






}