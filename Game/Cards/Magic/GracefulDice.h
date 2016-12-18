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

	class GracefulDice : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		Game::ModelLoader dice;
		int gain;

		void startup();
		void cleanup();
		void update();
		void render();

		glm::vec3 firstRotatePoint;
		glm::vec3 secondRotatePoint;
		float rotateSpeed;
		float rotateDuration;
		float currentRotation;
		glm::vec3 rotationLine;
		glm::quat startingRotation;
		void rotationUpdate();
		void calcDiceRotation();

		void startUpdate();
		void firstRotationUpdate();
		void secondRotationUpdate();
		void fadeTextInUpdate();
		void fadeCardInUpdate();
		void particleRiseUpdate();
		void finishFadeInCount();
		void startFadeOutCount();
		void fadeCardOutUpdate();
		void fadeTextOutUpdate();
		void finishupdate();

		void chooseResult();
		void calcStartingRotation();
		void setupDice();
		void increaseValues();
		std::vector<glm::vec3> getCardCentres();

		float rotationTime;
		float rotationSpeed;
		bool renderText;
		std::vector<CardData*>cardsToAlter;
		void alterCardStats();


	};

}