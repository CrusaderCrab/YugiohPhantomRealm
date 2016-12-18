#include <Utility\clock.h>
#include <Utility\ErrorHandler.h>

namespace Utility{

	Clock Clock::clockInstance;
	Clock Clock::musClockInstance;

	bool Clock::initialize(){
		//get how many 'tick' in a second =/= clock speed, but should be constant
		if(! QueryPerformanceFrequency(&timeFrequency))
			return false;//if getting 'ticks' failed, quit
		//try to get performance once to see that it works
		return QueryPerformanceCounter(&lastStartTime);
	}

	void Clock::start(){
		QueryPerformanceCounter(&lastStartTime);
	}
	void Clock::stop(){
		LARGE_INTEGER delta;
		LARGE_INTEGER thisTime;
		QueryPerformanceCounter(&thisTime);
		//get time difference between new and old frame in 'ticks'
		delta.QuadPart= thisTime.QuadPart - lastStartTime.QuadPart;
		//get difference in seconds
		deltaTime = ((float)delta.QuadPart) / timeFrequency.QuadPart;
		deltaLastLap.QuadPart = thisTime.QuadPart;
		if(deltaTime == 0.0f){
			errorHandler.printError("Clock.cpp: Delta time == 0 warning");
		}
	}

	bool Clock::shutdown(){//for now, nothing to close
		return true;
	}
	void Clock::newLoop(){
		stop();
		start();
		

	}
	float Clock::lastLoopTime() const{
		return deltaTime;
	}

	void Clock::forceDelta(){
		for(int i = 0; i <100; i++){}
	}

}
