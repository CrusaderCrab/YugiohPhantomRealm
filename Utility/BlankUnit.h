#pragma once
#include <Game\YugiohUnit.h>
namespace Utility{

	class BlankUnit : public Game::YugiohUnit
	{
		BlankUnit(){}
		BlankUnit(const BlankUnit&);
		BlankUnit& operator=(const BlankUnit&);
	public:
		Game::YugiohUnit* heldInput;
		bool lockedScreen;
		bool initialize();
		bool shutdown();
		void input();
		void takeInput(Game::YugiohUnit* newInput);
		void takeInput();
		void returnInput();
		void pauseInput(float duration);
		void transitionInput(Game::YugiohUnit* newInput, float duration);
		void holdInputForNow();
		float timePaused;
		float fullDuration;
		bool isIdling;
		void update();
		static BlankUnit blankUnitInstance;
	};
#define blankUnit Utility::BlankUnit::blankUnitInstance
}