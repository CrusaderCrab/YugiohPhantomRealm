#include <qt\qapplication.h>
#include <iostream>
#include <QtGui\qwidget.h>
#include <Base\YugiohGame.h>


void printErrorThenWait(const char* error){
	std::cout<< error <<std::endl;
	std::cout<<"Enter a letter to continue..."<<std::endl;
	char ch; std::cin>>ch;
}

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	YugiohGame game;
	if(!game.initialize()){
		printErrorThenWait("Yugioh Game initialization failed");
		return -1;
	}
	game.takeAppPointer(&app);
	game.go();

	int errorCode =  app.exec();
	if(!game.shutdown()){
		printErrorThenWait("Yugioh Game shutdown failed");
	}
	return errorCode;


}