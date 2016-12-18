#include <Game\Duel\Parts\BattleCursor.h>
#include <Game\Duel\Board.h>
#include <iostream>
#include <gtc\matrix_transform.hpp>
#include <Utility\TextureLoader.h>
#include <Utility\StateUnit.h>
#include <Game\Animation\Camera.h>
#include <Utility\Clock.h>
#include <Game\Animation\MovementUnit.h>
#include <DefinesAndTypedefs.h>
#include <Game\VectorUnit.h>

namespace Duel{

	void BattleCursor::startup(){
		doRender = true;
		amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		loadOBJ(YUG_PLANE_FILE_PATH,
			(vertices),(uvs),(normals));
		int leftMovement = 0;
		//std::cout<<"Model: vertex size: "<<vertices.size()<<std::endl;
		//std::cout<<"Model: uvs size: "<<uvs.size()<<std::endl;


		vertexNumber = vertices.size();
		glBindVertexArray(0);
		glGenBuffers(1, &verticesBO);
		glBindBuffer(GL_ARRAY_BUFFER, verticesBO);
		glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glGenBuffers(1, &uvBO);
		glBindBuffer(GL_ARRAY_BUFFER, uvBO);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

		GLuint texID1=0; emptyTextures.push_back(texID1 );
		textureLoader.loadTexture("gameData/textures/board/cursors/emptyboardcursor0UV.png", &emptyTextures[0]);
		GLuint texID2=0; emptyTextures.push_back(texID2 );
		textureLoader.loadTexture("gameData/textures/board/cursors/emptyboardcursor1UV.png", &emptyTextures[1]);
		GLuint texID3=0; emptyTextures.push_back(texID3 );
		textureLoader.loadTexture("gameData/textures/board/cursors/emptyboardcursor2UV.png", &emptyTextures[2]);
		GLuint texID7=0; emptyTextures.push_back(texID7 );
		textureLoader.loadTexture("gameData/textures/board/cursors/emptyboardcursor3UV.png", &emptyTextures[3]);

		GLuint texID4=0; fullTextures.push_back(texID4 );
		textureLoader.loadTexture("gameData/textures/board/cursors/fullboardcursor0UV.png", &fullTextures[0]);
		GLuint texID5=0; fullTextures.push_back(texID5 );
		textureLoader.loadTexture("gameData/textures/board/cursors/fullboardcursor1UV.png", &fullTextures[1]);
		GLuint texID6=0; fullTextures.push_back(texID6 );
		textureLoader.loadTexture("gameData/textures/board/cursors/fullboardcursor2UV.png", &fullTextures[2]);
		GLuint texID8=0; fullTextures.push_back(texID8 );
		textureLoader.loadTexture("gameData/textures/board/cursors/fullboardcursor3UV.png", &fullTextures[3]);

		isFlickering = true;
		currentTexture = 0;
		flickerSpeed = 1.0f/pos.wait[1];
		currentDuration = 0.0f;
		newTurnRotation();
		scale = pos.bBoardCursorScale;
		doRender = false;
		move();

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
	void BattleCursor::cleanup(){
		for(unsigned int i = 0; i <4 ; i++){
			textureLoader.deleteTexture(&emptyTextures[i]);
			textureLoader.deleteTexture(&fullTextures[i]);
		}
		emptyTextures.clear();
		fullTextures.clear();
		glDeleteVertexArrays(1, &modelVAO);
		glDeleteBuffers(1, &verticesBO);
		glDeleteBuffers(1, &uvBO);
	}
	void BattleCursor::render(){
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
			if(emptyMode)
				glBindTexture(GL_TEXTURE_2D, emptyTextures[currentTexture]);
			else
				glBindTexture(GL_TEXTURE_2D, fullTextures[currentTexture]);
	
			glDrawArrays(GL_TRIANGLES, 0, vertexNumber );

			glBindVertexArray(0);
	}
	void BattleCursor::move(){
		interpolate(
			mov.addOffset(
				theBoard.getBoardPosition(theBoard.cP[0],theBoard.cP[1]),
				pos.bBoardCursorOffset),
			pos.wait[1]);
	}
	void BattleCursor::newTurnRotation(){
		if(theBoard.playerControlling()){
			rotationMatrix = pos.atkFaceupFlat;
		}else{
			rotationMatrix = pos.eAtkFaceupFlat;
		}
	}
	void BattleCursor::fadeIn(){
		interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f), pos.wait[1]);
	}
	void BattleCursor::fadeOut(){
		interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f), pos.wait[1]);
	}

	void BattleCursor::turnon(){
		doRender = true;
		move();
	}
	void BattleCursor::turnoff(){
		doRender = false;
	}

	void BattleCursor::getNextTexture(){
		currentTexture = (currentTexture+1)%4;
	}
}
