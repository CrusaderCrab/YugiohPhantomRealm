#include <Utility\ErrorHandler.h>
#include <DefinesAndTypedefs.h>
#include <Game\Cards\CardData.h>
#include <Utility\TextPrinter.h>
#include <Utility\TextureLoader.h>
#include <iostream>

#define YUG_NAME_TEXT_HEIGHT 1.0f
#define YUG_NAME_TEXT_WIDTH 1.0f
#define YUG_ATK_TEXT_HEIGHT 0.5f
#define YUG_ATK_TEXT_WIDTH 0.4f
#define YUG_CARD_ATK_X_OFFSET 1.3f
#define YUG_CARD_ATK_Y_OFFSET 0.03f
#define YUG_CARD_TYPE_OFFSET 1.65f
#define YUG_CARD_CON1_OFFSET 1.78f
#define YUG_CARD_CON2_OFFSET 1.9f
#define YUG_CARD_SYMBOL_SCALE 0.8f

namespace Card{

	void CardData::startup(){
		//std::cout<<"Card Data start up "<<name.data()<<std::endl;
		chain = YUG_CARD_CH_IDLE;
		faceUp = false;
		attackMode = true;
		hasAttacked = false;
		isVisible = true;
		smallRender.doRender = false;
		bigRender.doRender = false;
		pictureTBO = YUG_UNBIND;
		atkStatBoost = 0; defStatBoost = 0;
		atkStatDrop = 0; defStatDrop = 0;
		hidden = false;
	}
	void CardData::cleanup(){
		if(pictureTBO != YUG_UNBIND){
			textureLoader.deleteTexture( &(pictureTBO));
		}
	}

	void CardData::update(){
		if(smallRender.doRender)
			smallRender.update();
		if(bigRender.doRender)
			bigRender.update();
	}

	void CardData::effect(int info){
		//for magic/trap cards
	}
	void CardData::listen(int info){
		//for trap cards
	}

	void CardData::setConstellation(int  chosenConstellation){
		if(chosenConstellation!=constellations[0]
		|| chosenConstellation!=constellations[1])
			///errorHandler.printError("Card given incorrect constellation");
		currentConstellation = chosenConstellation;
	}
	void CardData::setBattleMode(int mode){
		if(mode != YUG_ATTACK_MODE && mode != YUG_DEFENSE_MODE)
			//errorHandler.printError("Card given invalid battle mode.");
		attackMode = (mode == YUG_ATTACK_MODE);
	}
	void CardData::setFacePosition(int position){
		if(position!=YUG_FACE_UP && position !=YUG_FACE_DOWN)
			//errorHandler.printError("Card given invalid face position");
		faceUp = (position == YUG_FACE_UP);

	}
	int CardData::versusConstellation(int enemyCon){
		switch(enemyCon){
		case YUG_MERCURY:
			if(currentConstellation == YUG_VENUS)
				return YUG_STRONG_AGAINST;
			if(currentConstellation == YUG_SUN){
				return YUG_WEAK_AGAINST;
			}else{
				return constellationComparison(enemyCon);
			}
		case YUG_SUN:
			return constellationComparison(enemyCon);
		case YUG_MOON:
			return constellationComparison(enemyCon);
		case YUG_VENUS:
			if(currentConstellation == YUG_MERCURY)
				return YUG_WEAK_AGAINST;
			else if(currentConstellation == YUG_MOON)
				return YUG_STRONG_AGAINST;
			else if(currentConstellation == YUG_MARS)
				return YUG_EQUAL_AGAINST;
			else{
				return constellationComparison(enemyCon);
			}
		case YUG_MARS:
			if(currentConstellation == YUG_NEPTUNE)
				return YUG_STRONG_AGAINST;
			else if(currentConstellation == YUG_JUPITER)
				return YUG_WEAK_AGAINST;
			else if(currentConstellation == YUG_VENUS)
				return YUG_EQUAL_AGAINST;
			else{
				return constellationComparison(enemyCon);
			}
		case YUG_JUPITER:
			return constellationComparison(enemyCon);
		case YUG_SATURN:
			return constellationComparison(enemyCon);
		case YUG_URANUS:
			return constellationComparison(enemyCon);
		case YUG_PLUTO:
			return constellationComparison(enemyCon);
		case YUG_NEPTUNE:
			if(currentConstellation == YUG_MARS)
				return YUG_WEAK_AGAINST;
			else{
				return constellationComparison(enemyCon);
			}
		default:
			//errorHandler.printError("versusConstellation given invalid constellation");
			return YUG_EQUAL_AGAINST;
		}
	}

	int CardData::constellationComparison(int enemyCon){
		if(currentConstellation - enemyCon == 1)
			return YUG_WEAK_AGAINST;
		if(currentConstellation - enemyCon == -1)
			return YUG_STRONG_AGAINST;
		return YUG_EQUAL_AGAINST;
	}

	bool CardData::blankCard(){
		return cardNumber == YUG_NO_CARD;
	}

	void CardData::alterAttack(int change){
		attack += change;
		if(attack < 0)
			attack = 0;
		if(attack > 9999)
			attack = 9999;
		altAttack = attack;

		fieldlessAttack += change;
		if(fieldlessAttack < 0)
			fieldlessAttack = 0;
		if(fieldlessAttack > 9999)
			fieldlessAttack = 9999;
	}
	void CardData::alterAltAttack(int change){
		altAttack += change;
		if(altAttack < 0)
			altAttack = 0;
		if(altAttack > 9999)
			altAttack = 9999;
	}
	void CardData::alterDefense(int change){
		defense += change;
		if(defense < 0)
			defense = 0;
		if(defense > 9999)
			defense = 9999;
		altDefense = defense;

		fieldlessDefense += change;
		if(fieldlessDefense < 0)
			fieldlessDefense = 0;
		if(fieldlessDefense > 9999)
			fieldlessDefense = 9999;
	}
	void CardData::alterAltDefense(int change){
		altDefense += change;
		if(altDefense < 0)
			altDefense = 0;
		if(altDefense > 9999)
			altDefense = 9999;
	}
	void CardData::alterType(int newType){
		if( YUG_DRAGON > newType || newType > YUG_PLANT){
			//errorHandler.printError("CardData: alter type: invalid type passed.");
		}else{
			actualType = newType;
		}
	}

	void CardData::addFusionType(int newType){
		fusionTypes.push_back(newType);
	}

	void CardData::duelPrintData(glm::vec3 position){
		if(hidden) return;
		int length = strlen(name.data());
		float textSqueeze = 0.666f;
		if(length > 15)
			textSqueeze = 10.0f/length;
		textSqueeze *= 0.5;
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;
		textPrinter.printText(name.data(), YUG_TEXT_INFO_FONT,
			textSqueeze, YUG_NAME_TEXT_HEIGHT,
			position.x,position.y,position.z,glm::mat4());
		if(monMagTrap == YUG_MONSTER_CARD){
			textPrinter.printTextAndNumber("/",altAttack,
				YUG_TEXT_INFO_FONT,YUG_ATK_TEXT_WIDTH, YUG_ATK_TEXT_HEIGHT,
				position.x+YUG_CARD_ATK_X_OFFSET,
				position.y+YUG_CARD_ATK_Y_OFFSET,
				position.z,
				glm::mat4());
			textPrinter.printTextAndNumber("@",altDefense,
				YUG_TEXT_INFO_FONT,YUG_ATK_TEXT_WIDTH, YUG_ATK_TEXT_HEIGHT,
				position.x+YUG_CARD_ATK_X_OFFSET,
				position.y-YUG_CARD_ATK_Y_OFFSET,
				position.z,
				glm::mat4());
			textPrinter.printSymbol(actualType,
				YUG_CARD_SYMBOL_SCALE,YUG_CARD_SYMBOL_SCALE,
				position.x+YUG_CARD_TYPE_OFFSET,
				position.y,
				position.z,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.00001f);
			if(currentConstellation == YUG_CARD_NO_CURRENT_CON){
				textPrinter.printSymbol(constellations[0],
				YUG_CARD_SYMBOL_SCALE,YUG_CARD_SYMBOL_SCALE,
				position.x+YUG_CARD_CON1_OFFSET,
				position.y,
				position.z,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.00001f);
				textPrinter.printSymbol(constellations[1],
				YUG_CARD_SYMBOL_SCALE,YUG_CARD_SYMBOL_SCALE,
				position.x+YUG_CARD_CON2_OFFSET,
				position.y,
				position.z,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.00001f);
			}else{
				textPrinter.printSymbol(currentConstellation,
				YUG_CARD_SYMBOL_SCALE,YUG_CARD_SYMBOL_SCALE,
				position.x+YUG_CARD_CON1_OFFSET,
				position.y,
				position.z,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.00001f);

			}
		}else if(monMagTrap == YUG_MAGIC_CARD){
			textPrinter.printText("Magic", YUG_TEXT_INFO_FONT,
				0.333f,YUG_NAME_TEXT_HEIGHT,
				position.x+1.6f,
				position.y,
				position.z,
				glm::mat4());
			textPrinter.printSymbol(YUG_MAGIC_SYMBOL,
				YUG_CARD_SYMBOL_SCALE,YUG_CARD_SYMBOL_SCALE,
				position.x+YUG_CARD_CON2_OFFSET,
				position.y,
				position.z,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.00001f);
		}else if(monMagTrap == YUG_EQUIP_CARD){
			textPrinter.printText("Boost", YUG_TEXT_INFO_FONT,
				0.333f,YUG_NAME_TEXT_HEIGHT,
				position.x+1.6f,
				position.y,
				position.z,
				glm::mat4());
			textPrinter.printSymbol(YUG_EQUIP_SYMBOL,
				YUG_CARD_SYMBOL_SCALE,YUG_CARD_SYMBOL_SCALE,
				position.x+YUG_CARD_CON2_OFFSET,
				position.y,
				position.z,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.00001f);
		}else{
			textPrinter.printText("Trap", YUG_TEXT_INFO_FONT,
				0.333f,YUG_NAME_TEXT_HEIGHT,
				position.x+1.65f,
				position.y,
				position.z,
				glm::mat4());
			textPrinter.printSymbol(YUG_TRAP_SYMBOL,
				YUG_CARD_SYMBOL_SCALE,YUG_CARD_SYMBOL_SCALE,
				position.x+YUG_CARD_CON2_OFFSET,
				position.y,
				position.z,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.00001f);
		}
		textPrinter.ignoreCamera = false;
	}
	void CardData::duelPrintConstellation(glm::vec3 position){
		if(monMagTrap == YUG_MONSTER_CARD){
			textPrinter.ignoreCamera = true;
			textPrinter.printSymbol(currentConstellation,
				YUG_CARD_SYMBOL_SCALE,YUG_CARD_SYMBOL_SCALE,
				position.x+YUG_CARD_CON1_OFFSET,
				position.y,
				position.z,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.00001f);
			textPrinter.ignoreCamera = false;
		}
	}

	void CardData::trunkPrintData(glm::vec3 posi){
		duelPrintData(posi);
	}

	bool CardData::hasThisAttribute(int attrib){
		if(attrib == 0)
			return false;
		for(int i = 0; i < fusionTypes.size(); i++){
			if(fusionTypes[i] == attrib)
				return true;
		}
		return false;
	}

	bool CardData::hasThisAttribute(int attrib, int index){
		return (fusionTypes[index] == attrib && (!(attrib == 0)) ); 

	}

	bool CardData::canUseEquip(int num){
		return (cardNumber==num || hasThisAttribute(num) || actualType==num);
	}

	void CardData::newTurn(){
		hasAttacked = false;
	}

	void CardData::attacked(){
		hasAttacked = true;
	}

	void CardData::input(){
		//errorHandler.printError("Normal CardData .input() called, should never happen!");
	}

}
