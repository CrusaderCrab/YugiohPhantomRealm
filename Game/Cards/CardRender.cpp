#include <Game\Cards\CardRender.h>
#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <DefinesAndTypedefs.h>
#include <Utility\ErrorHandler.h>
#include <Utility\Clock.h>

#include <Utility\DebugUnit.h>
#include <iostream>
#include <gtc/quaternion.hpp>

namespace Card{
	//defining the static vars for linking later
	GLushort CardRender::cardFrontIndices[6];
	GLushort CardRender::cardBackIndices[6];
	GLuint CardRender::cardFrontIndicesIBO; 
	GLuint CardRender::cardBackIndicesIBO; 

	//once off call for all CardRender objects to set up static stuff
	bool CardRender::initialize(){
		cardFrontIndices[0] = 0;
		cardFrontIndices[1] = 1;
		cardFrontIndices[2] = 2;
		cardFrontIndices[3] = 0;
		cardFrontIndices[4] = 2;
		cardFrontIndices[5] = 3;

		cardBackIndices[0] = 2;
		cardBackIndices[1] = 1;
		cardBackIndices[2] = 0;
		cardBackIndices[3] = 3;
		cardBackIndices[4] = 2;
		cardBackIndices[5] = 0;

		glBindVertexArray(YUG_UNBIND);
		glGenBuffers(1, &cardFrontIndicesIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cardFrontIndicesIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cardFrontIndices), 
					&cardFrontIndices, GL_STATIC_DRAW);
		glGenBuffers(1, &cardBackIndicesIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cardBackIndicesIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cardBackIndices), 
					&cardBackIndices, GL_STATIC_DRAW);
		return true;
	}

	bool CardRender::shutdown(){
		glDeleteBuffers(1, &cardFrontIndicesIBO);
		glDeleteBuffers(1, &cardBackIndicesIBO);
		return true;
	}

	bool CardRender::startup(){
		isInterpolating = false;
		isRotating = false;
		doRender = false;
		ignoreCamera = false;
		position = glm::vec3(0.0f, 0.0f, -1.5f);
		scale = glm::vec3(1.0f,1.0f,1.0f);
		rotationMatrix = glm::rotate(0.2f,glm::vec3(0.0f, 1.0f, 0.0f));
		amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		isAmtraning = false;
		return true;
	}

	bool CardRender::cleanup(){
		return true;
	}

	void CardRender::update(){
		if(isInterpolating)
			continueInterpolate();
		if(isRotating)
			continueRotation();
		if(isAmtraning)
			continueAmtraning();
	}

	void CardRender::render(){
		errorHandler.printError("Base Card Render's render function called");
	}

	void CardRender::interpolate(
			glm::vec3 newPosition,
			float duration)
	{
		if(isInterpolating){
			//errorHandler.printError("Card Render: new interpolating call interrupting one in progress.");
		}
		interpolateStart = position;
		interpolateDestination = newPosition;
		interpolateSpeed = 1.0f/duration;
		//interpolateDuration = duration;
		currentInterpolateDuration = 0.0f;
		isInterpolating = true;

	}

	void CardRender::continueInterpolate(){
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

	void CardRender::rotate(glm::vec3 rotationLine,
			float finalAngle,
			float duration)
	{
			if(isRotating){
				//errorHandler.printError("Card Render: new rotation call interrupting one in progress.");
			}
			startingRotationMatrix = rotationMatrix;
			totalRequestedRotation = glm::rotate(finalAngle, rotationLine);
			rotateSpeed = 1.0f/duration;
			currentRotationDuration = 0.0f;
			isRotating = true;
	}

	void CardRender::rotate(glm::mat4 rotationMat, float duration){
		startingRotationMatrix = rotationMatrix;
		totalRequestedRotation = rotationMat;
		rotateSpeed = 1.0f/duration;
		currentRotationDuration = 0.0f;
		isRotating = true;

	}

	void CardRender::continueRotation(){
		currentRotationDuration += rotateSpeed*gameClock.lastLoopTime();
		if(currentRotationDuration < 1.0f){
			/*rotationMatrix = glm::mix(
				startingRotationMatrix, 
				totalRequestedRotation, 
				currentRotationDuration);
		}else{
			rotationMatrix = glm::mix(startingRotationMatrix, totalRequestedRotation, 1.0f);
			isRotating = false;
			std::cout<<"end:\n";*/
			glm::quat start(startingRotationMatrix);
			glm::quat end(totalRequestedRotation);
			rotationMatrix = glm::mat4_cast(glm::mix(start, end, currentRotationDuration));
		}else{
			glm::quat start(startingRotationMatrix);
			glm::quat end(totalRequestedRotation);
			rotationMatrix = glm::mat4_cast(glm::mix(start, end, 1.0f));
			isRotating = false;
		}
	}


	void CardRender::interpolateAmtran(glm::vec4 newAmtran,
			float duration)
	{
		if(isAmtraning){
			//errorHandler.printError("Card Render: new amtraning call interrupting one in progress.");
		}
		startingAmtran = amtran;
		destinationAmtran = newAmtran;
		amtranSpeed = 1.0f/duration;
		currentAmtranDuration = 0.0f;
		isAmtraning = true;
	}

	void CardRender::continueAmtraning(){
		currentAmtranDuration += (amtranSpeed*gameClock.lastLoopTime());
		if(currentAmtranDuration < 1.0f){
			amtran = glm::mix(
				startingAmtran, 
				destinationAmtran, 
				currentAmtranDuration);
		}else{
			amtran = destinationAmtran;
			isAmtraning = false;
		}
	}
}
