#pragma once
#define GLM_FORCE_RADIANS
#include <glm.hpp>

namespace Animation{
	class Camera 
	{
		
		//data for interpolating
		float interpolateSpeed;
		float currentInterpolateDuration;
		glm::vec3 interpolateStart;
		glm::vec3 interpolateDestination;
		//update flags
		bool isInterpolating;
		bool isRotating;
		bool isChangingDirection;
		//data for rotating
		float rotateSpeed;
		float currentRotationDuration;
		float rotationDestination;
		glm::vec3 startingRotationDirection;
		glm::vec3 currentRotationLine;
		//data for changing view direction
		float directionChangeSpeed;
		float currentDirectionChangeDuration;
		float directionDestination;
		glm::vec3 startingViewDirection;
		glm::vec3 endingViewDirection;

		void continueInterpolate();
		void continueRotation();
		void continueChangeDirection();

		Camera();
		Camera(const Camera&);
		Camera& operator=(const Camera&);
	public:
		glm::vec3 position;
		glm::vec3 viewDirection;
		glm::vec3 UP;

		bool initialize();
		bool shutdown();
		void resetCamera();
		glm::mat4 getWorldToViewMatrix() const;
		glm::vec3 getPosition() const;
		void interpolate(
			glm::vec3 newPosition,
			float duration);

		void rotate(glm::vec3 rotationLine,
			float finalAngle,
			float duration);

		void interpolateViewDirection(
			glm::vec3 newDirection,
			float duration);

		void update();
		void keyMove();//debug function

		static Camera cameraInstance;

	};
#define camera Animation::Camera::cameraInstance
}

