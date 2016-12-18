#include <base\YugiohGame.h>
#include <Utility\ErrorHandler.h>
#include <Utility\Clock.h>
#include <Base\RenderUnit.h>
#include <Utility\StateUnit.h>
#include <Utility\SoundUnit.h>
#include <Utility\TextureLoader.h>
#include <Utility\TextPrinter.h>
#include <Utility\InputUnit.h>
#include <Utility\PartsUnit.h>
#include <Game\Animation\Camera.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\PlayerData.h>
#include <Utility\DebugUnit.h>
#include <Game\Duel\Board.h>
#include <Utility\BlankUnit.h>
#include <Utility\UpdateUnit.h>
#include <Game\Cards\CardDisplayUnit.h>
#include <Game\Duel\FusionUnit.h>
#include <Screens\Trunk.h>
#include <DefinesAndTypedefs.h>
#include <iostream>
#include <gtc\matrix_transform.hpp>
#include <Game\Cursor.h>
#include <Game\Duel\PositionUnit.h>
#include <Game\Duel\BattleUnit.h>
#include <Game\EnemyData.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Game\Animation\ParticlesUnit.h>

#include <Screens\ScreenUnit.h>
#include <Screens\SplashScreen.h>
#include <Utility\ConfigUnit.h>

bool YugiohGame::initialize(){

	if(!errorHandler.initialize())
		return false;
	if(!gameClock.initialize())
		return false;
	if(!musicClock.initialize())
		return false;
	if(!renderer.initialize())
		return false;
	if(! stateUnit.initialize())
		return false;
	if(! soundUnit.initialize())
		return false;
	if(! textureLoader.initialize())
		return false;
	if(! textPrinter.initialize())
		return false;
	if(! inputUnit.initialize())
		return false;
	if(! updateUnit.initialize())
		return false;
	if(! partsUnit.initialize())
		return false;
	if(! camera.initialize())
		return false;
	if(! debugUnit.initialize())
		return false;
	if(! blankUnit.initialize())
		return false;
	if(! pos.initialize())
		return false;
	if(! mov.initialize())
		return false;
	if(! theBoard.initialize())
		return false;
	if(! fadeUnit.initialize())
		return false;
	if(! cardDisplayUnit.initialize())
		return false;
	if(! fusionUnit.initialize())
		return false;
	if(! positionUnit.initialize())
		return false;
	if(! battleUnit.initialize())
		return false;
	if(! trunkUnit.initialize())
		return false;
	particleUnit.initialize();
	screenUnit.startup();
	playerData.initialize();

	if(! initializeCards())
		return false;
	if(! puzzleCursor.initialize())
		return false;
	if(! prepareGlobalPointers())
		return false;
	if(! prepareStartOfGame())
		return false;
	if( ! connect( 
		&myTimer, 
		SIGNAL(timeout()),
		this, 
		SLOT(update()) ) )
	{
		return false;
	}
	return true;
}

bool YugiohGame::shutdown(){
	bool success = true;
	success &= trunkUnit.shutdown();
	success &=  battleUnit.shutdown();
	success &=  positionUnit.shutdown();
	success &=  fusionUnit.shutdown();
	success &=  cardDisplayUnit.shutdown();
	success &=  fadeUnit.shutdown();
	success &=  mov.shutdown();
	success &=  pos.shutdown();
	success &=  puzzleCursor.shutdown();
	success &=  shutdownCards();

	particleUnit.shutdown();
	enemyData.cleanup();

	success &=  theBoard.shutdown();
	success &=  blankUnit.shutdown();
	success &=  debugUnit.shutdown();
	success &=  camera.shutdown();
	success &=  partsUnit.shutdown();
	success &=  updateUnit.shutdown();
	success &=  inputUnit.shutdown();
	success &=  textPrinter.shutdown();
	success &=  textureLoader.shutdown();
	success &=  soundUnit.shutdown();
	success &=  stateUnit.shutdown();
	success &=  renderer.shutdown();
	success &= musicClock.shutdown();
	success &=  gameClock.shutdown();
	success &=  errorHandler.shutdown();
	return true;
}

bool YugiohGame::prepareStartOfGame(){
	// Final Version: creates Start Screen, 
	// for now :creates Main Menu
	return true;
}
bool YugiohGame::prepareGlobalPointers(){
	stateUnit.renderer = &renderer;
	junkFunction();
	return true;

}

bool YugiohGame::initializeCards(){
	bool success = true;
	Card::CardRender r;
	success &= r.initialize();
	Card::BigCardRender b;
	success &= b.initialize();
	Card::SmallCardRender s;
	success &= s.initialize();
	return success;
}

bool YugiohGame::shutdownCards(){
	bool success = true;
	Card::SmallCardRender s;
	success &= s.shutdown();
	Card::BigCardRender b;
	success &= b.shutdown();
	Card::CardRender r;
	success &= r.shutdown();
	return success;
}


void YugiohGame::update(){//infinite game loop, broken by something sending .quit()/.end() to containing QApplication;
	gameClock.newLoop();
	inputUnit.readKeys();
	camera.update();
	inputUnit.callCurrentInputTaker();
	updateUnit.update();
	puzzleCursor.update();
	blankUnit.update();
	renderer.render();
	fadeUnit.update();
	soundUnit.update();
}

void YugiohGame::takeAppPointer(QApplication* app){
	containingApp = app;
	stateUnit.takeAppPointer(app);
}

void YugiohGame::go(){
	myTimer.start();
}

void YugiohGame::junkFunction(){
	
	playerData.loadGame(1);
	playerData.currentSaveFile = YUG_PLAYER_DATA_NO_ACTIVE_SAVE_FILE;
	enemyData.startup();

	Utility::ConfigUnit config;
	config.loadConfig();

	screenUnit.comingScreen = new Screen::SplashScreen();
	screenUnit.startUpComingScreen(
		"GameData/Story/splashScreens/titleCardSplash.txt");/**/
}


