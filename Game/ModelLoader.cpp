//#include "windows.h"
//#include "psapi.h"

#include <Game\ModelLoader.h>
#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <gtc\quaternion.hpp>
#include <Utility\TextureLoader.h>
#include <Utility\StateUnit.h>
#include <Game\Animation\Camera.h>
#include <Utility\DebugUnit.h>
#include <Utility\ErrorHandler.h>
#include <Utility\Clock.h>
#include <iostream>

namespace{

};
namespace Game{

	void ModelLoader::startup(const char* objFileName, const char* textureFileName)
	{
		
		doRender = true;
		isAmtraning = false;
		isScaling = false;
		isRotating = false;
		isInterpolating = false;
		amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		loadOBJ(objFileName,
			(vertices),(uvs),(normals));
		int leftMovement = 0;
		

		textureBO = 0;
		vertexNumber = vertices.size();
		glBindVertexArray(0);
		glGenBuffers(1, &verticesBO);
		glBindBuffer(GL_ARRAY_BUFFER, verticesBO);
		glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glGenBuffers(1, &uvBO);
		glBindBuffer(GL_ARRAY_BUFFER, uvBO);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
		textureLoader.loadTexture(textureFileName, &textureBO);
		

		glBindVertexArray(0);
		glGenVertexArrays(1, &modelVAO);
		glBindVertexArray(modelVAO);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, verticesBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBO);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glBindVertexArray(0);

		
	}

	void ModelLoader::cleanup(){

		

		glDeleteBuffers(1, &verticesBO);
		glDeleteBuffers(1, &uvBO);
		glDeleteVertexArrays(1, &modelVAO);

		

		textureLoader.deleteTexture(&textureBO);

	}

	void ModelLoader::update(){
			if(isInterpolating)
				continueInterpolate();
			if(isRotating)
				continueRotation();
			if(isScaling)
				continueScaling();
			if(isAmtraning)
				continueAmtraning();
	}

	void ModelLoader::render()
	{
		if(!doRender)
			return;
		glDisable(GL_CULL_FACE);
		if(!stateUnit.isActiveShaderProgram(YUG_TEXTURE_SHADER_PROGRAM)){
				stateUnit.useShaderProgram(YUG_TEXTURE_SHADER_PROGRAM);
		}

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(! ignoreCamera )
			projectionMatrix *= camera.getWorldToViewMatrix();
		glm::mat4 translationMatrix = glm::translate(projectionMatrix, position );
		glm::mat4 fullTransfromMatrix = translationMatrix * rotationMatrix;
		glm::mat4 finalMatrix = glm::scale(fullTransfromMatrix, scale);

		glUniformMatrix4fv( 
		stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);
		glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);
		glBindVertexArray(0);
		glBindVertexArray(modelVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBO);

	
		glDrawArrays(GL_TRIANGLES, 0, vertexNumber );

		glBindVertexArray(0);
	}

	void ModelLoader::interpolate(
			glm::vec3 newPosition,
			float duration)
	{
		if(isInterpolating){
			//errorHandler.printError("Model Loader: new interpolating call interrupting one in progress.");
		}
		interpolateStart = position;
		interpolateDestination = newPosition;
		interpolateSpeed = 1.0f/duration;
		//interpolateDuration = duration;
		currentInterpolateDuration = 0.0f;
		isInterpolating = true;

	}

	void ModelLoader::continueInterpolate(){
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

		void ModelLoader::rotate(glm::vec3 rotationLine,
			float finalAngle,
			float duration)
	{
			if(isRotating){
				//errorHandler.printError("Model Loader: new rotation call interrupting one in progress.");
			}
			startingRotationMatrix = rotationMatrix;
			totalRequestedRotation = glm::rotate(finalAngle, rotationLine);
			rotateSpeed = 1.0f/duration;
			currentRotationDuration = 0.0f;
			isRotating = true;
			
	}

	void ModelLoader::rotate(glm::mat4 rotationMat, float duration){
		startingRotationMatrix = rotationMatrix;
		totalRequestedRotation = rotationMat;
		rotateSpeed = 1.0f/duration;
		currentRotationDuration = 0.0f;
		isRotating = true;
		//std::cout<<"Model Loader: starting Mat:\n";
		//debugUnit.printMat4(startingRotationMatrix);

	}

	void ModelLoader::continueRotation(){
		currentRotationDuration += rotateSpeed*gameClock.lastLoopTime();
		if(currentRotationDuration < 1.0f){
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

	void ModelLoader::scaleInterpolate(glm::vec3 newScale,
				float duration)
	{
		if(isScaling){
			//errorHandler.printError("Model Loader: new scaling call interrupting one in progress.");
		}
		scaleStart = scale;
		scaleDestination = newScale;
		scaleSpeed = 1.0f/duration;
		currentScaleDuration = 0.0f;
		isScaling = true;
		//std::cout<<"start scale: "<<scale.x<<" "<<scale.y<<" "<<scale.z<<"\n ";
	}

	void ModelLoader::continueScaling(){
		currentScaleDuration += (scaleSpeed*gameClock.lastLoopTime());
		if(currentScaleDuration < 1.0f){
			scale = glm::mix(
				scaleStart, 
				scaleDestination, 
				currentScaleDuration);
			//std::cout<<"current scale: "<<scale.x<<" "<<scale.y<<" "<<scale.z<<"\n ";
		}else{
			scale = scaleDestination;
			isScaling = false;
		}
	}


	void ModelLoader::interpolateAmtran(glm::vec4 newAmtran,
			float duration)
	{
		//if(isAmtraning){
		//	errorHandler.printError("Model loader: new amtraning call interrupting one in progress.");
		//}
		startingAmtran = amtran;
		destinationAmtran = newAmtran;
		amtranSpeed = 1.0f/duration;
		currentAmtranDuration = 0.0f;
		isAmtraning = true;
	}

	void ModelLoader::continueAmtraning(){
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