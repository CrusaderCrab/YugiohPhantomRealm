#pragma once
#include <Game\Cards\BigCardRender.h>//contains glew.h
#include <Game\Cards\SmallCardRender.h>
#include <vector>
#include <DefinesAndTypedefs.h>
#include <Game\YugiohUnit.h>

#define YUG_CARD_NO_CURRENT_CON -2
#define YUG_CARD_CH_IDLE 0
#define YUG_CARD_CH_START_MAGIC 5

namespace Card{
	class CardData : public Game::YugiohUnit
	{
		int constellationComparison(int enemyCon);
	public:
		GLuint pictureTBO;//will start == unbind
		int chain;
		int cardNumber;
		std::vector<char>name;
		std::vector<char>renderFileName;
		std::vector<char>blurb;
		int monMagTrap;
		int attack;
		int defense;
		int altAttack;//for temporary ups/downs ie constellations
		int altDefense;
		int origAttack;
		int origDefense;
		int fieldlessAttack;
		int fieldlessDefense;
		int atkStatBoost; int defStatBoost;
		int atkStatDrop; int defStatDrop;
		int starchips;//the stars on the card, not its worth
		int actualType;
		std::vector<int> fusionTypes;
		int element;
		int constellations[2];
		int currentConstellation;
		//board
		bool faceUp;
		bool attackMode;
		bool hasAttacked;
		bool isVisible;
		bool hidden;
		//render
		Card::BigCardRender bigRender;
		Card::SmallCardRender smallRender;

		virtual void startup();
		virtual void cleanup();
		virtual void update();
		virtual void input();
		virtual void effect(int info);//for magic,trap cards
		virtual void listen(int info);//for trap cards
		//battle
		virtual void newTurn();
		void setConstellation(int chosenConstellation);
		void setBattleMode(int mode);//attack/defense mode
		void setFacePosition(int position);//faceup/facedown position
		//utility
		bool blankCard();
		bool hasThisAttribute(int attrib);
		bool hasThisAttribute(int attrib, int index);
		bool canUseEquip(int num);
		//battle
		int versusConstellation(int enemyCon);
		void alterAttack(int change);
		void alterAltAttack(int change);
		void alterDefense(int change);
		void alterAltDefense(int change);
		void alterType(int newType);
		void addFusionType(int newType);

		virtual void duelPrintData(glm::vec3 position);
		virtual void duelPrintConstellation(glm::vec3 position);
		virtual void trunkPrintData(glm::vec3 position);
		void attacked();

	};



}

