#pragma once
#include <GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game\ModelLoader.h>
#include <Game\Cards\BigCardRender.h>
#include <Game\YugiohUnit.h>
#include <string>

#define YUG_CDU_CH_IDLE 0
#define YUG_CDU_CH_FLIP_CARD 3
#define YUG_CDU_CH_HAS_INPUT 1
#define YUG_CDU_CH_RETURN_CONTROL 2
namespace Card{

	class CardDisplayUnit : public Game::YugiohUnit
	{
		CardDisplayUnit(){}
		CardDisplayUnit(const CardDisplayUnit&);
		CardDisplayUnit& operator=(const CardDisplayUnit&);
		void endDisplay();

		glm::vec3 offScreenCardPos;
		glm::vec3 offScreenBoardPos;
		glm::vec3 onScreenCardPos;
		glm::vec3 onScreenBoardPos;
		glm::vec3 cardScale;
		
		glm::vec3 typeOffset;
		glm::vec3 consOffset;
		glm::vec3 blurbOffset;


	public:
		bool initialize();
		bool shutdown();
		Game::ModelLoader displayBoard; 
		Card::BigCardRender bigCard;
		bool doRender;
		std::vector<std::string>blurbLines;

		void displayCard(Card::BigCardRender);


		void update();
		void input();
		void render();
		void renderInfo();
		int chain;

		void wait(float duration);
		void continueWaiting();
		float waitDuration;
		float currentlyWaited;
		bool isWaiting;

		static CardDisplayUnit cardDisplayUnitInstance;
	};
#define cardDisplayUnit Card::CardDisplayUnit::cardDisplayUnitInstance
}
