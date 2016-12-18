#include "windows.h"
#include "psapi.h"

#include <Game\Animation\Camera.h>//contains GL_FORCE_RADIANS ,glm.hpp
#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <Utility\Clock.h>
#include <Utility\StateUnit.h>
//#include <Game\YugiohUnit.h>
#include <Utility\ErrorHandler.h>


#include <Game\Animation\ParticlesUnit.h>
#include <Game\VectorUnit.h>
#include <Game\Cards\Trap\TrapUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\EnemyData.h>

#include <Utility\DebugUnit.h>
#include <Utility\InputUnit.h>
#include <Game\Duel\BattleUnit.h>
#include <Game\Duel\Board.h>
#include <iostream>

#include <Game\VectorUnit.h>

#define YUG_CAM_DEFAULT_UP_VEC 0.0f, 1.0f, 0.0f
#define YUG_CAM_DEFAULT_VIEW_VEC 0.0f, 0.0f, -1.0f
#define YUG_CAM_DEFAULT_POS_VEC 0.0f, 0.0f, 0.0f

namespace Animation{

	Camera Camera::cameraInstance;

	Camera::Camera() : 
		position(YUG_CAM_DEFAULT_POS_VEC),
		viewDirection(YUG_CAM_DEFAULT_VIEW_VEC),
		UP(YUG_CAM_DEFAULT_UP_VEC),
		isInterpolating(false),
		isRotating(false)
	{
	}

	bool Camera::initialize(){
		position = glm::vec3(YUG_CAM_DEFAULT_POS_VEC);
		viewDirection = glm::vec3(YUG_CAM_DEFAULT_VIEW_VEC);
		UP = glm::vec3(YUG_CAM_DEFAULT_UP_VEC);
		isInterpolating = (false);
		isRotating = (false);
		return true;
	}
	bool Camera::shutdown(){
		return true;
	}

	void Camera::resetCamera(){
		position = glm::vec3(YUG_CAM_DEFAULT_POS_VEC);
		viewDirection = glm::vec3(YUG_CAM_DEFAULT_VIEW_VEC);
	}

	glm::mat4 Camera::getWorldToViewMatrix() const{
		return glm::lookAt(
			position, 
			position + viewDirection,
			UP);
	}

	glm::vec3 Camera::getPosition() const{
		return position;
	}

	void Camera::update(){
		if(isInterpolating){
			continueInterpolate();
		}
		if(isRotating){
			continueRotation();
		}
		if(isChangingDirection){
			continueChangeDirection();
		}

	}

	void Camera::interpolate(glm::vec3 newPosition, float duration){
		if(isInterpolating){
			//errorHandler.printError("Camera: new interpolating call interrupting one in progress.");
		}
		interpolateStart = position;
		interpolateDestination = newPosition;
		interpolateSpeed = 1.0f/duration;
		//interpolateDuration = duration;
		currentInterpolateDuration = 0.0f;
		isInterpolating = true;
	}

	void Camera::continueInterpolate(){
		currentInterpolateDuration += (interpolateSpeed*gameClock.lastLoopTime());
		if(currentInterpolateDuration < 1.0f){
			position = glm::mix(
				interpolateStart, 
				interpolateDestination, 
				currentInterpolateDuration);
		}else{
			position = interpolateDestination;
			isInterpolating = false;
		}
	}

	void Camera::rotate(glm::vec3 rotationLine,
			float finalAngle,
			float duration){
			if(isRotating){
				//errorHandler.printError("Camera: new rotation call interrupting one in progress.");
			}
			startingRotationDirection = viewDirection;
			rotationDestination = finalAngle;
			//rotationDuration = duration;
			rotateSpeed = 1/duration;
			currentRotationDuration = 0.0f;
			currentRotationLine = rotationLine;
			isRotating = true;
	}

	void Camera::continueRotation(){
		currentRotationDuration += rotateSpeed*gameClock.lastLoopTime();
		if(currentRotationDuration < 1.0f){
			viewDirection =  
				glm::mat3(
				glm::rotate(
				rotationDestination*currentRotationDuration, 
				currentRotationLine))
				* startingRotationDirection;

		}else{
			viewDirection =  
				glm::mat3(
				glm::rotate(rotationDestination, currentRotationLine))
				* startingRotationDirection;
			isRotating = false;
		}
	}

	void Camera::interpolateViewDirection(glm::vec3 newDirection,
			float duration){
		if(isChangingDirection){
			//errorHandler.printError("Camera: new view Direction call interrupting one in progress.");
		}
		//std::cout<<"View interp called\n";
		//debugUnit.printVec3(newDirection);
		startingViewDirection = viewDirection;
		endingViewDirection = newDirection;
		directionChangeSpeed = 1.0f/duration;
		currentDirectionChangeDuration = 0.0f;
		isChangingDirection = true;
	}

	void Camera::continueChangeDirection(){
		currentDirectionChangeDuration 
			+= (directionChangeSpeed*gameClock.lastLoopTime());
		if(currentDirectionChangeDuration < 1.0f){
			viewDirection = glm::mix(
				startingViewDirection, 
				endingViewDirection, 
				currentDirectionChangeDuration);
		}else{
			viewDirection = endingViewDirection;
			isChangingDirection = false;
		}

	}



	//debug fuction, remember to remove it
	void Camera::keyMove(){
		/*if(inputUnit.isKeyActive(YUG_KEY_TRI)){
			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
			SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
			std::cout<<"Total PHYS MEM USING NOW: "<<physMemUsedByMe<<std::endl;
		}*/
	}

}


