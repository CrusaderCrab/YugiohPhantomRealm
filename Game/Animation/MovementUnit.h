#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game/Cards/CardData.h>
#include <Game/YugiohUnit.h>


namespace Animation{

	class MovementUnit : public Game::YugiohUnit
	{
	public:

		std::vector<Card::CardData*> currentCards;

		bool initialize();
		bool shutdown();
		void update();

		glm::vec3 rotateAround(glm::vec3 centre, float distance, float angle);
		glm::vec3 rotateAroundXZ(glm::vec3 centre, glm::vec3 rotateAround, float distance, float angle);
		glm::vec3 rotateVectorX(glm::vec3 startV, float angle);
		glm::vec3 rotateVectorZ(glm::vec3 startV, float angle);
		glm::vec3 addOffset(glm::vec3 origin, glm::vec3 offset);
		glm::vec3 addXOffset(glm::vec3 origin, float x);
		glm::vec3 addYOffset(glm::vec3 origin, float y);
		glm::vec3 addZOffset(glm::vec3 origin, float z);

		void flipBig(Card::CardData* card, float duration);
		void flipSmall(Card::CardData* card, float duration);

		static MovementUnit MovementUnitInstance;
	};
#define mov Animation::MovementUnit::MovementUnitInstance
}