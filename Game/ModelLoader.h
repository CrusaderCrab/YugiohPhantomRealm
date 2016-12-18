#pragma once
#include<GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <ThirdParty\objloader.hpp>

#define YUG_MODEL_PLANE 0
namespace Game{

	class ModelLoader
	{
		void continueInterpolate();
		void continueRotation();
		void continueScaling();
		void continueAmtraning();
	public:
		GLuint verticesBO;
		GLuint uvBO;
		GLuint textureBO;
		GLuint modelVAO;
		bool doRender;

		void interpolate(
				glm::vec3 newPosition,
				float duration);
		float interpolateSpeed;
		float currentInterpolateDuration;
		glm::vec3 interpolateStart;
		glm::vec3 interpolateDestination;
		bool isInterpolating;

		void rotate(glm::vec3 rotationLine,
				float finalAngle,
				float duration);
		float rotateSpeed;
		float currentRotationDuration;
		glm::mat4 totalRequestedRotation;
		glm::mat4 startingRotationMatrix;
		bool isRotating;

		void rotate(glm::mat4 rotationMat,
				float duration);

		void scaleInterpolate(glm::vec3 newScale,
				float duration);
		float scaleSpeed;
		float currentScaleDuration;
		glm::vec3 scaleStart;
		glm::vec3 scaleDestination;
		bool isScaling;

		 void interpolateAmtran(glm::vec4 newAmtran,
			float duration);
		float amtranSpeed;
		float currentAmtranDuration;
		glm::vec4 destinationAmtran;
		glm::vec4 startingAmtran;
		bool isAmtraning;

		glm::vec4 amtran;
		glm::vec3 position;
		glm::mat4 rotationMatrix;
		glm::vec3 scale;
		bool ignoreCamera;

		int vertexNumber;
		virtual void startup(const char* objFileName, const char* textureFileName);
		void startup(int basicShape, const char* textureFileName);
		virtual void cleanup();
		virtual void render();
		virtual void update();
	};

}