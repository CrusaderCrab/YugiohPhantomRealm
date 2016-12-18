#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Utility\Clock.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>

namespace Card{

	class ShadowSpell : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		Game::ModelLoader chainModel;
		std::vector<CardData*> cardsToAlter;
		float chainTime;
		bool renderChains;
		bool explosionSet;

		void startup();
		void cleanup();
		void update();
		void render();
		void renderingChains();

		void startUpdate();
		void chainFallUpdate();
		void finishUpdate();

		void alterCardStats();
		int atkCalcLose(CardData* c);
		int defCalcLose(CardData* c);


	};

}