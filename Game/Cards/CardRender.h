#pragma once
#include <GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <Game\YugiohUnit.h>
//#include <Game\Cards\CardData.h>

namespace Card{
	class CardData;
	class CardRender : public Game::YugiohUnit
	{
	protected:
		static GLushort cardFrontIndices[6];
		static GLushort cardBackIndices[6];
		
		static GLuint cardFrontIndicesIBO;
		static GLuint cardBackIndicesIBO;

		void continueInterpolate();
		void continueRotation();
		void continueAmtraning();

	public:
		Card::CardData* parentCard;
		virtual bool initialize();//at game start, for static parts
		virtual bool shutdown();//clearing static parts ang GL buffer
		virtual bool startup();//getting default positions loaded
		virtual bool cleanup();
		virtual void update();
		virtual void render();

		virtual void interpolate(
			glm::vec3 newPosition,
			float duration);
		float interpolateSpeed;
		float currentInterpolateDuration;
		glm::vec3 interpolateStart;
		glm::vec3 interpolateDestination;
		bool isInterpolating;

		virtual void rotate(glm::vec3 rotationLine,
			float finalAngle,
			float duration);
		float rotateSpeed;
		float currentRotationDuration;
		glm::mat4 totalRequestedRotation;
		glm::mat4 startingRotationMatrix;
		bool isRotating;

		virtual void rotate(glm::mat4 rotationMat,
			float duration);

		virtual void interpolateAmtran(glm::vec4 newAmtran,
			float duration);
		float amtranSpeed;
		float currentAmtranDuration;
		glm::vec4 destinationAmtran;
		glm::vec4 startingAmtran;
		bool isAmtraning;
	
		bool doRender;
		bool ignoreCamera;
		glm::vec4 amtran;
		glm::vec3 position;
		glm::vec3 scale;
		glm::mat4 rotationMatrix;


	};

}