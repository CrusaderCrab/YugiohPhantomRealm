#pragma once
#include <Game\ModelLoader.h>
#include <glm.hpp>

#define YUG_FADE_GLOBAL 0
#define YUG_FADE_LOCAL 1
#define YUG_FADE_WHITE glm::vec4(1.0f,1.0f,1.0f,1.0f)
#define YUG_FADE_BLACK glm::vec4(0.001f,0.0f,0.0f,1.0f)
#define YUG_FADE_DARK  glm::vec4(0.01f,0.0f,0.0f,0.75f)
#define YUG_FADE_CLEAR glm::vec4(1.0f,1.0f,1.0f,0.0f)
#define YUG_FADE_BLACK_CLEAR glm::vec4(0.001f,0.0f,0.0f,0.0f)


namespace Animation{

	class FadeUnit
	{
	public:
		bool initialize();
		bool shutdown();
		void update();
		void render(int location);
		void fadeTo(glm::vec4 newAmtran, float duration);
		void changeZ(float newZ);
		void continueFading();

		bool doRender;
		bool renderBlock;
		bool fading;
		float currentDuration;
		float fadeSpeed;
		glm::vec4 pastAmtran;
		glm::vec4 destinationAmtran;
		float currentZ;

		Game::ModelLoader sheet;
		static FadeUnit FadeUnitInstance;
	};
#define fadeUnit Animation::FadeUnit::FadeUnitInstance

}
