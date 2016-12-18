#include <Game\Cards\BigCardRender.h>
#include <Game\Cards\CardData.h>
#define GLM_FORCE_RADIANS
#include <gtc\matrix_transform.hpp>
#include <Utility\TextureLoader.h>
#include <DefinesAndTypedefs.h>
#include <Utility\StateUnit.h>
#include <Game\Animation\Camera.h>
#include <Utility\TextPrinter.h>
#include <Utility\Clock.h>

#include <Utility\InputUnit.h>
#include <iostream>
#include <Utility\DebugUnit.h>

#define YUG_CARD_RATIO 1.45333f
#define YUG_CARD_X 0.5f
#define YUG_CARD_Y 0.7266f

#define YUG_MON_X_LEFT -0.39f
#define YUG_MON_X_RIGHT 0.378f
#define YUG_MON_Y_TOP 0.457f
#define YUG_MON_Y_BOTTOM -0.305f
#define YUG_MON_Z 0.001f

#define YUG_NAME_SCALE_X 0.22f
#define YUG_NAME_SCALE_Y 1.0f

#define YUG_ELEMENT_SCALE 0.65f

#define YUG_STATS_SCALE_X 0.3f
#define YUG_STATS_SCALE_Y 1.3f

#define YUG_STAR_SCALE 0.45f
#define YUG_STAR_GAP (YUG_STAR_SCALE * 0.156f)

#define YUG_MAGIC_CHIP_SCALE_X 0.2f
#define YUG_MAGIC_CHIP_SCALE_Y 0.45f

namespace Card{

	glm::vec3 BigCardRender::nameStartPosition;
	glm::vec3 BigCardRender::attackStartPosition;
	glm::vec3 BigCardRender::defenseStartPosition;
	glm::vec3 BigCardRender::elementStartPosition;
	glm::vec3 BigCardRender::starchipsStartPosition;
	glm::vec3 BigCardRender::magicchipsStartPosition;
	GLuint BigCardRender::cardVertsVBO;
	GLuint BigCardRender::pictureVertsVBO;
	GLuint BigCardRender::backCardTBO;
	GLuint BigCardRender::monsterCardTBO;
	GLuint BigCardRender::magicCardTBO;
	GLuint BigCardRender::trapCardTBO;
	GLuint BigCardRender::frontCardVAO;
	GLuint BigCardRender::backCardVAO;
	GLuint BigCardRender::pictureVAO;

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
			YUG_MON_X_LEFT, YUG_MON_Y_TOP, YUG_MON_Z,
			0.0f, 0.0f,
			YUG_MON_X_LEFT, YUG_MON_Y_BOTTOM, YUG_MON_Z,
			0.0f, 1.0f,
			YUG_MON_X_RIGHT, YUG_MON_Y_BOTTOM, YUG_MON_Z,
			1.0f, 1.0f,
			YUG_MON_X_RIGHT, YUG_MON_Y_TOP, YUG_MON_Z,
			1.0f, 0.0f,
		};
	}

	bool BigCardRender::initialize(){
		nameStartPosition = glm::vec3(-0.425f,0.60f, YUG_MON_Z*2);
		elementStartPosition = glm::vec3(0.36f, 0.6f, YUG_MON_Z*2);
		attackStartPosition = glm::vec3(-0.23f, -0.605f, YUG_MON_Z);
		defenseStartPosition = glm::vec3(0.2f, -0.605f, YUG_MON_Z);
		starchipsStartPosition = glm::vec3(0.36f, 0.508f, YUG_MON_Z*2);
		magicchipsStartPosition= glm::vec3(0.25f, 0.51f, YUG_MON_Z*2);
		//openGL initialization
		glBindVertexArray(YUG_UNBIND);

		glGenBuffers(1, &cardVertsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cardVertsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cardVerts), &cardVerts, GL_STATIC_DRAW);

		glGenBuffers(1, &pictureVertsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, pictureVertsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pictureVerts), &pictureVerts, GL_STATIC_DRAW);

		textureLoader.loadTexture("GameData/textures/Cards/bigMonsterCardSource.png", &monsterCardTBO);
		textureLoader.loadTexture("GameData/textures/Cards/bigMagicCardSource.png", &magicCardTBO);
		textureLoader.loadTexture("GameData/textures/Cards/bigTrapCardSource.png", &trapCardTBO);
		textureLoader.loadTexture("GameData/textures/Cards/bigBackCardSource.png", &backCardTBO);


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

	bool BigCardRender::shutdown(){
		glDeleteBuffers(1, &cardVertsVBO);
		glDeleteBuffers(1, &pictureVertsVBO);
		textureLoader.deleteTexture(&monsterCardTBO);
		textureLoader.deleteTexture(&magicCardTBO);
		textureLoader.deleteTexture(&trapCardTBO);
		
		glDeleteVertexArrays(1, &frontCardVAO);
		glDeleteVertexArrays(1, &backCardVAO);
		glDeleteVertexArrays(1, &pictureVAO);
		return true;
	}
	bool BigCardRender::startup(){
		CardRender::startup();
		hasStarted = true;
		raiseStat = false;
		emphasisStat = YUG_BIG_CARD_EMPHASIS_NONE;
		if((parentCard->name).size() > 15)
			nameScaleX = 5.0f/((parentCard->name).size());
		else
			nameScaleX = 0.333f;
		displayedAttack = parentCard->attack;
		displayedDefense = parentCard->defense;
		//loading the monster picture
		if(parentCard->pictureTBO == YUG_UNBIND){
			textureLoader.loadTexture((parentCard->renderFileName).data(), &(parentCard->pictureTBO));
		}

		
		return true;
	}

	bool BigCardRender::cleanup(){
		CardRender::cleanup();
		hasStarted = false;
		return true;
	}

	void BigCardRender::update(){
		CardRender::update();
		if(raiseStat && doRender ){
			continueStatCountUp();
		}
	}


	GLuint BigCardRender::returnCorrectFrameID(){
		if(parentCard->monMagTrap == YUG_MONSTER_CARD)
			return monsterCardTBO;
		if(parentCard->monMagTrap == YUG_TRAP_CARD)
			return trapCardTBO;
		else
			return magicCardTBO;
	}

	void BigCardRender::emphasis(int value){
		emphasisStat = value;
	}


	void BigCardRender::render(){
		if(!doRender)
			return;
		glBindVertexArray(YUG_UNBIND);
		if(!stateUnit.isActiveShaderProgram(YUG_TEXTURE_SHADER_PROGRAM))
			stateUnit.useShaderProgram(YUG_TEXTURE_SHADER_PROGRAM);
		
		//calculating the ModelProjection Matrix
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
		//rendering the front frame of the card
		glBindVertexArray(frontCardVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, returnCorrectFrameID());
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(YUG_UNBIND);
		//rendering the back of the card
		glBindVertexArray(backCardVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, backCardTBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(YUG_UNBIND);
		glDisable(GL_CULL_FACE);
		//rendering the picture of the card
		glBindVertexArray(pictureVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, (parentCard->pictureTBO));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(YUG_UNBIND);

		textPrinter.leftAlign = true;
		glm::vec4 oldAmtran = textPrinter.amtran;
		textPrinter.amtran = amtran;

		//rendering the card's name
		glm::mat4  projectionTranslateMat = finalMat;
		glm::mat4 nameMatrix = glm::translate(glm::mat4(),nameStartPosition);
		nameMatrix = glm::scale(nameMatrix,glm::vec3(nameScaleX, YUG_NAME_SCALE_Y, 1.0f));
		textPrinter.printText(
			(parentCard->name).data(),//card name
			YUG_TEXT_NAME_FONT,
			finalMat,
			nameMatrix);

		textPrinter.leftAlign = false;

		//rendering the card's element
		nameMatrix = glm::translate(glm::mat4(), elementStartPosition);
		nameMatrix = glm::scale(nameMatrix,glm::vec3(YUG_ELEMENT_SCALE,YUG_ELEMENT_SCALE,1.0f));
		textPrinter.printSymbol(
			parentCard->element, 
			finalMat,
			nameMatrix);
		if(parentCard->monMagTrap == YUG_MONSTER_CARD){
			//rendering the card's atk+def
			glm::vec4 medAmtran = textPrinter.amtran;
			if(emphasisStat == YUG_BIG_CARD_EMPHASIS_DEF){
				textPrinter.amtran = glm::vec4(1.0f,1.0f,1.0f,0.75f);
			}
			nameMatrix = glm::translate(glm::mat4(),attackStartPosition);
			nameMatrix = glm::scale(nameMatrix,glm::vec3(YUG_STATS_SCALE_X, YUG_STATS_SCALE_Y,1.0f));
			textPrinter.printTextAndNumber(
				"ATK/", displayedAttack,
				YUG_TEXT_NAME_FONT, 
				finalMat, nameMatrix);
			if(emphasisStat == YUG_BIG_CARD_EMPHASIS_ATK){
				textPrinter.amtran *= glm::vec4(1.0f,1.0f,1.0f,0.75f);
			}else{
				textPrinter.amtran *= textPrinter.defaultAmtran;
			}
			nameMatrix = glm::translate(glm::mat4(),defenseStartPosition);
			nameMatrix = glm::scale(nameMatrix,glm::vec3(YUG_STATS_SCALE_X, YUG_STATS_SCALE_Y,1.0f));
			textPrinter.printTextAndNumber(
				"DEF/", displayedDefense,
				YUG_TEXT_NAME_FONT, 
				finalMat, nameMatrix);
			textPrinter.amtran = medAmtran;
			//drawing the star chips
			for(int starCount = 0; 
				starCount < parentCard->starchips; starCount++)
			{
				glm::vec3 starPlace = glm::vec3(
					starchipsStartPosition.x - (starCount*YUG_STAR_GAP),
					starchipsStartPosition.y,
					starchipsStartPosition.z);
				nameMatrix = glm::translate(glm::mat4(),starPlace);
				nameMatrix = glm::scale(nameMatrix,glm::vec3(YUG_STAR_SCALE, YUG_STAR_SCALE,1.0f));
				textPrinter.printSymbol(
					YUG_STAR, 
					finalMat, nameMatrix);
			}
		}//end monster
		else if(parentCard->monMagTrap == YUG_MAGIC_CARD || parentCard->monMagTrap == YUG_EQUIP_CARD){
			nameMatrix = glm::translate(glm::mat4(),magicchipsStartPosition);
			nameMatrix = glm::scale(nameMatrix,glm::vec3(YUG_MAGIC_CHIP_SCALE_X, YUG_MAGIC_CHIP_SCALE_Y,1.0f));
			textPrinter.printText(
			"[Magic Card]",//card name
			YUG_TEXT_NAME_FONT,
			finalMat, nameMatrix);
		}else{
			nameMatrix = glm::translate(glm::mat4(),magicchipsStartPosition);
			nameMatrix = glm::scale(nameMatrix,glm::vec3(YUG_MAGIC_CHIP_SCALE_X, YUG_MAGIC_CHIP_SCALE_Y,1.0f));
			textPrinter.printText(
			"[Trap Card]",//card name
			YUG_TEXT_NAME_FONT,
			finalMat, nameMatrix);

		}
		textPrinter.amtran = oldAmtran;
		textPrinter.leftAlign = true;
	}


	void BigCardRender::continueStatCountUp(){
		attackCountUpClock += gameClock.lastLoopTime()*attackCountUpSpeed;
		if(attackCountUpClock < 1.0f){
			displayedAttack = startAttack + (finalAttack-startAttack)*attackCountUpClock;//QWERTY
			displayedDefense = startDefense + (finalDefense-startDefense)*attackCountUpClock;
		}else{
			displayedAttack = finalAttack;
			displayedDefense = finalDefense;
			raiseStat = false;
		}
	}

	void BigCardRender::countStatUp(int amount, float duration){
		startAttack = parentCard->attack;
		startDefense = parentCard->defense;
		finalDefense = startDefense;
		finalAttack = startAttack;
		if(!(emphasisStat == YUG_BIG_CARD_EMPHASIS_ATK)){
			finalDefense = startDefense+amount;
		}
		if(!(emphasisStat == YUG_BIG_CARD_EMPHASIS_DEF)){
			finalAttack = startAttack+amount;
			//std::cout<<"final atk = "<<finalAttack<<std::endl;
		}
		attackCountUpSpeed = 1.0f/duration;
		attackCountUpClock = 0.0f;
		raiseStat = true;
	}

}
