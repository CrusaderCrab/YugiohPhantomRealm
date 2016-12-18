#pragma once

namespace Utility{
	class ErrorHandler
	{
		ErrorHandler(){}
		ErrorHandler(const ErrorHandler&);
		ErrorHandler& operator=(const ErrorHandler&);
	public:
		bool initialize();
		bool shutdown();
		void printError(const char* errorMessage);
		void printErrorAndQuit(const char* errorMessage);

		static ErrorHandler ErrorHandlerInstance;
	};
#define errorHandler Utility::ErrorHandler::ErrorHandlerInstance
}

