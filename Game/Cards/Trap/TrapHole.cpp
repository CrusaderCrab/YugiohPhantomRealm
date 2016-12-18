#include <Game\Cards\Trap\TrapHole.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Game\Duel\Board.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Utility\SoundUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <iostream>

#define ZYUG_IDLE 0
#define ZYUG_START 1
#define ZYUG_FADEIN 2
#define ZYUG_BURNUP 3
#define ZYUG_FADECARD 4
#define ZYUG_FADEOUT 5
#define ZYUG_RETURN 6
#define ZYUG_END 7

#define ZYUG_X_OFF 1.5f

namespace Card{

		void TrapHole::startup(){
			Game::WaitUnit::startup();
			renderingFade = false;
			chain = ZYUG_START;
			attackCard = &(theBoard.board[theBoard.aP[0]][theBoard.aP[1]]);
			attackCard->bigRender.startup();
			defender = &(theBoard.board[trapUnit.cardCol][trapUnit.cardRow]);
			defender->bigRender.startup();
			setupBigCards();
			hasCards = true;
		}

		void TrapHole::setupBigCards(){
			attackCard->bigRender.ignoreCamera = true;
			attackCard->bigRender.scale = pos.batBigCardScale;
			attackCard->bigRender.position = mov.addXOffset(pos.batBigCardPos, -ZYUG_X_OFF);
			attackCard->bigRender.doRender = true;
			attackCard->bigRender.raiseStat = false;
			attackCard->bigRender.emphasis(YUG_BIG_CARD_EMPHASIS_BOTH);
			attackCard->bigRender.rotationMatrix = glm::mat4();

			defender->bigRender.ignoreCamera = true;
			defender->bigRender.scale = pos.batBigCardScale;
			defender->bigRender.position = mov.addXOffset(pos.batBigCardPos, pos.batBigCardXOffset + ZYUG_X_OFF);
			defender->bigRender.doRender = true;
			defender->bigRender.raiseStat = false;
			defender->bigRender.emphasis(YUG_BIG_CARD_EMPHASIS_BOTH);
			defender->bigRender.rotationMatrix = glm::mat4();
		}

		void TrapHole::cleanup(){
			
		}
		void TrapHole::update(){
			if(hasCards){
				attackCard->bigRender.update();
				defender->bigRender.update();
			}
			if(!isWaiting){
				switch(chain){
				case ZYUG_START:
					startUpdate();
					break;
				case ZYUG_FADEIN:
					fadeInUpdate();
					break;
				case ZYUG_BURNUP:
					burnUpdate();
					break;
				case ZYUG_FADECARD:
					fadeCardUpdate();
					break;
				case ZYUG_FADEOUT:
					fadeOutUpdate();
					break;
				case ZYUG_RETURN:
					returnUpdate();
					break;
				case ZYUG_END:
					endUpdate();
					break;
				default:
					break;
				}
			}else{
				continueWaiting();
			}
		}
		void TrapHole::render(){
			if(renderingFade){
				fadeUnit.render(YUG_FADE_LOCAL);
			}
			if(hasCards){
				attackCard->bigRender.render();
				defender->bigRender.render();
			}
		}


		void TrapHole::startUpdate(){
			theBoard.trapBeforeParticles = true;
			theBoard.currentPlayerView(pos.wait[2]);
			theBoard.LP.hide();
			theBoard.field.hide();
			fadeUnit.renderBlock = true;
			renderingFade = true;
			chain = ZYUG_FADEIN;
			wait(0.3f);
		}
		void TrapHole::fadeInUpdate(){
			fadeUnit.changeZ(pos.nc_z[5]-0.001f);
			fadeUnit.sheet.amtran = glm::vec4(0.001f,0.0f,0.0f,0.0f);
			fadeUnit.fadeTo(YUG_FADE_BLACK, pos.wait[5]);
			attackCard->bigRender.interpolate(pos.batBigCardPos, 0.5f);
			defender->bigRender.interpolate(mov.addXOffset(pos.batBigCardPos,pos.batBigCardXOffset),0.5f);
			chain = ZYUG_BURNUP;
			wait(0.7f);
		}
		void TrapHole::burnUpdate(){
			particleUnit.swapFlames();
			particleUnit.particleAmtran = glm::vec4(0.5f,0.1f,0.9f,1.0f);
			glm::vec3 posi = mov.addZOffset(attackCard->bigRender.position, 0.025f);
			soundUnit.playOnce("GameData/sounds/battle/CardBurnup02.wav");
			particleUnit.burnup(
				YUG_PARTICLE_BURN_MON, 
				mov.addYOffset(posi, -0.2f),
				0.27f, 0.45f, 0.6f);
			chain = ZYUG_FADECARD;
			wait(0.57f);
		}
		void TrapHole::fadeCardUpdate(){
			attackCard->bigRender.doRender = false;
			defender->bigRender.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
			defender->bigRender.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),0.3f);
			chain = ZYUG_FADEOUT;
			wait(0.3f);
			theBoard.cP[0] = theBoard.aP[0];
			theBoard.cP[1] = theBoard.aP[1];
			theBoard.lockRowMovement = false;
			theBoard.battleCursor.turnon();
			theBoard.currentPlayer->toBoardView();
			theBoard.underlay.viewingBoard();
		}
		void TrapHole::fadeOutUpdate(){
			particleUnit.swapFlames();
			attackCard->smallRender.cleanup();
			attackCard->bigRender.cleanup();
			attackCard->cleanup();
			theBoard.board[theBoard.aP[0]][theBoard.aP[1]] = cardCreator.blankCard();

			defender->bigRender.cleanup();
			hasCards = false;
			renderingFade = false;
			fadeUnit.renderBlock = false;
			fadeUnit.fadeTo(YUG_FADE_CLEAR,0.2f);
			chain = ZYUG_RETURN;
			wait(0.2f);
		}
		void TrapHole::returnUpdate(){
			theBoard.LP.reveal();
			theBoard.field.reveal();
			theBoard.lockRowMovement = false;
			chain = ZYUG_IDLE;
			trapUnit.magicEnd = false;
			theBoard.trapBeforeParticles = false;
			trapUnit.chain = YUG_TRAP_CH_SPECIFIC_END;
		}
		void TrapHole::endUpdate(){

		}


		bool TrapHole::listen(int info){
			if(info == YUG_TRAP_ATTACK){
				if(theBoard.board[theBoard.aP[0]][theBoard.aP[1]].attack
					<= 1500){
					return true;
				}
			}
			return false;
		}
}
