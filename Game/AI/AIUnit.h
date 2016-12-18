#pragma once
#include <vector>
#include <Game\Cards\CardData.h>

#define YUG_AI_NO_MARK -2
#define YUG_AI_CARD_DOWN -3
#define YUG_AI_NEEDED 1
#define YUG_AI_IS_TRAP 2
#define YUG_AI_DANGEROUS 3
#define YUG_AI_NUISANCE 4
#define YUG_AI_TOP_CON 5
#define YUG_AI_BOT_CON 6
#define YUG_AI_1 7
#define YUG_AI_2 8
#define YUG_AI_3 9
#define YUG_AI_4 10
#define YUG_AI_5 11
#define YUG_AI_HIGH_ATTACK 12
#define YUG_AI_HIGH_DEFENSE 13

#define YUG_AI_NO_FUSION 49
#define YUG_AI_LOWEST_AB 50
#define YUG_AI_LOW_AB 51
#define YUG_AI_MID_AB 52
#define YUG_AI_HIGH_AB 53
#define YUG_AI_HIGHEST_AB 54

namespace AI{

	struct AICard{
		bool firstCardOnBoard;
		Card::CardData card;
		std::vector<int>posis;
		AICard();
		AICard(Card::CardData* c);
	};


	class AIUnit
	{
	public:
		virtual void startup();
		virtual void cleanup();
		virtual void calcHandMove();
		virtual void calcBoardMove();
		void dropCurrentKey();
		void pressNextKey();
		bool hasMovesLeft();
		std::vector<int>moveList;
		int moveIndex;

		int boardMarks[5][4];//AI thoughs
		bool bestAttack;
		bool canBreak;
		bool canHold;
		bool attackMagics;
		std::vector<AICard>strongestCards;
		bool hasHandDecision;
		bool hasBoardDecision;

		int highValue, lowValue;
		int theirHighestAtk, theirAtkCard, theirHighestDef, theirDefCard;
		int myHighestAtk, myAtkCard, myHighestDef, myDefCard;
		int handPositon;
		int lowStat; int highStat; int dangerStat;

		int playedCon; bool playFaceUp; bool isFusing;

		int currentCursorLocal[2];
		int currentHandCursorLocal;
		int discardNumber;
		int ability;

		//multi utility
		bool isCardVisible(int theirCol);
		bool isAttackMagic(int myCol);
		bool isAttackMagic(Card::CardData& c);
		int differenceIfAtkFight(Card::CardData& myC, int theirCol);
		int differenceIfDefending(Card::CardData& myC, int theirCol);
		int differenceIfDefFight(Card::CardData& myC, int theirCol);
		int differenceIfCurrentFight(Card::CardData& myC, int theirCol);
		int differenceIfBestFight(Card::CardData& myC, int theirCol);
		int damageFromFight(Card::CardData& myC, Card::CardData& thC);
		int damageFromAtkFight(Card::CardData& myC, Card::CardData& thC);
		int differenceFromDefFight(Card::CardData& myC, Card::CardData& thC);
		bool canBreakTheirCard(int theirCol);
		bool canBreakMyCard(int myCol);
		int versusConstellation(int myCon, int theirCon);
		int constellationComparison(int myCon, int theirCon);
		int neededConstellation(int myCol, int theirCol);
		void setCorrectConstellation(int stIndex, int theirCol);
		//thinking utility
		//--- turn start functions ---
		void newTurnSetup();
		bool isEnemyBoardEmpty();
		bool isEnemyBoardMonsterless();
		bool isEnemyBoardMagicless();
		bool isEnemyBoardTrapless();
		int myNoOfMonsterCards();
		void setHighestCards();
		void boardDoIHaveBestAttack();
		void doIHaveBestAttack();
		void canIBreakThrough();
		void canIHoldHim();
		void doIHaveAttackMagic();
		//--- strongest cards ---
		void calculateStrongestCards();
		void calcStrongest1Cards();
		void calcStrongest2Cards();
		void calcStrongest3Cards();
		void calcStrongest4Cards();
		//--- strongest card with field
		void calcStrongest2BoardCards();
		void calcStrongest3BoardCards();
		void playBoardCard(std::vector<int> cardPosis);
		int choosePosCardPosition();

		void calc3CardFusion(int i, int j, int k);
		void calc3BoardCardFusion(int i, int j, int k);
		void calc4CardFusion(int i, int j, int k, int m);
		AICard getAICard(int i, int j);
		AICard getAICard(int i, int j, int k);

		void removeDuplicateStrongestCards();
		void sortStrongestCardsAttack();
		void sortStrongestCardsDefense();
		AICard getStrongestCard();

		void printStrongestCards();

		//hand decision start
		virtual void setupHandState();
		virtual void decideHandPath();
		virtual void breakThroughHandPath();
		virtual void emptyBoardHandPath();
		virtual void bestAttackHandPath();
		virtual void canHoldHandPath();
		virtual void badPositonHandPath();
		virtual void iHaveNoCardsHandPath();
		virtual void firstTurnHandPath();
		virtual void setUpHighLowStats();
		

		//--- Can Break Through ---
		bool enemyHasTraps();
		int noOfEnemyMagics();
		int canPlayHarpies();	
		bool canWin();
		void playStrongestCard();
		int hasSupportMagics(bool& playThisCardFaceup);
		int hasDamageMagic(); bool isDamageMagic(Card::CardData& c);
		int hasHealMagic(); bool isHealMagic(Card::CardData& c);
		int hasField(); bool isFieldCard(Card::CardData& c);
		bool overlappingFields(int handPos);
		int hasEquip(); bool isEquipCard(Card::CardData& c);
		int hasMiscMagic(); bool isMiscMagic(Card::CardData& c);
		int hasDefensiveTrap(); bool isDefensiveTrap(Card::CardData& c);
		bool opponentHasStrongField();
		//playing cards
		void playCard(int handPos);
		void playMagicCard(int posi);
		void playCard(std::vector<int>cardPosis);
		void cursorTo(int dest);
		//canWin functions
		std::vector<Card::CardData> getCardsFromRow(int row);
		void sortCardsByCurrentStat(std::vector<Card::CardData>& v);
		void sortCardsByAtkStat(std::vector<Card::CardData>& v);
		void sortCardsByDefStat(std::vector<Card::CardData>& v);
		int getCurrentStat(const Card::CardData& c);
		//--- Best Attack ---
		bool foundAndPlayedCardToBreakDefense();
		bool getStrongestEnemyDefenseCard(Card::CardData& c);
		int canBreakStrongestDefenseCardIndex();
		void printCards(std::vector<Card::CardData>& c);
		bool foundAndPlayedAttackMagic();
		//--- Hold him functions ---
		bool foundAndPlayedNewStrongestCard();
		int newStrongestCardIndex();
		bool imSworded();
		bool theirSworded();
		void playMiddlingCard();
		//--- Bad Position Functions ---
		int hasSwords();
		void discardCards(int noToDiscard);
		//--- other hand functions ---
		void markImportantCards();
		int noOfCardsIn(int row);
		bool isFirstTurn();
		AICard tryToAddEquipCard(AICard& aiCard);
		//--- position play ---//
		void pressPlayPosition(Card::CardData& firstCard);
		int bestPosition(Card::CardData& firstCard);
		//---best defense---//
		bool foundAndPlayedNewStrongestDefense();
		int newStrongestDefenseIndex();
		int differenceIfImDefending(Card::CardData& myC, int thCol);


		//---!!--- BATTLE FUNCTIONS ---!!---//
		//One time battles
		void prepareBattleAI();
		void removeAllTrapMarks();
		//Recursion battle functions
		void recursionBattleSetup();
		bool cardsDecided[5];
		void clearCardsDecided();
		void clearLastMonsterMarks();
		void setMonsterMarks();//?
		void setGeneralMonsterMarks();//?
		void markMyHighestAttack();
		void markMyHighestDefense();
		void markMyNeededCards();
		void markHighestAttack();//?
		void markHighestDefense();//?
		virtual void fieldRecursion();
		bool allCardsDecided();

		//field recursion
		void setCursorPos();
		int cursorRow; int cursorCol;
		void boardCursorTo(int col, int row);

		int boardSupportMagic();
		bool isBoardSupportMagic(int col);
		int boardMiscMagic();
		bool isBoardMiscMagic(int col);
		bool worthStopDef(); bool worthDarkLight(); bool worthCurse();
		void activateSupportMagic(int posi);
		void boardEquipCards(int& magicPos, int& monPos);//
		void activateEquipCard(int magPos, int monPos);//
		int boardAttackMagic();
		bool isBoardAttackMagic(int posi);

		std::vector<int> myCurrentMonstersList();//
		std::vector<int> theirCurrentMonsterList();//
		void sortRowList(std::vector<int>& cardPosis, int row);//

		bool thinkAboutAttack(int myCol, int theirCol);
		bool thinkAboutDirectAttack(int myCol);
		bool putInBestPosition(int col);
		bool higherAttackThenThis(Card::CardData& c);
		virtual bool defenseOnlyMon(int myCol);

		bool thinkAboutDamage(int damage, int myCol, int thCol);
		bool thisDefenseIsNeeded(int myCol);
		bool boardCanBreakTheirCard(int enemyPos);
		//bool secondStrongestAttack(int theirCol);
	};


}