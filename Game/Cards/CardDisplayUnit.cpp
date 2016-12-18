#include <Game\Cards\CardDisplayUnit.h>
#include <Utility\InputUnit.h>
#include <Utility\TextPrinter.h>
#include <Utility\BlankUnit.h>
#include <Game\Cards\CardData.h>
#include <Utility\Clock.h>
#include <Utility\StateUnit.h>
#include <iostream>
#include <gtc\matrix_transform.hpp>
#include <Game\Animation\FadeUnit.h>
#include <Utility\SoundUnit.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\MovementUnit.h>

namespace Card{
	CardDisplayUnit CardDisplayUnit::cardDisplayUnitInstance;

	bool CardDisplayUnit::initialize(){
		displayBoard.startup("GameData/models/plane02.obj",
			"GameData/textures/models/carddisplayUV.png");
		doRender = false;
		displayBoard.ignoreCamera = true;
		displayBoard.scale = pos.cduBoardScale;
		chain = YUG_CDU_CH_IDLE;
		waitDuration = 0.0f;
		currentlyWaited = 0.0f;
		isWaiting = false;

		return true;
	}
	bool CardDisplayUnit::shutdown(){
		displayBoard.cleanup();
		return false;
	}


	void CardDisplayUnit::displayCard(Card::BigCardRender givenCard){
		fadeUnit.changeZ(pos.nc_z[3]-0.001f);
		fadeUnit.sheet.amtran = glm::vec4(0.001f,0.0f,0.0f,0.0f);
		fadeUnit.fadeTo(YUG_FADE_DARK, pos.wait[6]);
		//std::cout<<"Card display: Got card\n";
		soundUnit.cardSwoosh();
		bigCard = givenCard;
		bigCard.startup();
		bigCard.doRender = true;
		bigCard.ignoreCamera = true;
		bigCard.scale = pos.cduCardScale;
		doRender = true;
		bigCard.position = pos.cduCardOffPos;
		bigCard.rotationMatrix = pos.atkFacedownUp;
		bigCard.interpolate(pos.cduCardPos,pos.wait[4]);
		displayBoard.position = pos.cduBoardOffPos;
		displayBoard.interpolate(pos.cduBoardPos, pos.wait[4]);
		blankUnit.transitionInput(this, 0.2f);
		chain = YUG_CDU_CH_FLIP_CARD;
		wait(pos.wait[4]);
		blurbLines = stateUnit.breakUpText(bigCard.parentCard->blurb, 24);
		//for(int i = 0; i < blurbLines.size(); i++){
		//	std::cout<<blurbLines[i].c_str()<<std::endl;
		//}
	}

	void CardDisplayUnit::endDisplay(){
		fadeUnit.fadeTo(YUG_FADE_CLEAR, pos.wait[1]);
		soundUnit.cardSwivel();
		bigCard.interpolate( pos.cduCardOffPos, pos.wait[1]);
		displayBoard.interpolate(pos.cduBoardOffPos, pos.wait[1]);
		chain = YUG_CDU_CH_RETURN_CONTROL;
		wait(pos.wait[2]);
	}


	void CardDisplayUnit::update(){
		if(!doRender)
			return;
		bigCard.update();
		displayBoard.update();
		if(!isWaiting){
			if(chain == YUG_CDU_CH_FLIP_CARD){
				bigCard.rotate(pos.atkFaceupUp, pos.wait[1]);
				chain = YUG_CDU_CH_HAS_INPUT;
				wait(pos.wait[1]);
			}
			if(chain == YUG_CDU_CH_RETURN_CONTROL){
				bigCard.doRender = false;
				bigCard.cleanup();
				doRender = false;
				blankUnit.returnInput();
				chain = YUG_CDU_CH_IDLE;
			}
		}else{
			continueWaiting();
		}
	}

	void CardDisplayUnit::input(){
		if(chain == YUG_CDU_CH_HAS_INPUT && !isWaiting){
			if(inputUnit.isKeyActive(YUG_KEY_O)){
				endDisplay();
			}
		}
	}

	void CardDisplayUnit::render(){
		if(doRender){
			displayBoard.render();
			bigCard.render();
			renderInfo();
		}
		
	}

	void CardDisplayUnit::renderInfo(){
		Card::CardData* card = bigCard.parentCard;
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;
		//display type, then write out type
		glm::vec3 typePos = mov.addOffset(displayBoard.position, pos.cduTypeOffset);
		textPrinter.printSymbol(card->actualType,
			pos.cduTypeSymbolScale,
			typePos, glm::mat4());
		textPrinter.printText(stateUnit.getTypeName(card->actualType),
			YUG_TEXT_INFO_FONT,
			pos.cduTypeTextScale,
			mov.addXOffset(typePos, pos.cduTypeTextXOffset),
			glm::mat4());
		//guardian Stars
		if(card->monMagTrap == YUG_MONSTER_CARD){
			textPrinter.printText("GUARDIAN STARS", YUG_TEXT_INFO_FONT,
				pos.cduGuardainScale, 
				mov.addOffset(displayBoard.position,pos.cduGuardianOffset), 
				glm::mat4());
			//con1
			typePos = mov.addOffset(displayBoard.position, pos.cduFirstConOffset);
			textPrinter.printSymbol(card->constellations[0],
				pos.cduConSymbolScale,
				typePos, glm::mat4());
			textPrinter.printText(stateUnit.getConstellationName(card->constellations[0]),
				YUG_TEXT_INFO_FONT, pos.cduConTextScale, 
				mov.addXOffset(typePos, pos.cduConTextXOffset),
				glm::mat4());
			//con2
			typePos = mov.addOffset(displayBoard.position, pos.cduSecondConOffset);
			textPrinter.printSymbol(card->constellations[1],
				pos.cduConSymbolScale,
				typePos, glm::mat4());
			textPrinter.printText(stateUnit.getConstellationName(card->constellations[1]),
				YUG_TEXT_INFO_FONT, pos.cduConTextScale, 
				mov.addXOffset(typePos, pos.cduConTextXOffset),
				glm::mat4());
		}
		//blurb
		typePos = mov.addOffset(displayBoard.position, pos.cduFirstTextOffset);
		for(unsigned int i = 0; i < blurbLines.size(); i++){
			textPrinter.printText(blurbLines[i].c_str(),YUG_TEXT_INFO_FONT,
				pos.cduTextScale,
				mov.addYOffset(typePos, pos.cduTextYOffset*i),
				glm::mat4());


		}
		//textPrinter.ignoreCamera = true;
		
		textPrinter.ignoreCamera = false;
	}

	void CardDisplayUnit::wait(float duration){
		//if(isWaiting)
			//std::cout<<"Card display: one wait time overriding another\n";
		waitDuration = duration;
		currentlyWaited = 0;
		isWaiting = true;

	}
	void CardDisplayUnit::continueWaiting(){
		currentlyWaited += gameClock.lastLoopTime();
		if(currentlyWaited > waitDuration){
			isWaiting = false;
		}
	}

	






}
