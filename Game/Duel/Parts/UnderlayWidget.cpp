#include <Game\Duel\Parts\UnderlayWidget.h>
#include <DefinesAndTypedefs.h>
#include <Game\Duel\Board.h>
#include <Game\Duel\Parts\SwordUnit.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Utility\TextureLoader.h>
#include <Utility\TextPrinter.h>
#include <iostream>

namespace Duel{


	void UnderlayWidget::startup(){
		underlay1.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/board/underlayUV.png");
		underlay2.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/board/underlay2UV.png");
		guardianWarning.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/board/underlayBadGuardUV.png");
		textureLoader.deleteTexture(&guardianWarning.textureBO);
		textureLoader.loadTexture("GameData/textures/board/underlayBadGuardUV.png",&badGuardTBO);
		textureLoader.loadTexture("GameData/textures/board/underlayGoodGuardUV.png",&goodGuardTBO);
		renderGuard = YUG_BOARD_UL_NO_GUARD;
		underlay1.scale = pos.bUnder1Scale;
		underlay1.position = pos.bUnder1HiddenPos;
		underlay1.ignoreCamera = true;
		underlay1.doRender = true;

		underlay2.scale = pos.bUnder1Scale;
		underlay2.position = pos.bUnder2HiddenPos;
		underlay2.ignoreCamera = true;
		underlay2.doRender = true;

		guardianWarning.doRender = true;
		guardianWarning.ignoreCamera = true;
		guardianWarning.scale = pos.bUnderguardWarnScale;

		displayType = YUG_BOARD_UL_HIDE;
		nextType = YUG_BOARD_UL_NONE;
		Game::WaitUnit::startup();
	}
	void UnderlayWidget::cleanup(){
		underlay1.cleanup();
		underlay2.cleanup();
		guardianWarning.textureBO = 0;
		guardianWarning.cleanup();
		textureLoader.deleteTexture(&badGuardTBO);
		textureLoader.deleteTexture(&goodGuardTBO);
	}
	void UnderlayWidget::update(){
		underlay1.update();
		underlay2.update();
		guardianWarning.update();
		if(!isWaiting){
			if(nextType != YUG_BOARD_UL_NONE){
				switch(nextType){
				case YUG_BOARD_UL_HAND:
					viewingHand();
					break;
				case YUG_BOARD_UL_HAND_BOARD:
					viewingHandBoard();
					break;
				case YUG_BOARD_UL_BOARD:
					viewingBoard();
					break;
				case YUG_BOARD_UL_BOARD_ATTACK:
					viewingBoardAttack();
					break;
				default:
					break;
				};
			}
		}else{
			continueWaiting();
		}
	}
	void UnderlayWidget::render(){
		underlay2.render();
		underlay1.render();
		switch(displayType){
		case YUG_BOARD_UL_HAND:
			renderHand();
			break;
		case YUG_BOARD_UL_HAND_BOARD:
			renderHandBoard();
			break;
		case YUG_BOARD_UL_BOARD:
			renderBoard();
			break;
		case YUG_BOARD_UL_BOARD_ATTACK:
			renderAttack();
			break;
		default:
			break;
		};
	}

	void UnderlayWidget::show(){

	}
	void UnderlayWidget::hide(){
		//std::cout<<"UL: hide() called\n";
		underlay1.interpolate(pos.bUnder1HiddenPos, 0.1f/*pos.wait[1]*/);
		underlay2.interpolate(pos.bUnder2HiddenPos, 0.1f/*pos.wait[1]*/);
		guardianWarning.interpolate(mov.addOffset(pos.bUnder2HiddenPos,pos.bUnderguardWarnOffset), 0.1f);
		renderGuard = YUG_BOARD_UL_NO_GUARD;
		nextType = YUG_BOARD_UL_NONE;
	}
	void UnderlayWidget::hideThen(int nextMode){
		hide();
		//std::cout<<"UL: hideThen() called\n";
		nextType = nextMode;
		wait(pos.wait[3]);

	}

	void UnderlayWidget::renderHand(){
		if(theBoard.currentPlayer->currentCardGood() ){
			theBoard.currentPlayer->getCurrentCard()->duelPrintData(
				underlay1.position + pos.bUnder1TextOffset);
		}
	}
	void UnderlayWidget::renderHandBoard(){
		if(theBoard.currentPlayer->currentCardGood() ){
			theBoard.currentPlayer->getCurrentCard()->duelPrintData(
				underlay1.position + pos.bUnder1TextOffset);
		}
		if(theBoard.currentCardGood() ){
			if( theBoard.cardVisibleToPlayer()){
			theBoard.getCurrentCard()->duelPrintData(
				underlay2.position + pos.bUnder2TextOffset);
			}else{
				theBoard.getCurrentCard()->duelPrintConstellation(
				underlay2.position + pos.bUnder2TextOffset);
			}
		}
			
	}
	void UnderlayWidget::renderBoard(){
		if(theBoard.currentCardGood()){
			if( theBoard.cardVisibleToPlayer()){
			theBoard.getCurrentCard()->duelPrintData(
				underlay1.position + pos.bUnder1TextOffset);
			}else{
				theBoard.getCurrentCard()->duelPrintConstellation(
				underlay1.position + pos.bUnder1TextOffset);
			}
		}
		if(!swordUnit.currentPlayerCanAttack()){
			textPrinter.ignoreCamera = true; textPrinter.leftAlign = true;
			glm::vec3 posi = underlay1.position;
			posi.y += 0.07f;
			posi.x += 0.04f; posi.z += 0.001f;
			glm::vec3 sc = glm::vec3(0.35f,0.8f,1.0f);
			switch(swordUnit.currentPlayerCountLeft()){
			case 3:
				textPrinter.printText("Swords turns left- 3",YUG_TEXT_INFO_FONT,
					sc,posi,glm::mat4());
				break;
			case 2:
				textPrinter.printText("Swords turns left- 2",YUG_TEXT_INFO_FONT,
					sc,posi,glm::mat4());
				break;
			default:
				textPrinter.printText("Swords turns left- 1",YUG_TEXT_INFO_FONT,
					sc,posi,glm::mat4());
				break;
			}
			textPrinter.ignoreCamera = false; textPrinter.leftAlign = false;
		}
	}
	void UnderlayWidget::renderAttack(){
		if(theBoard.currentAtkCardGood() ){
			theBoard.getCurrentAtkCard()->duelPrintData(
				underlay1.position + pos.bUnder1TextOffset);
		}
		if(theBoard.currentCardGood()){
			if( theBoard.cardVisibleToPlayer()){
			theBoard.getCurrentCard()->duelPrintData(
				underlay2.position + pos.bUnder2TextOffset);
			}else{
				theBoard.getCurrentCard()->duelPrintConstellation(
				underlay2.position + pos.bUnder2TextOffset);
			}
		}
		if(renderGuard != YUG_BOARD_UL_NO_GUARD){
			guardianWarning.render();

		}
	}

	void UnderlayWidget::viewingHand(){
		underlay1.interpolate(pos.bUnder1ShownPos, pos.wait[1]);
		underlay2.interpolate(pos.bUnder2HiddenPos, pos.wait[1]);
		guardianWarning.interpolate(mov.addOffset(pos.bUnder2HiddenPos,pos.bUnderguardWarnOffset), pos.wait[1]);
		noGuard();
		displayType = YUG_BOARD_UL_HAND;
		nextType = YUG_BOARD_UL_NONE;
	}
	void UnderlayWidget::viewingHandBoard(){
		underlay1.interpolate(pos.bUnder1ShownPos, pos.wait[1]);
		underlay2.interpolate(pos.bUnder2ShownPos, pos.wait[1]);
		guardianWarning.interpolate(mov.addOffset(pos.bUnder2HiddenPos,pos.bUnderguardWarnOffset), pos.wait[1]);
		noGuard();
		displayType = YUG_BOARD_UL_HAND_BOARD;
		nextType = YUG_BOARD_UL_NONE;
	}
	void UnderlayWidget::viewingBoard(){
		underlay1.interpolate(pos.bUnder1ShownPos, pos.wait[1]);
		underlay2.interpolate(pos.bUnder2HiddenPos, pos.wait[1]);
		guardianWarning.interpolate(mov.addOffset(pos.bUnder2HiddenPos,pos.bUnderguardWarnOffset), pos.wait[1]);
		noGuard();
		displayType = YUG_BOARD_UL_BOARD;
		nextType = YUG_BOARD_UL_NONE;
	}
	void UnderlayWidget::viewingBoardAttack(){
		underlay1.interpolate(pos.bUnder1ShownPos, pos.wait[1]);
		underlay2.interpolate(pos.bUnder2ShownPos, pos.wait[1]);
		guardianWarning.interpolate(mov.addOffset(pos.bUnder2ShownPos,pos.bUnderguardWarnOffset), pos.wait[1]);
		displayType = YUG_BOARD_UL_BOARD_ATTACK;
		nextType = YUG_BOARD_UL_NONE;
	}

	void UnderlayWidget::noGuard(){
		renderGuard = YUG_BOARD_UL_NO_GUARD;
	}
	void UnderlayWidget::badGuard(){
		renderGuard = YUG_BOARD_UL_BAD_GUARD;
		guardianWarning.textureBO = badGuardTBO;
	}
	void UnderlayWidget::goodGuard(){
		renderGuard = YUG_BOARD_UL_GOOD_GUARD;
		guardianWarning.textureBO = goodGuardTBO;
	}

}