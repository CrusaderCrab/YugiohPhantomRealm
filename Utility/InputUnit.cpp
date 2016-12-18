#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <qt\qapplication.h>
#include <Game\YugiohUnit.h>
#include <Utility\InputUnit.h>
#include <Utility\StateUnit.h>
#include <Base\RenderUnit.h>
#include <iostream>
#include <Utility\SoundUnit.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

namespace Utility{



	InputUnit InputUnit::inputUnitInstance;


	bool InputUnit::initialize(){
		resetAllKeys();
		takePlayerInputsOnly();
		Game::WaitUnit::startup();
		return true;
	}

	bool InputUnit::shutdown(){
		return true;
	}

	void InputUnit::readKeys(){
		if(stateUnit.application->activeWindow() == 0)
			return;

		setKeyBits = 0;
		int possibleAction = 1;
		for(int i = 0; i < YUG_NO_OF_KEYS; i++){
			if( GetAsyncKeyState(realKeyBindings[i]) & 0x8000){
				setKeyBits |= possibleAction;
			}
			possibleAction <<= 1;
		}
	}

	void InputUnit::computerKeyPress(int key){
		int action = 1;
		action <<= key;//move bit to this keys position
		compSetKeyBits |= action;
	}

	bool InputUnit::isKeyActive(int key){
		int setKeys;
		if(playerInputAllowed)
			setKeys = (setKeyBits >> key);
		else
			setKeys = (compSetKeyBits >> key);
		return (setKeys%2);
	}

	void InputUnit::releaseAllComputerKeys(){
		compSetKeyBits = 0;
	}

	void InputUnit::callCurrentInputTaker(){
		//std::cout<<"Calling current Inputer\n";
		if(!isWaiting){
			currentInput->input();
		}else{
			continueWaiting();
		}
	}

	void InputUnit::setKeyBinding(int key, int newKeyBinding){

	}
	int InputUnit::listenForAnyKey(){
		int count = 0;
		std::cout<<"Input: Hold down new key binding\n";
		while(count++ < 30){
			std::this_thread::sleep_for (std::chrono::milliseconds(300));
			for (int i = 0; i < 256; i++)
			{
				if(GetAsyncKeyState(i) & 0x8000){
					soundUnit.cursorSelect();
					std::cout<<"Input: New key binding obtained\n";
					return i;
				}
			}
		}
		std::cout<<"Input: No new key binding obtained.\n";
		soundUnit.cursorSelectionFail();
		return -1;
	}
	void InputUnit::resetKey(int key){
		int res = listenForAnyKey();
		if(res==-1)return;
		for(int i = 0; i <YUG_NO_OF_KEYS; i++){
			if(res==realKeyBindings[i])return;
		}
		realKeyBindings[key] = res;
	}
	void InputUnit::resetAllKeys(){
		realKeyBindings[YUG_KEY_UP] = VkKeyScan('w');
		realKeyBindings[YUG_KEY_DOWN] = VkKeyScan('s');
		realKeyBindings[YUG_KEY_LEFT] = VkKeyScan('a');
		realKeyBindings[YUG_KEY_RIGHT] = VkKeyScan('d');
		realKeyBindings[YUG_KEY_X] = VK_DOWN;
		realKeyBindings[YUG_KEY_O] = VK_RIGHT;
		realKeyBindings[YUG_KEY_TRI] = VK_UP;
		realKeyBindings[YUG_KEY_SQU] = VK_LEFT;
		realKeyBindings[YUG_KEY_START] = VK_SPACE;
		realKeyBindings[YUG_KEY_SELECT] = VK_BACK;
		realKeyBindings[YUG_KEY_R1] = VkKeyScan('r');
		realKeyBindings[YUG_KEY_L1] = VkKeyScan('l');
		realKeyBindings[YUG_KEY_SPECIAL] = VK_NUMPAD0;
	}

	void InputUnit::takePlayerInputsOnly(){
		playerInputAllowed = true;
		computerInputAllowed = false;
	}
	void InputUnit::takeComputerInputsOnly(){
		playerInputAllowed = false;
		computerInputAllowed = true;
	}
	void InputUnit::swapInputAllowances(){
		bool temp = playerInputAllowed;
		playerInputAllowed = computerInputAllowed;
		computerInputAllowed = temp;
	}
}
