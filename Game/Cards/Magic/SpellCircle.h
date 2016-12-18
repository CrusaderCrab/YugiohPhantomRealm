#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>

namespace Card{

	class SpellCircle : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		Game::ModelLoader circle;
		std::vector<CardData*> cardsToAlter;
		float cardTime;
		float cardSpeed;

		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void circleFadeInUpdate();
		void cardFadeInUpdate();
		void finishFadeInUpdate();

		void fadeOutUpdate();
		void finishUpdate();

		void alterCardStats();
		std::vector<glm::vec3> getCardCentres();
		void addToCircleRotation();
		int atkCalcLose(CardData* c);
		int defCalcLose(CardData* c);

	};

}
