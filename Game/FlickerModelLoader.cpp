#include <Game\FlickerModelLoader.h>
#include <iostream>
#include <gtc\matrix_transform.hpp>
#include <Utility\TextureLoader.h>
#include <Utility\StateUnit.h>
#include <Game\Animation\Camera.h>
#include <Utility\Clock.h>

namespace Game{

	
		void FlickerModelLoader::startup(const char* objFileName, const char* firstTexture)
		{
		doRender = true;
		amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		loadOBJ(objFileName,
			(vertices),(uvs),(normals));
		int leftMovement = 0;
		//std::cout<<"fModel: vertex size: "<<vertices.size()<<std::endl;
		//std::cout<<"fModel: uvs size: "<<uvs.size()<<std::endl;

		vertexNumber = vertices.size();
		glBindVertexArray(0);
		glGenBuffers(1, &verticesBO);
		glBindBuffer(GL_ARRAY_BUFFER, verticesBO);
		glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glGenBuffers(1, &uvBO);
		glBindBuffer(GL_ARRAY_BUFFER, uvBO);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
		GLuint firstTexID;
		textures.push_back(firstTexID);
		textureLoader.loadTexture(firstTexture, &textures[0]);
		isFlickering = false;
		currentTexture = 0;
		flickerSpeed = 0;
		currentDuration = 0;

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
		void FlickerModelLoader::cleanup(){
			for(unsigned int i = 0; i <textures.size(); i++){
				textureLoader.deleteTexture(&textures[i]);
			}
		}
		void FlickerModelLoader::addTexture(const char* textureName){
			GLuint TexID;
			textures.push_back(TexID);
			textureLoader.loadTexture(textureName, &textures[textures.size()-1]);
		}
		void FlickerModelLoader::addTextures(std::vector<char*> textureNames){
			for(unsigned int i = 0; i<textureNames.size(); i++){
				GLuint TexID;
				textures.push_back(TexID);
				textureLoader.loadTexture(textureNames[i], &textures[textures.size()-1]);
			}
		}
			
		void FlickerModelLoader::render(){
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
			glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);

	
			glDrawArrays(GL_TRIANGLES, 0, vertexNumber );

			glBindVertexArray(0);
		}
		void FlickerModelLoader::update(){
			if(isFlickering){
				continueFlicker();
			}
			ModelLoader::update();
		}
		void FlickerModelLoader::changeTexture(int textureNumber){
			if(textureNumber >= textures.size()){
				//std::cout<<"ERROR: FMODEL: asked to load texture outside of text range\n";
			}else{
				currentTexture = textureNumber;
			}

		}
			
		void FlickerModelLoader::continueFlicker(){
			currentDuration += gameClock.lastLoopTime()*flickerSpeed;
			if(currentDuration >= 1.0f){
				getNextTexture();
				currentDuration = 0.0f;
			}
		}

		void FlickerModelLoader::getNextTexture(){
			currentTexture = (currentTexture+1) % textures.size();
		}

		void FlickerModelLoader::startFlickering(float duration){
			isFlickering = true;
			flickerSpeed = 1.0f/duration;
		}
		void FlickerModelLoader::stopFlickering(){
			isFlickering = false;
			currentDuration = 0.0f;
		}
}
