#pragma once
#include <string>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>
#include <Game\YugiohUnit.h>
#include <Game\Cards\CardData.h>

#define YUG_BOARD_FIELD_NORMAL 0

#define YUG_BOARD_FIELD_IDLE 0
#define YUG_BOARD_FIELD_CHANGING_FIELD 5

namespace Duel{

	class FieldWidget : public Game::YugiohUnit, Game::WaitUnit
	{
	public:
		void startup();
		void cleanup();
		void update();
		void render();
		void renderWidget();
		void renderAnimation();
		void applyFieldEffect(Card::CardData* card);
		void changeCurrentField(int newField);
		bool isCurrent(int field);
		GLuint getFieldTexture();
		glm::vec4 getFieldAmtran();
		void reveal();
		void hide();

		std::vector< char >fieldName;
		int currentField;
		GLuint currentTexture;
		glm::vec4 currentAmtran;
		std::vector<int>boostTypes;
		std::vector<int>weakenTypes;
		void newBoosts(std::vector<int> newBoosts);
		void newWeakens(std::vector<int> newWeakens);
		void applyField(Card::CardData* c);
		void undoField(Card::CardData* c);
		void applyFieldAll();
		void undoFieldAll();
		void alterCardStats(Card::CardData* c, int amount);
		Game::ModelLoader fieldBackground;
		int chain;

	};


}