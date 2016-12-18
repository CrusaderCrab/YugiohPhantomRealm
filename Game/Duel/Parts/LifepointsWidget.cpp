#include <Game\VectorUnit.h>
#include <Utility\Clock.h>
#include <Utility\TextPrinter.h>
#include <Game\Duel\Parts\LifepointsWidget.h>
#include <DefinesAndTypedefs.h>
#include <Game\Duel\Board.h>
#include <Game\Animation\MovementUnit.h>

namespace Duel{

	void LifepointsWidget::startup(){
		LPbackground.startup(YUG_PLANE_FILE_PATH, "GameData/textures/board/lifepointsWidgetUV.png");
		LPbackground.scale = pos.bLPMModelScale;
		LPbackground.position = pos.bLPModelPos;
		LPbackground.ignoreCamera = true;
		displayedEnemyLP = 0;
		newEnemyLP(8000, pos.wait[9]);
		displayedPlayerLP = 0;
		newPlayerLP(8000, pos.wait[9]);
	}
	void LifepointsWidget::cleanup(){
		LPbackground.cleanup();
	}
	
	void LifepointsWidget::update(){
		continueCountdown();
		LPbackground.update();
	}
	void LifepointsWidget::render(){
		LPbackground.render();

		textPrinter.leftAlign = true;
		textPrinter.ignoreCamera = true;
		//printing lp
		textPrinter.printNumber(displayedPlayerLP,YUG_TEXT_INFO_FONT,
			pos.bLPLPNumScale, mov.addOffset(LPbackground.position,pos.bLPPlayerLPOffset), glm::mat4());
		textPrinter.printNumber(displayedEnemyLP,YUG_TEXT_INFO_FONT,
			pos.bLPLPNumScale, mov.addOffset(LPbackground.position,pos.bLPEnemyLPOffset), glm::mat4());
		//printing deck
		textPrinter.printNumber(theBoard.player.deck.cardsLeft,
			YUG_TEXT_INFO_FONT, pos.bLPDeckScale, mov.addOffset(LPbackground.position,pos.bLPPlayerCardOffset), glm::mat4());
		textPrinter.printNumber(theBoard.enemy.deck.cardsLeft,
			YUG_TEXT_INFO_FONT, pos.bLPDeckScale, mov.addOffset(LPbackground.position,pos.bLPEnemyCardOffset), glm::mat4());
		textPrinter.ignoreCamera = false; //QWERTY
	}
	
	void LifepointsWidget::newPlayerLP(
			int newLP,  
			float countdownDuration)
	{
		if(newLP < 0)
			newLP = 0;
		if(newLP > 9999)
			newLP = 9999;
		countingPlayer = true;
		currentPlayerDuration = 0.0f;
		totalPlayerDuration = countdownDuration;
		oldPlayerLP = displayedPlayerLP;
		destinationPlayerLP = newLP;
		oldPlayerLP = displayedPlayerLP;
		destinationPlayerLP = newLP;
	}
	void LifepointsWidget::newEnemyLP(
			int newLP,  
			float countdownDuration)
	{
		countingEnemy = true;
		currentEnemyDuration = 0.0f;
		totalEnemyDuration = countdownDuration;
		oldEnemyLP = displayedEnemyLP;
		destinationEnemyLP = newLP;
		oldEnemyLP = displayedEnemyLP;
		destinationEnemyLP = newLP;
	}
	void LifepointsWidget::continueCountdown(){
		if(countingEnemy){
			currentEnemyDuration += gameClock.lastLoopTime();
			if(currentEnemyDuration < totalEnemyDuration){
				displayedEnemyLP = oldEnemyLP 
					+ ((destinationEnemyLP-oldEnemyLP) //OPTIMIZE: can be replaced by 1/total, using 0->1 counting
						* (currentEnemyDuration/totalEnemyDuration));
			}else{
				countingEnemy = false;
				displayedEnemyLP = destinationEnemyLP;
			}
		}
		if(countingPlayer){
			currentPlayerDuration += gameClock.lastLoopTime();
			if(currentPlayerDuration < totalPlayerDuration){
				displayedPlayerLP = oldPlayerLP 
					+ ((destinationPlayerLP-oldPlayerLP) //OPTIMIZE: can be replaced by 1/total, using 0->1 counting
						* (currentPlayerDuration/totalPlayerDuration));
			}else{
				countingPlayer = false;
				displayedPlayerLP = destinationPlayerLP;
			}
		}
	}
	
	void LifepointsWidget::reveal(){
		LPbackground.interpolate(pos.bLPModelPos, pos.wait[3]);
	}
	void LifepointsWidget::hide(){
		LPbackground.interpolate(pos.bLPModelOffPos, pos.wait[3]);
	}

}