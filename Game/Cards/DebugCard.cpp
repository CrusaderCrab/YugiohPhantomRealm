#include "DebugCard.h"
#include <Utility\TextureLoader.h>
#include <Utility\StateUnit.h>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <Game\Animation\Camera.h>
#include <Utility\StateUnit.h>
#include <iostream>

namespace{
	float cardVerts[] = {
		-0.5f,	0.75f,	0.0f,//top Left, vertice
		0.0f, 0.0f,			//texture co-ord

		-0.5f,	-0.75f,	0.0f,//bottom left
		0.0f,	1.0f,

		0.5f,	-0.75f,	0.0f,//bottom right
		1.0f,	1.0f,

		0.5f,	0.75f,	0.0f,//top right
		1.0f,	0.0f,
	};
	GLushort frontQuadIndices[] = {0,1,2,0,2,3};
	int quadNoofIndices = 6;
}

void DebugCard::setup(){
	glGenBuffers(1, &statsVBO);//vbo
	glBindBuffer(GL_ARRAY_BUFFER, statsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cardVerts), &cardVerts, GL_STATIC_DRAW);
	glGenBuffers(1, &statsIBO);//ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, statsIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frontQuadIndices), &frontQuadIndices, GL_STATIC_DRAW);
	statsTBO = 4564535;
	textureLoader.loadTexture("GameData/textures/cards/bigTrapCardSource.png", &statsTBO);
	//std::cout<<"debug Card: Texture ID: "<<statsTBO;
	glGenVertexArrays(1, &statsVAO);
	glBindVertexArray(statsVAO);//all futrue calls saved to this VAO
	glBindBuffer(GL_ARRAY_BUFFER, statsVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3) );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, statsIBO);
	glBindVertexArray(0);
}
void DebugCard::draw(){
	if(!stateUnit.isActiveShaderProgram(YUG_TEXTURE_SHADER_PROGRAM)){
			stateUnit.useShaderProgram(YUG_TEXTURE_SHADER_PROGRAM);
		}

	glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
	projectionMatrix *= camera.getWorldToViewMatrix();
	glm::mat4 translationMatrix = glm::translate(projectionMatrix, glm::vec3(0.0f, 0.0f, -3.0f) );
	glm::mat4 fullTransfromMatrix = glm::rotate(translationMatrix, .01f, glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 mvp = fullTransfromMatrix;
	glUniformMatrix4fv( stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glBindVertexArray(statsVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, statsTBO);
	glDrawElements(GL_TRIANGLES, quadNoofIndices, GL_UNSIGNED_SHORT, 0);

	//glDisable(GL_CULL_FACE);
	glBindVertexArray(0);

}
