#include <Game\YugiohUnit.h>
#include <Utility\ErrorHandler.h>
#include <Utility\UpdateUnit.h>
#include <Game\Cards\CardDisplayUnit.h>

#define Y_NULL 0
namespace Utility{

	UpdateUnit UpdateUnit::updateUnitInstance;

	bool UpdateUnit::initialize(){
		oldUpdater = Y_NULL;
		currentUpdater = Y_NULL;
		return true;
	}
	bool UpdateUnit::shutdown(){
		return true;
	}

	void UpdateUnit::newUpdater(Game::YugiohUnit* newUpdater){
		if(newUpdater == Y_NULL){
			errorHandler.printError("UpdateUnit: newUpdater: NULL passed, ignoring it.");
		}else{
			oldUpdater = currentUpdater;
			currentUpdater = newUpdater;
		}

	}
	void UpdateUnit::swapUpdaters(){
		if(oldUpdater == Y_NULL){
			errorHandler.printError("UpdateUnit: swapUpdater: oldUpdater is NULL, ignoring it.");
		}else{
			Game::YugiohUnit* temp = oldUpdater;
			oldUpdater = currentUpdater;
			currentUpdater = temp;
		}
	}
	void UpdateUnit::returnToOldUpdater(){
		if(oldUpdater == Y_NULL){
			errorHandler.printError("UpdateUnit: return Updater: oldUpdater is NULL, ignoring it.");
		}else{
			currentUpdater = oldUpdater;
		}
	}


	void UpdateUnit::setOldUpdater(Game::YugiohUnit* newUpdater){
		if(newUpdater == Y_NULL){
			errorHandler.printError("UpdateUnit: new oldUpdater: passed Updater is NULL, ignoring it.");
		}else{
			oldUpdater = newUpdater;
		}

	}
	void UpdateUnit::update(){
		if(currentUpdater != Y_NULL)
			currentUpdater->update();
		else
			errorHandler.printError("UpdateUnit: update: currentUpdater is null, ignoring it");
		cardDisplayUnit.update();
	}


}
