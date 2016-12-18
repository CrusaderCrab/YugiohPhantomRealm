#include <Game\Cards\SmallCardRender.h>
#include <Game\Cards\CardData.h>
#define GLM_FORCE_RADIANS
#include <gtc\matrix_transform.hpp>
#include <Utility\TextureLoader.h>
#include <DefinesAndTypedefs.h>
#include <Utility\StateUnit.h>
#include <Game\Animation\Camera.h>
#include <Utility\TextPrinter.h>

#include <Utility\InputUnit.h>
#include <iostream>
#include <Utility\DebugUnit.h>

#define YUG_CARD_RATIO 1.3186f
#define YUG_CARD_X 0.2f
#define YUG_CARD_Y 0.26372f

#define YUG_MON_RX 0.178f
#define YUG_MON_LX -0.176f
#define YUG_MON_Y_TOP 0.24f
#define YUG_MON_Y_BOTTOM -0.110f//-0.117f
#define YUG_MON_Z 0.0001f

#define YUG_MON_TEX_X 1.0f//0.8f
#define YUG_MON_TEX_Y 1.0f//0.8f
#define YUG_MON_TEX_NX 0.0f//0.2f
#define YUG_MON_TEX_NY 0.0f//0.2f

#define YUG_STATS_SCALE_X 0.5f//0.5f
#define YUG_STATS_SCALE_Y 0.8f//0.6f

#define YUG_MAGIC_SCALE_X 0.45f
#define YUG_MAGIC_SCALE_Y 1.0f

namespace Card{

	glm::vec3 SmallCardRender::atkStartPosition;
	glm::vec3 SmallCardRender::defStartPosition;
	glm::vec3 SmallCardRender::magicStartPosition;

	GLuint SmallCardRender::cardVertsVBO;
	GLuint SmallCardRender::pictureVertsVBO;
	GLuint SmallCardRender::backCardTBO;
	GLuint SmallCardRender::monsterCardTBO;
	GLuint SmallCardRender::magicCardTBO;
	GLuint SmallCardRender::trapCardTBO;
	GLuint SmallCardRender::frontCardVAO;
	GLuint SmallCardRender::backCardVAO;
	GLuint SmallCardRender::pictureVAO;
	GLuint SmallCardRender::hiddenTBO;
	GLuint SmallCardRender::hiddenCardTBO;

	namespace{
		float cardVerts[] = {
			-YUG_CARD_X, YUG_CARD_Y, 0.0f,//TOPLEFT VERTICE
			0.0f, 0.0f,						//TEXTURE
			-YUG_CARD_X, -YUG_CARD_Y, 0.0f,//BOOTOM LEFT
			0.0f, 1.0f,						
			YUG_CARD_X, -YUG_CARD_Y, 0.0f,//BOTTOM RIGHT
			1.0f, 1.0f,						
			YUG_CARD_X, YUG_CARD_Y, 0.0f,//TOP RIGHT
			1.0f, 0.0f,		
		};
		float pictureVerts[] = {
			YUG_MON_LX, YUG_MON_Y_TOP, YUG_MON_Z,
			YUG_MON_TEX_NX, YUG_MON_TEX_NY,
			YUG_MON_LX, YUG_MON_Y_BOTTOM, YUG_MON_Z,
			YUG_MON_TEX_NX, YUG_MON_TEX_Y,
			YUG_MON_RX, YUG_MON_Y_BOTTOM, YUG_MON_Z,
			YUG_MON_TEX_X, YUG_MON_TEX_Y,
			YUG_MON_RX, YUG_MON_Y_TOP, YUG_MON_Z,
			YUG_MON_TEX_X, YUG_MON_TEX_NY,
		};
	}

	bool SmallCardRender::initialize(){
		atkStartPosition = glm::vec3(0.04f, -0.163f, 0.002f );
		defStartPosition = glm::vec3(0.04f, -0.218f, 0.002f);
		magicStartPosition = glm::vec3(-0.02f, -0.18f, 0.002f);//z=0.002f
		glBindVertexArray(YUG_UNBIND);

		glGenBuffers(1, &cardVertsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cardVertsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cardVerts), &cardVerts, GL_STATIC_DRAW);

		glGenBuffers(1, &pictureVertsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, pictureVertsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pictureVerts), &pictureVerts, GL_STATIC_DRAW);

		textureLoader.loadTexture("GameData/textures/Cards/smallMonsterCardSource.png", &monsterCardTBO);
		textureLoader.loadTexture("GameData/textures/Cards/smallMagicCardSource.png", &magicCardTBO);
		textureLoader.loadTexture("GameData/textures/Cards/smallTrapCardSource.png", &trapCardTBO);
		textureLoader.loadTexture("GameData/textures/Cards/bigBackCardSource.png", &backCardTBO);
		textureLoader.loadTexture("GameData/textures/Cards/hiddenFace.png", &hiddenTBO);
		textureLoader.loadTexture("GameData/textures/Cards/smallHiddenCardSource.png", &hiddenCardTBO);

		glBindVertexArray(YUG_UNBIND);
		glGenVertexArrays(1, &frontCardVAO);
		glBindVertexArray(frontCardVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cardVertsVBO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3) );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cardFrontIndicesIBO);
		glBindVertexArray(YUG_UNBIND);

		glGenVertexArrays(1, &backCardVAO);
		glBindVertexArray(backCardVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cardVertsVBO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3) );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cardBackIndicesIBO);
		glBindVertexArray(YUG_UNBIND);
		
		glGenVertexArrays(1, &pictureVAO);
		glBindVertexArray(pictureVAO);
		glBindBuffer(GL_ARRAY_BUFFER, pictureVertsVBO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3) );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cardFrontIndicesIBO);
		glBindVertexArray(YUG_UNBIND);



		return true;
	}

	bool SmallCardRender::shutdown(){
		glDeleteBuffers(1, &cardVertsVBO);
		glDeleteBuffers(1, &pictureVertsVBO);
		textureLoader.deleteTexture(&monsterCardTBO);
		textureLoader.deleteTexture(&magicCardTBO);
		textureLoader.deleteTexture(&trapCardTBO);
		textureLoader.deleteTexture(&hiddenTBO);
		textureLoader.deleteTexture(&hiddenCardTBO);
		glDeleteVertexArrays(1, &frontCardVAO);
		glDeleteVertexArrays(1, &backCardVAO);
		glDeleteVertexArrays(1, &pictureVAO);
		return true;
	}

	bool SmallCardRender::startup(){
		CardRender::startup();
		doRender = true;
		//loading the monster picture
		if(parentCard->pictureTBO == YUG_UNBIND){
			textureLoader.loadTexture((parentCard->renderFileName).data(), &(parentCard->pictureTBO));
		}

		rotationMatrix = stateUnit.attackFaceupUpRotation;
		return true;
	}

	bool SmallCardRender::cleanup(){
		//std::cout<<"Small card cleanup\n";
		//std::cout<<"^^^^^Small: cleanup on"<<parentCard->name.data()<<std::endl;
		CardRender::cleanup();
		return true;
	}

	void SmallCardRender::update(){
		CardRender::update();
	}

	GLuint SmallCardRender::returnCorrectFrameID(){
		if(parentCard->hidden) return hiddenCardTBO;
		if(parentCard->monMagTrap == YUG_MONSTER_CARD)
			return monsterCardTBO;
		if(parentCard->monMagTrap == YUG_TRAP_CARD )
			return trapCardTBO;
		else
			return magicCardTBO;
	}

	void SmallCardRender::render(){

		if(!doRender)
			return;
		glBindVertexArray(YUG_UNBIND);
		if(!stateUnit.isActiveShaderProgram(YUG_TEXTURE_SHADER_PROGRAM))
			stateUnit.useShaderProgram(YUG_TEXTURE_SHADER_PROGRAM);
		

		glm::mat4 projectionMat = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMat *= camera.getWorldToViewMatrix();
		glm::mat4 translateMat = glm::translate(projectionMat, position);
		glm::mat4 finalMat = translateMat * rotationMatrix;
		finalMat = glm::scale(finalMat, scale);
		glUniformMatrix4fv(stateUnit.textureMatrixUniformLocation,
			1, GL_FALSE, &finalMat[0][0]);
		glUniform1f(stateUnit.textureSamplerUniformLocation, 0.0f);
		glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);

		glBindVertexArray(frontCardVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, returnCorrectFrameID());
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(YUG_UNBIND);

		glBindVertexArray(backCardVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backCardTBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(YUG_UNBIND);
		glDisable(GL_CULL_FACE);

		glBindVertexArray(pictureVAO);
		glActiveTexture(GL_TEXTURE0);
		if(parentCard->hidden){
			glBindTexture(GL_TEXTURE_2D, hiddenTBO);
		}else{
			glBindTexture(GL_TEXTURE_2D, (parentCard->pictureTBO));
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(YUG_UNBIND);

		if(!parentCard->hidden){

			textPrinter.leftAlign = false;
			glm::vec4 oldAmtran = textPrinter.amtran;
			textPrinter.amtran = amtran;

			glm::mat4 projectionTranslateMat = glm::translate(projectionMat, position);
			if(parentCard->monMagTrap == YUG_MONSTER_CARD){
				glm::mat4 altMat = glm::translate(glm::mat4(), atkStartPosition);
				altMat = glm::scale(altMat, glm::vec3(YUG_STATS_SCALE_X,YUG_STATS_SCALE_Y,1.0f));
				textPrinter.printTextAndNumber(
					"/",parentCard->attack,YUG_TEXT_INFO_FONT,
					finalMat, altMat);

				altMat = glm::translate(glm::mat4(), defStartPosition);
				altMat = glm::scale(altMat, glm::vec3(YUG_STATS_SCALE_X,YUG_STATS_SCALE_Y,1.0f));
				textPrinter.printTextAndNumber(
					"@",parentCard->defense,YUG_TEXT_INFO_FONT,
					finalMat, altMat);
			}else if(parentCard->monMagTrap == YUG_MAGIC_CARD){
				glm::mat4 altMat = glm::translate(glm::mat4(), magicStartPosition);
				altMat = glm::scale(altMat, glm::vec3(YUG_MAGIC_SCALE_X,YUG_MAGIC_SCALE_Y,1.0f));
				textPrinter.printText("Magic", YUG_TEXT_NAME_FONT,
					finalMat,altMat);
			}else if(parentCard->monMagTrap == YUG_EQUIP_CARD){
				glm::mat4 altMat = glm::translate(glm::mat4(), magicStartPosition);
				altMat = glm::scale(altMat, glm::vec3(YUG_MAGIC_SCALE_X,YUG_MAGIC_SCALE_Y,1.0f));
				textPrinter.printText("  Equip", YUG_TEXT_NAME_FONT,
					finalMat,altMat);
			}else{
				glm::mat4 altMat = glm::translate(glm::mat4(), magicStartPosition);
				altMat = glm::scale(altMat, glm::vec3(YUG_MAGIC_SCALE_X,YUG_MAGIC_SCALE_Y,1.0f));
				textPrinter.printText("Trap", YUG_TEXT_NAME_FONT,
					finalMat,altMat);


			}
			textPrinter.amtran = oldAmtran;
			textPrinter.leftAlign = true;

		}//end hidden
	}
}
