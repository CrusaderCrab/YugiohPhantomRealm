#pragma once
#include <vector>
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\Cards\CardData.h>
#include <Game\WaitUnit.h>
#include <Game\ModelLoader.h>

namespace Card{

	class Raigeki : public MagicSource, public Game::WaitUnit
	{
	public:
		Game::ModelLoader lightnings[7];
		Game::ModelLoader explosion;
		int chain;
		int lNum;
		std::vector<CardData*>cardsToAlter;
		std::vector<glm::vec3>cardPos;
		float time;
		float speed;
		bool playerView;
		float offMove;
		float explosionOff;
		bool expSetOff;


		void startup();
		void cleanup();
		void update();
		void render();

		void startUpdate();
		void fallingUpdate();
		void explosionUpdate();
		void finishUpdate();

		void startUpLightning();
		void updateLNum(float t);
		void removeCards();
		void addThisCardPosition(CardData* c, std::vector<int>& positions);


	};

}