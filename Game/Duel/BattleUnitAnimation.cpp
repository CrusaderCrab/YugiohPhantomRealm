#include <Game\Duel\BattleUnit.h>
#include <Game\Duel\Board.h>
#include <Utility\Clock.h>
#include <Utility\BlankUnit.h>
#include <Utility\StateUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Cursor.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <iostream>
#include <Game\VectorUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Utility\TextureLoader.h>
#include <Utility\Clock.h>
#include <Game\Duel\EndUnit.h>
#include <Utility\SoundUnit.h>
#include <Game\EnemyData.h>
#include <random>

#define ZYUG_SHAKE_DEF 1
#define ZYUG_SHAKE_ATK 2

namespace Duel{


	/*
	#define  8
#define  9
#define  10
#define  11
#define  12
#define YUG_BAT_CH_GUARD1 13
#define  14
#define  15
#define  16
#define  17
#define  18
#define  19
#define  20
#define  21
#define  22
#define  23
#define  24
#define  25
#define 26*/

	void BattleUnit::battleStartUpdate(){
		theBoard.underlay.hide();
		puzzleCursor.doRender = false;
		theBoard.battleCursor.turnoff();
		chain = YUG_BAT_CH_LIFTOFF;
	}

	void BattleUnit::liftoffUpdate(){
		interpolateSmallCardsToPosition();
		rotateSmallCardsToPosition();
		chain = YUG_BAT_CH_CAMBACK;
		wait(pos.wait[2]);
	}
	void BattleUnit::interpolateSmallCardsToPosition(){
		if(theBoard.playerControlling()){
			theBoard.attacker.smallRender.interpolate(pos.batPSmallLeftPos, pos.wait[3]);
			if(!directAttack)
				theBoard.currentCard.smallRender.interpolate(mov.addXOffset(pos.batPSmallLeftPos, pos.batPSmallXOffset),pos.wait[3]);
		}else{
			theBoard.attacker.smallRender.interpolate(pos.batESmallLeftPos, pos.wait[3]);
			if(!directAttack)
				theBoard.currentCard.smallRender.interpolate(mov.addXOffset(pos.batESmallLeftPos, pos.batESmallXOffset),pos.wait[3]);

		}
	}
	void BattleUnit::rotateSmallCardsToPosition(){
		float waitTime = 0.8f;//pos.wait[3];
		if(theBoard.playerControlling()){
			if(theBoard.attacker.faceUp){
				theBoard.attacker.smallRender.rotate(pos.atkFaceupUp, waitTime);
			}else{
				theBoard.attacker.smallRender.rotate(pos.atkFacedownUp, waitTime);
			}
			if(!directAttack){
				if(theBoard.currentCard.faceUp){
					theBoard.currentCard.smallRender.rotate(pos.atkFaceupUp, waitTime);
				}else{
					theBoard.currentCard.smallRender.rotate(pos.atkFacedownUp, waitTime);
				}
			}
		}else{
			if(theBoard.attacker.faceUp){
				theBoard.attacker.smallRender.rotate(pos.eAtkFaceupUp, waitTime);
			}else{
				theBoard.attacker.smallRender.rotate(pos.eAtkFacedownUp, waitTime);
			}
			if(!directAttack){
				if(theBoard.currentCard.faceUp){
					theBoard.currentCard.smallRender.rotate(pos.eAtkFaceupUp, waitTime);
				}else{
					theBoard.currentCard.smallRender.rotate(pos.eAtkFacedownUp, waitTime);
				}
			}
		}
	}

	void BattleUnit::cambackUpdate(){
		theBoard.currentPlayerView(pos.wait[2]);
		theBoard.LP.hide();
		theBoard.field.hide();
		chain = YUG_BAT_CH_REVEAL1;
		wait(1.0f/*pos.wait[3]*/);
	}

	void BattleUnit::cardReveal1Update(){
		fadeUnit.changeZ(pos.nc_z[5]-0.001f);
		fadeUnit.sheet.amtran = glm::vec4(0.001f,0.0f,0.0f,0.0f);
		fadeUnit.fadeTo(YUG_FADE_BLACK, pos.wait[5]);
		flipCardsCorrectDirection();
		chain = YUG_BAT_CH_REVEAL2;
		wait(0.5f/*pos.wait[3]*/);
	}

	void BattleUnit::flipCardsCorrectDirection(){
		float waitTime = 0.5f;//pos.wait[3];
		if(theBoard.playerControlling()){
			if(theBoard.attacker.faceUp){
				theBoard.attacker.smallRender.rotate(pos.cardPBackLeft,  waitTime);
			}else{
				theBoard.attacker.smallRender.rotate(pos.cardPBackRight,  waitTime);
			}
			if(!directAttack){
				if(theBoard.currentCard.faceUp){
					theBoard.currentCard.smallRender.rotate(pos.cardPBackLeft,  waitTime);
				}else{
					theBoard.currentCard.smallRender.rotate(pos.cardPBackRight,  waitTime);
				}
			}
		}else{
			if(theBoard.attacker.faceUp){
				theBoard.attacker.smallRender.rotate(pos.cardEBackLeft,  waitTime);
			}else{
				theBoard.attacker.smallRender.rotate(pos.cardEBackRight,  waitTime);
			}
			if(!directAttack){
				if(theBoard.currentCard.faceUp){
					theBoard.currentCard.smallRender.rotate(pos.cardEBackLeft,  waitTime);
				}else{
					theBoard.currentCard.smallRender.rotate(pos.cardEBackRight,  waitTime);
				}
			}
		}
	}

	void BattleUnit::cardReveal2Update(){
		callFadeUnit = true;
		fadeUnit.renderBlock = true;
		float waitTime = 0.4f;//pos.wait[3];
		atkCard.doRender = true;
		atkCard.rotationMatrix = pos.cardBatAtkBackLeft;
		atkCard.rotate(glm::mat4(), waitTime);
		theBoard.attacker.smallRender.doRender = false;
		if(!directAttack){
			defCard.doRender = true;
			defCard.ignoreCamera = true;
			defCard.rotationMatrix = pos.cardBatDefBackLeft;
			defCard.rotate(glm::mat4(), waitTime);
			theBoard.currentCard.smallRender.doRender = false;
		}
		if(atkGuardianBoost || defGuardianBoost){
			chain = YUG_BAT_CH_GUARD1;
			wait(waitTime + pos.wait[2]);
		}else{
			chain = YUG_BAT_CH_STARTSLASH;
			wait(waitTime + pos.wait[2]);
		}
	}

	void BattleUnit::guard1Update(){
		float aniTime = 0.8f;
		particleUnit.particleAmtran.z = 1.5f;
		soundUnit.playOnce("GameData/sounds/battle/guardCharge01.wav");
		textureLoader.deleteTexture(&glow.textureBO);
		textureLoader.loadTexture("GameData/textures/particles/glowingorb.png",&glow.textureBO);
		particleUnit.ignoreCamera = true;
		if(atkGuardianBoost){
			particleUnit.particleSuck(pos.batBigCardPos, 0.8f);
			particleUnit.getTextureFromConstellation(
				theBoard.attacker.currentConstellation);
			atkCard.emphasis(YUG_BIG_CARD_EMPHASIS_ATK);
			atkCard.countStatUp(500, aniTime);
			glow.position = mov.addZOffset(atkCard.position,0.01f);
		}else{
			if(defenderDefending){
				defCard.emphasis(YUG_BIG_CARD_EMPHASIS_DEF);
			}else{
				defCard.emphasis(YUG_BIG_CARD_EMPHASIS_ATK);
			}
			particleUnit.particleSuck(defCard.position, 0.8f);
			particleUnit.getTextureFromConstellation(
				theBoard.currentCard.currentConstellation);
			defCard.countStatUp(500, aniTime);
			glow.position = mov.addZOffset(defCard.position,0.01f);
		}
		glow.doRender = true;
		glow.amtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		glow.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f), aniTime);
		glow.scale = glm::vec3(0.00001f,0.00001f,0.00001f);
		glow.scaleInterpolate(pos.batConBoostOrbScale,aniTime);
		chain = YUG_BAT_CH_GUARD2;
		wait(aniTime);
	}

	void BattleUnit::guard2Update(){
		float aniTime = 0.1f;
		glow.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f), aniTime);
		glow.scaleInterpolate(glm::vec3(0.00001f,0.00001f,0.00001f),aniTime);
		chain = YUG_BAT_CH_STARTSLASH;
		wait(aniTime+0.2f);
	}

	void BattleUnit::startSlashUpdate(){
		float aniTime = 0.05f;
		particleUnit.particleAmtran.z = 1.0f;
		glow.doRender = false;
		slash.doRender = true;
		slash.ignoreCamera = true;
		slash.amtran =  glm::vec4(1.0f,1.0f,1.0f,0.0f);
		slash.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f), 0.3f);
		if(onRebound){
			slash.position = mov.addZOffset(atkCard.position, 0.01f);
		}else{
			glm::vec3 posi = mov.addXOffset(pos.batBigCardPos,pos.batBigCardXOffset);
			slash.position = mov.addZOffset(posi, 0.01f);
		}
		soundUnit.playOnce("GameData/sounds/battle/slash04.wav");
		chain = YUG_BAT_CH_WHITEFLASH;
		wait(aniTime);
	}

	void BattleUnit::whiteFlashUpdate(){
		float aniTime = 0.005f;
		flash.doRender = true;
		flash.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f),aniTime);
		wait(aniTime);
		chain = YUG_BAT_CH_FADEOUT;
	}

	void BattleUnit::fadeOutUpdate(){
		float aniTime = 0.2f;
		flash.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),aniTime);
		//wait(aniTime);
		if(!willRebound || onRebound || defCardDead)
			chain = YUG_BAT_CH_SUCCESSANI;
		else
			chain = YUG_BAT_CH_FAILANI;
	}

	void BattleUnit::successUpdate(){
		float anitime = 0.15f;
		shakeCard();
		attackGlow();
		fadeTextIn();
		chain = YUG_BAT_CH_SUCCESSANI2;
		wait(anitime);
	}

	void BattleUnit::shakeCard(){
		srand(0);
		if(!onRebound){
			shakeCardVar = ZYUG_SHAKE_DEF;
			originalCardPosition = defCard.position;
		}
		else{
			shakeCardVar = ZYUG_SHAKE_ATK;
			originalCardPosition = atkCard.position;
		}
	}

	void BattleUnit::fadeTextIn(){
		if(damage == 0 || ( !onRebound && willRebound))
			return;
		textFading = true;
		fadeTextSpeed = 1/0.2f;
		fadeTextDuration = 0.0f;
		textStarttran = 0.5f;
		textDestinationTran = 1.0f;
		textCurrentTran = textStarttran;
	}
	void BattleUnit::fadeTextOut(){
		if(damage == 0 || (!onRebound && willRebound))
			return;
		textFading = true;
		fadeTextSpeed = 1/0.8f;
		fadeTextDuration = 0.0f;
		textStarttran = 1.0f;
		textDestinationTran = 0.0f;
	}
	void BattleUnit::attackGlow(){
		glow.doRender = true;
		glow.position = mov.addZOffset(slash.position,0.01f);
		textureLoader.deleteTexture(&glow.textureBO);
		textureLoader.loadTexture("GameData/textures/particles/orbwithhalo4.png",&glow.textureBO);
		glow.amtran =  glm::vec4(1.0f,1.0f,1.0f,0.0f);
		glow.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.33f), 0.15f);
		glow.scale = glm::vec3(0.001f,0.001f,1.000f);
		glow.scaleInterpolate(/*pos.batConBoostOrbScale*/glm::vec3(0.5f,0.5f,1.0f),0.15f);
	}

	void BattleUnit::success2Update(){
		slash.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f), 0.4f);
		glow.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f), 0.4f);
		fadeTextOut();
		wait(0.4f);
		chain = YUG_BAT_CH_WAITBEAT;
	}

	void BattleUnit::waitBeatUpdate(){
		slash.doRender = false;
		glow.doRender = false;
		flash.doRender = false;
		textFading = false;
		if(shakeCardVar != ZYUG_SHAKE_NONE){
			if(shakeCardVar == ZYUG_SHAKE_ATK){
				atkCard.position = originalCardPosition;
			}else{
				defCard.position = originalCardPosition;
			}
			shakeCardVar = ZYUG_SHAKE_NONE;
		}
		defCardBurnt = false;
		chain = YUG_BAT_CH_BURNUPSET;
		if(willRebound && !onRebound){
			onRebound = true;
			chain = YUG_BAT_CH_STARTSLASH;
		}
		wait(0.2f);
	}

	 void BattleUnit::failUpdate(){
		float anitime = 0.2f;
		chain = YUG_BAT_CH_FAILANI2;
		wait(anitime);
	 }

	 void BattleUnit::fail2Update(){
		slash.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f), 0.4f);
		wait(0.4f);
		chain = YUG_BAT_CH_WAITBEAT;
	 }

	 void BattleUnit::burnupSetUpdate(){
		 particleUnit.particleAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		 if(defCardBurnt){
			 defCard.doRender = false;
		 }
		 if(defCardDead && !defCardBurnt){
			 defCardBurnup();
			 defCardBurnt = true;
			 wait(0.5f);
			 return;
		 }
		 if(atkCardDead){
			 atkCardBurnup();
			 wait(0.5f);
			 chain = YUG_BAT_CH_FADEOUTCARD;
			 return;
		 }
		 chain = YUG_BAT_CH_FADEOUTCARD;
		 wait(0.1f);
		 return;
	 }

	 void BattleUnit::defCardBurnup(){
		 glm::vec3 posi = mov.addZOffset(defCard.position, 0.025f);
		 soundUnit.playOnce("GameData/sounds/battle/CardBurnup02.wav");
		 particleUnit.burnup(
			 YUG_PARTICLE_BURN_MON, 
			 mov.addYOffset(posi, -0.2f),
			 0.27f, 0.45f, 0.6f);
	 }

	 void BattleUnit::atkCardBurnup(){
		 glm::vec3 posi = mov.addZOffset(atkCard.position, 0.025f);
		 soundUnit.playOnce("GameData/sounds/battle/CardBurnup02.wav");
		 particleUnit.burnup(
			 YUG_PARTICLE_BURN_MON, 
			 mov.addYOffset(posi, -0.25f),
			 0.27f, 0.45f, 0.6f);
	 }

	 void BattleUnit::fadeCardOutUpdate(){
		 float anitime = 0.2f;
		 
		 if(atkCardDead){
			 atkCard.doRender = false;
		 }
		 if(!atkCardDead){
			 atkCard.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
			 atkCard.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),anitime);
		 }
		 if(!defCardDead){
			 defCard.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
			 defCard.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),anitime);
		 }
		 prepareBoard();
		 chain = YUG_BAT_CH_RETURN;
		 wait(anitime);
	 }

	 void BattleUnit::prepareBoard(){
		 //std::cout<<theBoard.aP[0]<<theBoard.aP[1]<<theBoard.cP[0]<<theBoard.cP[1]<<"\n";
		 if(atkCardDead){
			// std::cout<<"DDDDDDDDDDDDDDDDDDDDD\n";
			 theBoard.attacker.smallRender.cleanup();
			 theBoard.attacker.cleanup();
			 theBoard.attacker = cardCreator.blankCard();
		 }else{
			 theBoard.attacker.faceUp = true;
			 theBoard.pullCardToBoard(theBoard.aP[0],theBoard.aP[1]);
			 theBoard.attacker.smallRender.rotationMatrix = theBoard.getCorrectCardRotation(theBoard.aP[0],theBoard.aP[1]);
			 theBoard.attacker.hasAttacked = true;
			 theBoard.attacker.smallRender.interpolateAmtran(glm::vec4(0.5f,0.5f,0.5f,1.0f),0.01f);
		 }
		 if(defCardDead && !directAttack){
			 theBoard.currentCard.smallRender.cleanup();
			 theBoard.currentCard.cleanup();
			 theBoard.currentCard = cardCreator.blankCard();
			 if(theBoard.playerControlling()){
				 endUnit.enemyCardsDestroyed++;
				 endUnit.noOfSuccessfulAttacks++;
			 }
		 }else if(!defCardDead && !directAttack){
			 theBoard.currentCard.faceUp = true;
			 theBoard.pullCardToBoard(theBoard.cP[0],theBoard.cP[1]);
			 theBoard.currentCard.smallRender.rotationMatrix = theBoard.getCorrectCardRotation(theBoard.cP[0],theBoard.cP[1]);
			 if(!theBoard.playerControlling()){
				 endUnit.noOfSuccessfulDefenses++;
			 }
		 }
		 if(directAttack && theBoard.playerControlling()){
			 endUnit.noOfSuccessfulAttacks++;
		 }
		 theBoard.cP[0] = theBoard.aP[0];
		 theBoard.cP[1] = theBoard.aP[1];
		 theBoard.lockRowMovement = false;
		 theBoard.battleCursor.turnon();
		 theBoard.currentPlayer->toBoardView();
		 theBoard.underlay.viewingBoard();

	 }

	 void BattleUnit::returnUpdate(){
		 atkCard.parentCard->altAttack = atkCard.parentCard->attack;
		 if(!atkCardDead){
			 atkCard.parentCard->smallRender.doRender = true;
			 atkCard.doRender = false;
		 }
		 atkCard.cleanup();
		 if(!defCardDead && !directAttack){
			 defCard.parentCard->smallRender.doRender = true;
			 defCard.doRender = false;
		 }
		 if(!directAttack){
			 defCard.parentCard->altDefense = defCard.parentCard->defense;
			 defCard.parentCard->altAttack = defCard.parentCard->attack;
			 defCard.cleanup();
		 }
		 float aniTime = 0.2f;
		 callFadeUnit = false;
		 fadeUnit.renderBlock = false;
		 fadeUnit.fadeTo(YUG_FADE_CLEAR,aniTime);
		 //fadeUnit.doRender = false;
		 chain = YUG_BAT_CH_ENDSET;
		 wait(aniTime);
	 }

	 void BattleUnit::endSetUpdate(){
		 theBoard.LP.reveal();
		 theBoard.field.reveal();
		 inflictDamage();
		 chain = YUG_BAT_CH_IDLE;
		 doRender = false;
		 if(!theBoard.playerControlling()){
			 theBoard.enemy.ai->removeAllTrapMarks();
		 }
		 int gameStatus = endUnit.checkLifepoints();
		 if(gameStatus == YUG_END_PLAYER_LOST){
			 endUnit.playerLost();
		 }else if(gameStatus == YUG_END_PLAYER_WIN){
			 endUnit.playerWin();
		 }else{
			 theBoard.chain = YUG_BOARD_CH_PLAYERTURN;
			 theBoard.lockRowMovement = false;
			 blankUnit.transitionInput(&(theBoard),0.2f);
		 }
	 }

	void BattleUnit::animationUpdate(){
		switch(chain){
		case YUG_BAT_CH_STARTSET:
			battleStartUpdate();
			break;
		case YUG_BAT_CH_LIFTOFF:
			liftoffUpdate();
			break;
		case YUG_BAT_CH_CAMBACK:
			cambackUpdate();
			break;
		case YUG_BAT_CH_REVEAL1:
			cardReveal1Update();
			break;
		case YUG_BAT_CH_REVEAL2:
			cardReveal2Update();
			break;
		case YUG_BAT_CH_GUARD1:
			guard1Update();
			break;
		case YUG_BAT_CH_GUARD2:
			guard2Update();
			break;
		case YUG_BAT_CH_GUARDEND:

			break;
		case YUG_BAT_CH_STARTSLASH:
			startSlashUpdate();
			break;
		case YUG_BAT_CH_WHITEFLASH:
			whiteFlashUpdate();
			break;
		case YUG_BAT_CH_FADEOUT:
			fadeOutUpdate();
			break;
		case YUG_BAT_CH_SUCCESSANI:
			successUpdate();
			break;
		case YUG_BAT_CH_FAILANI:
			failUpdate();
			break;
		case YUG_BAT_CH_SUCCESSANI2:
			success2Update();
			break;
		case YUG_BAT_CH_FAILANI2:
			fail2Update();
			break;
		case YUG_BAT_CH_WAITBEAT:
			waitBeatUpdate();
			break;
		case YUG_BAT_CH_REBOUNDSTART:

			break;
		case YUG_BAT_CH_BURNUPSET:
			burnupSetUpdate();
			break;
		case YUG_BAT_CH_FADEOUTCARD:
			fadeCardOutUpdate();
			break;
		case YUG_BAT_CH_RETURN:
			returnUpdate();
			break;
		case YUG_BAT_CH_ENDSET:
			endSetUpdate();
			break;
		default:

			break;
		}
	}

	void BattleUnit::shakeCardUpdate(){
		float randX = (rand()%30)*0.0001f;
		if(onRebound){
			atkCard.position = mov.addXOffset(originalCardPosition, randX);
		}else{
			defCard.position = mov.addXOffset(originalCardPosition, randX);
		}
	}
	void BattleUnit::fadeTextUpdate(){
		fadeTextDuration += gameClock.lastLoopTime()*fadeTextSpeed;
		if(fadeTextDuration < 1.0f){
			textCurrentTran = textStarttran + ((textDestinationTran-textStarttran)*fadeTextDuration);
		}else{
			textCurrentTran = textDestinationTran;
			//textFading = false;
		}
	}

	void BattleUnit::renderfadeText(){
		textPrinter.leftAlign = false;
		textPrinter.ignoreCamera = true;
		glm::vec3 posi = mov.addXOffset((glow.position),0.02f);
		posi.z += 0.001f;
		glm::vec4 oldAmtran = textPrinter.amtran;
		textPrinter.amtran = glm::vec4(1.0f,1.0f,1.0f,textCurrentTran);
		textPrinter.printDamageNumber(damage, pos.batNumberScale, posi);
		textPrinter.amtran = oldAmtran;
		textPrinter.leftAlign = true;
		textPrinter.ignoreCamera = false;
	}

	void BattleUnit::setUpBigAttacker(){
		atkCard.ignoreCamera = true;
		atkCard.scale = pos.batBigCardScale;
		atkCard.position = pos.batBigCardPos;
		atkCard.emphasis(YUG_BIG_CARD_EMPHASIS_ATK);
		atkCard.doRender = false;
		atkCard.raiseStat = false;
	}

	void BattleUnit::setUpBigDefender(){
		defCard.ignoreCamera = true;
		defCard.scale = pos.batBigCardScale;
		defCard.position = mov.addXOffset(pos.batBigCardPos, pos.batBigCardXOffset);
		if(defenderDefending){
			defCard.emphasis(YUG_BIG_CARD_EMPHASIS_DEF);
		}else{
			defCard.emphasis(YUG_BIG_CARD_EMPHASIS_ATK);
		}
		defCard.raiseStat = false;
	}

}