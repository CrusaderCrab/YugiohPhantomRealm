#include <iostream>
#include <utility\ErrorHandler.h>

namespace Utility{

	ErrorHandler ErrorHandler::ErrorHandlerInstance;

	bool ErrorHandler::initialize()
	{
		return true;
	}

	bool ErrorHandler::shutdown()
	{
		return true;
	}

	void ErrorHandler::printError(const char* errorMessage)
	{
		std::cout<<errorMessage<<std::endl;
	}

	void ErrorHandler::printErrorAndQuit(const char* errorMessage)
	{
		printError(errorMessage);
		std::cout<<"Press enter to quit"<<std::endl;
		char ch;
		std::cin>>ch;
		exit(1);
	}

}