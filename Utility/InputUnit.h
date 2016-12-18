#pragma once
#include <Game\WaitUnit.h>

#define YUG_KEY_UP 0
#define YUG_KEY_DOWN 1
#define YUG_KEY_LEFT 2
#define YUG_KEY_RIGHT 3
#define YUG_KEY_X 4
#define YUG_KEY_O 5
#define YUG_KEY_TRI 6
#define YUG_KEY_SQU 7
#define YUG_KEY_START 8
#define YUG_KEY_SELECT 10
#define YUG_KEY_R1 9
#define YUG_KEY_L1 11
#define YUG_KEY_SPECIAL 12
#define YUG_NO_OF_KEYS 13
namespace Game{ class YugiohUnit; }
namespace Utility{
	class InputUnit : public Game::WaitUnit
	{
		/*int real_up;
		int real_down;
		int real_left;
		int real_right;
		int real_x;
		int real_o;
		int real_tri;
		int real_squ;
		int real_start;
		int real_select;
		int real_r1;
		int real_r2;
		int real_special;*/
		InputUnit() {}
		InputUnit(const InputUnit&);
		InputUnit& operator=(const InputUnit&);
	public:
		int realKeyBindings[YUG_NO_OF_KEYS];
		int setKeyBits;
		int compSetKeyBits;
		
		bool initialize();
		bool shutdown();

		void readKeys();
		bool isKeyActive(int key);
		void callCurrentInputTaker();

		void setKeyBinding(int key, int newKeyBinding);
		int listenForAnyKey();
		void resetKey(int key);
		void resetAllKeys();
		void takePlayerInputsOnly();
		void takeComputerInputsOnly();
		void swapInputAllowances();
		bool playerInputAllowed;
		bool computerInputAllowed;

		void computerKeyPress(int key);
		void releaseAllComputerKeys();

		Game::YugiohUnit* oldInput;
		Game::YugiohUnit* currentInput;

		static InputUnit inputUnitInstance;
	};
#define inputUnit Utility::InputUnit::inputUnitInstance
}

