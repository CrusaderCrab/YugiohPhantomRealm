#include <Game\Duel\BattleUnit.h>
#include <Game\Duel\Board.h>
#include <Utility\Clock.h>
#include <Utility\BlankUnit.h>
#include <Utility\StateUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Cursor.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Game\Duel\EndUnit.h>
#include <Game\EnemyData.h>
#include <iostream>

namespace Duel{

	BattleUnit BattleUnit::battleUnitInstance;

	bool BattleUnit::initialize(){
		chain = YUG_BAT_CH_IDLE;
		callFadeUnit = false;
		atkCardDead = false;
		defCardDead = false;
		doRender = false;
		doUpdate = false;
		isWaiting = false;
		printDamage = false;
		printOnDefender = false;
		textFading = false;
		waitDuration = 0.0f;
		currentlyWaited = 0.0f;

		atkCardOffPosition = glm::vec3(-3.0f,0.0f,-0.8f);
		atkCardOnPosition = glm::vec3(-0.15f,0.0f,-0.8f);
		defCardOffPosition = glm::vec3(3.0f,0.0f,-0.8f);
		defCardOnPosition = glm::vec3(0.15f,0.0f,-0.8f);
		textOffset =  glm::vec3(0.15f,0.0f,-0.75f);
		cardScale = glm::vec3(0.3f,0.3f,1.0f);

		initializeGlow();
		initializeSlash();
		initializeFlash();
		return true;
	}
	bool BattleUnit::shutdown(){
		shutdownModels();
		return true;
	}
	void BattleUnit::startBattle(bool directAtk){
		directAttack = directAtk;
		doRender = true;
		doUpdate = true;
		atkCardDead = false;
		defCardDead = false;
		atkCard = theBoard.board[theBoard.aP[0]][theBoard.aP[1]].bigRender;
		theBoard.board[theBoard.aP[0]][theBoard.aP[1]].attacked();
		atkCard.startup();
		atkCard.doRender = false;
		setUpBigAttacker();
		atkGuardianBoost = false;
		defGuardianBoost = false;
		atkCard.parentCard->altAttack = atkCard.parentCard->attack;
		if(!directAttack){
			defCard = theBoard.currentCard.bigRender;
			defCard.startup();
			defCard.doRender = false;
			setUpBigDefender();
			defenderDefending = !(defCard.parentCard->attackMode);
			setupCons();
		}else{
			atkCard.displayedAttack = atkCard.parentCard->attack;
		}
		fillInParts();
		blankUnit.holdInputForNow();
		chain = YUG_BAT_CH_STARTSET;
		theBoard.chain = YUG_BOARD_CH_BATTLEUNIT;
		wait(0.2f);
	}

	void BattleUnit::setupCons(){
		defCard.parentCard->altDefense = defCard.parentCard->defense;
		defCard.parentCard->altAttack = defCard.parentCard->attack;
		atkCard.parentCard->altAttack = atkCard.parentCard->attack;
		if(atkCard.parentCard->versusConstellation(defCard.parentCard->currentConstellation)
			== YUG_STRONG_AGAINST){
			atkCard.parentCard->altAttack = atkCard.parentCard->attack+500;
			atkGuardianBoost = true;
		}else if(atkCard.parentCard->versusConstellation(defCard.parentCard->currentConstellation)
			== YUG_WEAK_AGAINST){
			if(defenderDefending){
				defCard.parentCard->altDefense = defCard.parentCard->defense + 500;
				defGuardianBoost = true;
			}else{
				defCard.parentCard->altAttack = defCard.parentCard->attack + 500;
				defGuardianBoost = true;
			}
		}
	}

	void BattleUnit::positionCards(){
		/* */
	}
	void BattleUnit::update(){
		if(doUpdate){
			atkCard.update();
			defCard.update();
			glow.update();
			slash.update();
			flash.update();
			if(textFading){
				fadeTextUpdate();
			}
			if(shakeCardVar != ZYUG_SHAKE_NONE)
				shakeCardUpdate();
			if(!isWaiting){
				if(chain == YUG_BAT_CH_POSITIONING){
					positionUpdate();
				}else if(chain == YUG_BAT_CH_ATTACK1){
					attack1Update();
				}else if(chain == YUG_BAT_CH_BURNUP){
					burnupUpdate();
				}else if(chain == YUG_BAT_CH_REBOUND){
					reboundUpdate();
				}else if(chain == YUG_BAT_CH_CLEANUP){
					cleanupUpdate();
				}else if(chain == YUG_BAT_CH_END){
					endUpdate();
				}else{
					animationUpdate();
				}
			}else{
				continueWaiting();
			}
		}
	}
	void BattleUnit::render(){
		if(doRender){
			if(callFadeUnit)
				fadeUnit.render(YUG_FADE_LOCAL);
			/*textPrinter.ignoreCamera = true;
			textPrinter.leftAlign = true;
			textPrinter.printText("-",YUG_TEXT_INFO_FONT,glm::vec3(0.1f,0.1f,1.0f),pos.batBigCardPos,glm::mat4());
			textPrinter.ignoreCamera = false;*/
			atkCard.render();
			if(!directAttack){
				defCard.render();
			}
			slash.render();
			glow.render();
			if(textFading){
				renderfadeText();
			}
			flash.render();
		}//text draw, slash draw...
	}
	void BattleUnit::positionUpdate(){
	
	}
	void BattleUnit::attack1Update(){

	}
	void BattleUnit::burnupUpdate(){

	}
	void BattleUnit::reboundUpdate(){

	}
	void BattleUnit::cleanupUpdate(){
		
	}
	void BattleUnit::endUpdate(){
		atkCard.cleanup();
		atkCard.doRender = false;
		if(!directAttack)
			defCard.cleanup();
		defCard.doRender = false;
		doRender = false;
		doUpdate = false;
		blankUnit.transitionInput(&(theBoard),0.01f);
		theBoard.chain = YUG_BOARD_CH_PLAYERTURN;
		theBoard.cameraToBoardCursor();
		chain = YUG_BAT_CH_IDLE;
		if(atkCardDead){
			theBoard.board[theBoard.aP[0]][theBoard.aP[1]].cleanup();
			theBoard.board[theBoard.aP[0]][theBoard.aP[1]] = cardCreator.blankCard();
		}
		if(defCardDead && !directAttack){
			theBoard.currentCard.cleanup();
			theBoard.currentCard = cardCreator.blankCard();
		}
		atkCardDead = false;
		defCardDead = false;
		puzzleCursor.doRender = true;
		theBoard.cP[0] = theBoard.aP[0];
		theBoard.cP[1] = theBoard.aP[1];
	}

	int BattleUnit::attackCalculation(){
		return 1234;
	}
	void BattleUnit::successfulAttack(){
		
	}
	void BattleUnit::drawAttack(){
	
	}
	void BattleUnit::unsuccessfulAttack(){
		
	}

	void BattleUnit::lowerDefendersLifepoints(){
		if(theBoard.playerControlling())
			theBoard.enemy.removeLP(damage);
		else
			theBoard.player.removeLP(damage);
	}
	void BattleUnit::lowerAttackersLifepoints(){
		if(!theBoard.playerControlling())
			theBoard.enemy.removeLP(damage);
		else
			theBoard.player.removeLP(damage);
	}


	void BattleUnit::wait(float duration){
		//if(isWaiting)
		//	std::cout<<"Battle Unit: one wait time overriding another\n";
		waitDuration = duration;
		currentlyWaited = 0;
		isWaiting = true;

	}
	void BattleUnit::continueWaiting(){
		currentlyWaited += gameClock.lastLoopTime();
		if(currentlyWaited > waitDuration){
			isWaiting = false;
		}
	}

	//direct attack set by containing function
	void BattleUnit::fillInParts(){
		shakeCardVar = ZYUG_SHAKE_NONE;
		willRebound = false;
		onRebound = false;
		draw = false;
		if(!directAttack){
			damage = atkCard.parentCard->altAttack - getDefenderValue();
			if(damage > 0){
				defCardDead = true;
			}else if(damage == 0){
				draw = true;
				if(!defenderDefending){
					defCardDead = true;
					atkCardDead = true;
					willRebound = true;
				}
			}else{
				damage = -damage;
				willRebound = true;
				if(!defenderDefending){
					atkCardDead = true;
				}
			}
			if(defenderDefending && !willRebound){
				damage = 0;
			}
		}else{
			damage = theBoard.attacker.attack;//atkCard.parentCard->attack;
			//std::cout<<"Bat: Direct damage dealt"<<damage<<std::endl;
		}
	}
	void BattleUnit::destroyCards(){
		if(atkCardDead){
			theBoard.board[theBoard.cP[0]][theBoard.cP[1]].cleanup();
			theBoard.board[theBoard.cP[0]][theBoard.cP[1]] = cardCreator.blankCard();
		}
		if(defCardDead){
			theBoard.board[theBoard.aP[0]][theBoard.aP[1]].cleanup();
			theBoard.board[theBoard.aP[0]][theBoard.aP[1]] = cardCreator.blankCard();

		}
	}
	void BattleUnit::inflictDamage(){
		if(willRebound){
			lowerAttackersLifepoints();
		}else{
			lowerDefendersLifepoints();
		}
	}

	int BattleUnit::getDefenderValue(){
		if(defenderDefending){
			return defCard.parentCard->altDefense;
		}else{
			return defCard.parentCard->altAttack;
		}
	}


	void BattleUnit::initializeGlow(){
		glow.startup(YUG_PLANE_FILE_PATH, "GameData/textures/particles/glowingorb.png");
		glow.doRender = false;
		glow.ignoreCamera = true;
	}
	void BattleUnit::initializeSlash(){
		slash.startup(YUG_PLANE_FILE_PATH, "GameData/textures/models/swordslash04.png");
		slash.doRender = false;
		slash.scale = pos.batSlashScale;
		glow.ignoreCamera = true;

	}
	void BattleUnit::initializeFlash(){
		flash.startup(YUG_PLANE_FILE_PATH, 	"GameData/textures/models/whitescreenUV.png");
		flash.doRender = false;
		flash.scale = glm::vec3(3.0f,3.0f,1.0f);
		flash.position = glm::vec3(0.0f,0.0f,pos.nc_z[2]);
		flash.amtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		flash.ignoreCamera = true;
	}

	void BattleUnit::shutdownModels(){
		glow.cleanup();
		slash.cleanup();
		flash.cleanup();
	}





}
