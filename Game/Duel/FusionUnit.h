#pragma once
#include <GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <vector>
#include <Game\Cards\CardData.h>
#include <Game\YugiohUnit.h>
#include <Game\Duel\markingStruct.h>
#include <Game\Duel\GamePlayer.h>
#include <fstream>

#define YUG_FUS_CH_POSITIONING 0
#define YUG_FUS_CH_FUSEDOWN 1
#define YUG_FUS_CH_FUSEUP 2
#define YUG_FUS_CH_SECONDANIMATION 3
#define YUG_FUS_CH_FINALFUSEANIMATION 4
#define YUG_FUS_CH_PASSOVER 5
#define YUG_FUS_CH_FINISHED 6

#define YUG_FUS_CH_ANI_START 7
#define YUG_FUS_CH_GOOD_START 8
#define YUG_FUS_CH_BAD_START 9
#define YUG_FUS_CH_BAD_SHUNT 10
#define YUG_FUS_CH_BAD_RETURN 11
#define YUG_FUS_CH_BAD_END 12
#define YUG_FUS_CH_GOOD_ROTATING 19
#define YUG_FUS_CH_GOOD_FADE_IN 13
#define YUG_FUS_CH_GOOD_FADE_OUT 14
#define YUG_FUS_CH_GOOD_CARD_REVEAL1 15
#define YUG_FUS_CH_GOOD_CARD_REVEAL2 21
#define YUG_FUS_CH_GOOD_CARD_FB1 16
#define YUG_FUS_CH_GOOD_CARD_FB2 20
#define YUG_FUS_CH_GOOD_RETURN 17
#define YUG_FUS_CH_GOOD_END 18

#define YUG_FUS_CH_EQUIP_START 23
#define YUG_FUS_CH_EQ_PULL_IN 24
#define YUG_FUS_CH_EQ_FIRST_SLIDE 25
#define YUG_FUS_CH_EQ_SECOND_SLIDE 26
#define YUG_FUS_CH_EQ_GLOW1 33
#define YUG_FUS_CH_EQ_GLOW2 34
#define YUG_FUS_CH_EQ_CARD_REVEAL1 27
#define YUG_FUS_CH_EQ_CARD_REVEAL2 28
#define YUG_FUS_CH_EQ_CARD_FB1 29
#define YUG_FUS_CH_EQ_CARD_FB2 30
#define YUG_FUS_CH_EQ_RETURN 31
#define YUG_FUS_CH_EQ_END 32
			   
namespace Duel{
			   
	class FusionInfo{
		
	public:
		int secondType;
		int outputCard;
		int lowerAttack;
		int upperAttack;
		FusionInfo(int _secondType, int _lowerAttack, int _upperAttack, int _outputCard):
			secondType(_secondType), lowerAttack(_lowerAttack), upperAttack(_upperAttack), outputCard(_outputCard) {}
		bool checkAtkRange(int atk1, int atk2){
			return(atk1>=lowerAttack && atk2>=lowerAttack && atk1<upperAttack && atk2<upperAttack);
		}
	};

	class FusionUnit : public Game::YugiohUnit
	{
	public:
		glm::vec3 firstCardPosition;
		glm::vec2 cardXYoffset;
		glm::vec3 throwCardPosition;
		std::vector<Card::CardData>cards;
		std::vector<FusionMarkings>markings;

		int chain;
		bool goodFusion;
		int priCard;
		int secCard;
		int outsideIndex;
		bool wasDowning;
		Card::CardData newCard;
		bool isWaiting;
		float totalWait;
		float currentlyWaited;
		bool boardFusion;

		bool initialize();
		bool shutdown();

		void wait(float duration);
		void continueWaiting();

		void takingHandCards();
		void interpolateCards();
		void handFunction(int player);

		void fuseDowns();
		void downRecursion(int insideIndex);
		void fuseUps();
		void upRecursion(int insideIndex);
		void secondaryAnimationUpdate();
		void finalFusePart();
		void pass2Positioner();
		void fusionInterpolate();

		int readFusionList();
		Card::CardData tryFusion();
		void render();
		void update();

		char waitForSymbol(std::ifstream*);
		bool isGeneral(int number);
		int indiviualFusion(int pri, int sec);
		int generalFusion(int pri, int sec);
		int indiviualFusion(Card::CardData& c1, Card::CardData& c2);
		int generalFusion(Card::CardData& c1, Card::CardData& c2);

		//equip cards
		bool specialEquips(int pri, int sec);
		bool tryEquip();
		bool readEquipList();
		bool readingEquipList(bool back);
		bool readingEquipList(Card::CardData& eqCard, Card::CardData& eqMon);
		bool regularFusion();
		int equipMonster;
		int equipCard;
		bool equiping;
		void boostStats();

		//new animation parts
		Game::ModelLoader centreGlow;
		Card::BigCardRender massiveCard;
		glm::vec3 currentCentre;
		bool priCardStrong;
		glm::vec3 returnToPoint;
		glm::vec3 shuntDirection;

		float rotationDuration;
		float rotationSpeed;
		glm::vec3 priStartV;
		glm::vec3 secStartV;
		bool particlesShoot;
		bool renderNewCard;
		bool renderMassiveCard;

		//New animation functions
		void animationUpdates();
		void interpolateCardsToStartPositions();
		void rotateCardsToStartPosition();

		void badStartAnimation();
		float getStrongWeakPriZOffset();
		void interpolateBadCardsTogether();
		void badShuntAnimation();
		void badReturnAnimation();
		void badEnd();

		void goodStartAnimation();
		void goodRotationAnimation();
		void saveStartVectors();
		void growOrb(float time);
		void goodFadeInAnimation();
		void goodFadeOutAnimation();
		void setupNewCard();
		void setupMassiveCard();
		void goodCardReveal1Animation();
		void goodCardFB1Animation();
		void goodCardFB2Animation();
		void goodCardReturnAnimation();
		void goodEnd();

		void equipStartAnimation();
		void equipPullInAnimation();
		void equipFirstSlideAnimation();
		void equipSecondSlideAnimation();
		void equipGlow1Animation();
		void equipGlow2Animation();
		void equipOrb(float time);
		void equipSetUpBigCard();
		void equipCardReveal1Animation();
		void equipCardReveal2Animation();
		void equipCardFB1Animation();
		void equipCardFB2Animation();
		void equipCardReturnAnimation();
		void equipEnd();

		void cardPassover();


		//new animation Utility
		void twoCardInterpolate();
		void threeCardInterpolate();
		void fourCardInterpolate();
		void fiveCardInterpolate();
		void sixCardInterpolate();
		void initializeCentreGlow();
		void initializeCards();
		bool isPriCardStrong();
		void saveReturnToPoint();
		void saveShuntDirection();

		//new fusion type
		std::vector<std::vector<FusionInfo>> generalLists;
		void createFusionLists();
		int tryFuse(Card::CardData& c1, Card::CardData& c2);
		int leftOver(Card::CardData& longCard, Card::CardData& shortCard, int indexCount);
		int tryFuse2(Card::CardData& c1, Card::CardData& c2);
		int tryFuse3(int primaryIndex, int secondIndex, Card::CardData& primaryCard, Card::CardData& secondCard);

		static FusionUnit FusionUnitInstance;
	};

	
#define fusionUnit Duel::FusionUnit::FusionUnitInstance
}

