#pragma once
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game\ModelLoader.h>
#include <Game\YugiohUnit.h>
#include <Game\Cards\CardData.h>

namespace Duel{

class LifepointsWidget : public Game::YugiohUnit
{
public:
	void startup();
	void cleanup();
	
	void update();
	void render();

	void newPlayerLP(
		int newLP,  
		float countdownDuration); 
	void newEnemyLP(
		int newLP, 
		float countdownDuration); 
	void continueCountdown();

	void reveal();
	void hide();

	float currentPlayerDuration;
	float totalPlayerDuration;
	float currentEnemyDuration;
	float totalEnemyDuration;

	int oldPlayerLP;
	int oldEnemyLP;
	int destinationPlayerLP;
	int destinationEnemyLP;
	int displayedPlayerLP;
	int displayedEnemyLP;
	bool countingPlayer;
	bool countingEnemy;

	Game::ModelLoader LPbackground;
};


}
