#pragma once
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game\Cards\DebugCard.h>
#include <Game\Cards\CardData.h>
#include <Game\ModelLoader.h>
namespace Utility{
	class DebugUnit
	{
	public:
		bool initialize();
		bool shutdown();
		void printMat4(glm::mat4 mat);
		void printVec3(glm::vec3 vec);
		DebugCard debugCard;
		Card::CardData blueEyes;
		Game::ModelLoader boardLoader;
		static DebugUnit debugUnitInstance;
	};
#define debugUnit Utility::DebugUnit::debugUnitInstance
}

