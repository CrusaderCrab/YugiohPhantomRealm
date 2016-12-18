#pragma once

#include <Windows.h>//for 

namespace Utility
{	//tells compiler that we want to export this class, makes .lib file for our .dll
	class Clock
	{
		LARGE_INTEGER timeFrequency;
		LARGE_INTEGER lastStartTime;
		LARGE_INTEGER deltaLastLap;
		float deltaTime;//time delta in seconds, but shouldn't hold more then a second
	public:
		bool initialize();
		bool shutdown();
		void start();
		void stop();
		void forceDelta();
		void newLoop();
		//time between the start of one frame to the next
		float lastLoopTime() const;
		static Clock clockInstance;
		static Clock musClockInstance;
	};
#define gameClock Utility::Clock::clockInstance
#define musicClock Utility::Clock::musClockInstance
#define gameDeltaTime Utility::Clock::clockInstance.lastLoopTime()
}

