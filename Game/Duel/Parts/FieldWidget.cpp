#include <Game\Duel\Parts\FieldWidget.h>
#include <Utility\Clock.h>
#include <Utility\TextPrinter.h>
#include <Utility\TextureLoader.h>
#include <Game\VectorUnit.h>
#include <Game\Duel\Board.h>
#include <Game\Animation\MovementUnit.h>

#define ZYUG_NO_OF_FIELDS 1

namespace Duel{


	void FieldWidget::startup(){
		fieldBackground.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/board/fieldWidgetUV.png");
		fieldBackground.position = pos.bFieldModelPos;
		fieldBackground.scale = pos.bFieldModelScale;
		fieldBackground.ignoreCamera = true;
		//textureLoader.loadTexture("GameData/textures/board/boardUV2.png",
		textureLoader.loadTexture("GameData/textures/board/finalBoard01.png",
			&currentTexture);
		currentField = YUG_BOARD_FIELD_NORMAL;
		chain = YUG_BOARD_FIELD_IDLE;
		Game::WaitUnit::startup();

		fieldName.clear();
		fieldName.push_back(' ');
		fieldName.push_back('-');
		fieldName.push_back(' ');
		fieldName.push_back('\0');
		boostTypes.clear();
		boostTypes.push_back(0);
		weakenTypes.clear();
		weakenTypes.push_back(0);

		currentAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	}

	void FieldWidget::cleanup(){
		fieldBackground.cleanup();
		theBoard.boardModel.textureBO = YUG_UNBIND;
		textureLoader.deleteTexture(&currentTexture);
	}

	void FieldWidget::update(){
		fieldBackground.update();
	}

	void FieldWidget::render(){
		renderWidget();
		renderAnimation();
	}

	void FieldWidget::renderWidget(){
		fieldBackground.render();
		textPrinter.leftAlign = false;
		textPrinter.ignoreCamera = true;
		textPrinter.printText(fieldName.data(),
			YUG_TEXT_INFO_FONT,pos.bFieldTextScale,
			mov.addOffset(fieldBackground.position,pos.bFieldTextOffset), glm::mat4());
		textPrinter.leftAlign = true;//QWERTY
		textPrinter.ignoreCamera = false;//QWERTY
	}
	void FieldWidget::renderAnimation(){
		//for field change animation
	}

	void FieldWidget::applyFieldEffect(Card::CardData* card){
		//ASDFG
	}

	void FieldWidget::changeCurrentField(int newField){
		currentField = newField;
	}

	bool FieldWidget::isCurrent(int field){
		return field == currentField;
	}

	GLuint FieldWidget::getFieldTexture(){
		return currentField;
	}

	glm::vec4 FieldWidget::getFieldAmtran(){
		return currentAmtran;
	}

	void FieldWidget::reveal(){
		fieldBackground.interpolate(pos.bFieldModelPos, pos.wait[3]);
	}
	void FieldWidget::hide(){
		fieldBackground.interpolate(pos.bFieldModelOffPos, pos.wait[3]);
	}

	void FieldWidget::newBoosts(std::vector<int> newBoosts){
		boostTypes.clear();
		for(unsigned int i = 0 ; i < newBoosts.size(); i++){
			boostTypes.push_back(newBoosts[i]);
		}
	}
	void FieldWidget::newWeakens(std::vector<int> newWeakens){
		weakenTypes.clear();
		for(unsigned int i = 0 ; i < newWeakens.size(); i++){
			weakenTypes.push_back(newWeakens[i]);
		}
	}
	void FieldWidget::applyField(Card::CardData* c){
		int type = c->actualType;
		for(unsigned i = 0 ; i < boostTypes.size(); i++){
			if(boostTypes[i] == type){
				alterCardStats(c,500);
				return;
			}
		}
		for(unsigned i = 0 ; i < weakenTypes.size(); i++){
			if(weakenTypes[i] == type){
				alterCardStats(c,-500);
				return;
			}
		}
	}
	void FieldWidget::undoField(Card::CardData* c){
		int type = c->actualType;
		for(unsigned i = 0 ; i < boostTypes.size(); i++){
			c->attack = c->fieldlessAttack;
			c->defense = c->fieldlessDefense;
		}
	}
	void FieldWidget::applyFieldAll(){
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
				applyField(&theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW]);
			}
			if(!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard()){
				applyField(&theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW]);
			}
			if(!theBoard.player.hand[i].blankCard()){
				applyField(&theBoard.player.hand[i]);
			}
			if(!theBoard.enemy.hand[i].blankCard()){
				applyField(&theBoard.enemy.hand[i]);
			}
		}
	}
	void FieldWidget::undoFieldAll(){
		for(unsigned int i = 0; i < 5; i++){
			if(!theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW].blankCard()){
				undoField(&theBoard.board[i][YUG_BOARD_PLAYER_MON_ROW]);
			}
			if(!theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW].blankCard()){
				undoField(&theBoard.board[i][YUG_BOARD_ENEMY_MON_ROW]);
			}
			if(!theBoard.player.hand[i].blankCard()){
				undoField(&theBoard.player.hand[i]);
			}
			if(!theBoard.enemy.hand[i].blankCard()){
				undoField(&theBoard.enemy.hand[i]);
			}
		}
	}

	void FieldWidget::alterCardStats(Card::CardData* c, int amount){
		c->fieldlessAttack = c->attack;
		c->fieldlessDefense = c->defense;
		c->attack += amount;
		if(c->attack < 0)
			c->attack = 0;
		if(c->attack > 9999)
			c->attack = 9999;
		c->altAttack = c->attack;
		c->defense += amount;
		if(c->defense < 0)
			c->defense = 0;
		if(c->defense > 9999)
			c->defense = 9999;
		c->altDefense = c->defense;
	}
}
