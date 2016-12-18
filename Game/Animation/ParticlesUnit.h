#pragma once
#include <GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game\ModelLoader.h>

#define YUG_PARTICLE_BURN_NO_BURN 0
#define YUG_PARTICLE_BURN_MON 1
#define YUG_PARTICLE_BURN_NO 200

namespace Animation{

	class ParticlesUnit
	{
	public:
	
		GLuint VBO;
		GLuint uvBO;
		GLuint textureBangBO;
		GLuint textureSuckBO;
		GLuint textureBurnBO;
		GLuint textureAltBurnBO;
		GLuint textureTrapBO;
		GLuint VAO;
		int vertexNumber;
		Game::ModelLoader cardBlocker;

		glm::vec4 particleAmtran;

		bool doRender;
		bool doUpdate;
		bool ignoreCamera;
		int currentAnimation;

		bool initialize();
		bool shutdown();

		void update();
		void render();

		void renderParticleBang();
		void particleBang(glm::vec3 centre, float duration);
		glm::vec3 centre;

		void renderParticleSuck();
		void particleSuck(glm::vec3 centre, float duration);
		void getTextureFromConstellation(int con);

		void renderParticleRise();
		void particleRise(std::vector<glm::vec3> centres, glm::vec4 lightColour,float duration, bool playerView);
		std::vector<glm::vec3>risingLightCentres;
		glm::vec4 risingColour;
		bool playerView;

		void renderParticleCardHit();
		void particleCardHit(std::vector<glm::vec3> centres, glm::vec4 lightColour,float duration, bool playerView);
		float zPlayerOffset;

		void renderTrapParticles();
		void trapParticles(glm::vec3 centre, float duration, bool player);
		float circleXOffset;
		float circleZOffset;
		glm::mat4 trapRotation;

		float burnLifespans[YUG_PARTICLE_BURN_NO];
		glm::vec3 burnPositions[YUG_PARTICLE_BURN_NO];
		void burnUpdate();
		void renderBurnup();
		void burnup(int type,glm::vec3 start, float width, float height, float duration);
		glm::vec3 burnStartPoint;
		float burnHeight;
		float burnWidth;
		glm::vec3 burnParticleScale;
		int burnupType;
		void swapFlames();

		void continueCount();
		void startCount(float duration);
		float countSpeed;
		float currentCount;
		bool counting;

		static ParticlesUnit particleUnitInstance;

	};
#define particleUnit Animation::ParticlesUnit::particleUnitInstance
}