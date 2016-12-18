#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Utility\Clock.h>
#define GLM_FORCE_RADIANS 
#include <gtc\quaternion.hpp>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>

namespace Card{

	class DarkHole : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		Game::ModelLoader bg;
		std::vector<CardData*> cards;
		std::vector<glm::quat> cardQuats;
		float time;

		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void suckupUpdate();
		void finishUpdate();

		void cleanupAllCards();
		void getAllCardsOnBoard();

	};

}

