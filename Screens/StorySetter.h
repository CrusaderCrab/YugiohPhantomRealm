#pragma once
#include <Screens\BaseScreen.h>
#include <string>
namespace Screen{
class StorySetter
{
public:
	static BaseScreen* getNextScreen(int cursorPos);
	static std::string getNextFilePath(int cursorPos);
};

}