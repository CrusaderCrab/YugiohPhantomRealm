#include <Game\Animation\ParticlesUnit.h>
#include <vector>
#include <string>
#include <gtc\matrix_transform.hpp>
#include <ThirdParty\objloader.hpp>
#include <Utility\TextureLoader.h>
#include <Utility\Clock.h>
#include <Utility\StateUnit.h>
#include <Game\Animation\Camera.h>
#include <DefinesAndTypedefs.h>
#include <Game\Animation\MovementUnit.h>
#include <random>
#include <iostream>

#define NO_OF_PARTICLES 100
#define HALF_NO_OF_PARTICLES 50
#define COUNT_2_2PI 0.0628319f
#define COUNT_2_4PI 0.1256637f
#define ZYUG_BANG 0
#define ZYUG_SUCK 1
#define ZYUG_BURN 2
#define ZYUG_TRAP 3
#define ZYUG_RISE 4
#define ZYUG_CARD_HIT 5
namespace{

#include <TextFiles\randomNumbers.txt>
#include <TextFiles\risingLightData.txt>

}
namespace Animation{

	ParticlesUnit ParticlesUnit::particleUnitInstance;

	bool ParticlesUnit::initialize(){ 
		particleAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		doRender = false;
		countSpeed = 0.0f;
		currentCount = 0.0f;
		counting = false;
		doRender = false;
		doUpdate = false;
		ignoreCamera = true;
		burnupType = YUG_PARTICLE_BURN_NO_BURN;
		//init cardBlocker
		cardBlocker.startup(YUG_PLANE_FILE_PATH,"GameData/textures/models/whitescreenUV.png");
		cardBlocker.amtran = glm::vec4(0.001f,0.0f,0.0f,1.0f);
		cardBlocker.doRender = false;
		cardBlocker.scale = glm::vec3(0.17f,0.22f,1.0f);

		
		textureLoader.loadTexture("GameData/textures/particles/flame02.png",&textureBurnBO);
		textureLoader.loadTexture("GameData/textures/particles/trapflame02.png",&textureAltBurnBO);
		textureBangBO = YUG_UNBIND;
		textureSuckBO = YUG_UNBIND;
		textureTrapBO = YUG_UNBIND;

		//getting plane
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		loadOBJ(YUG_PLANE_FILE_PATH,
			(vertices),(uvs),(normals));
		vertexNumber = vertices.size();
		//pushing down to openGL
		glBindVertexArray(0);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glGenBuffers(1, &uvBO);
		glBindBuffer(GL_ARRAY_BUFFER, uvBO);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
		//setting up VAO
		glBindVertexArray(0);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBO);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glBindVertexArray(0);

		return true;
	}
	bool ParticlesUnit::shutdown(){
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &uvBO);
		glDeleteVertexArrays(1, &VAO);
		textureLoader.deleteTexture(&textureBangBO);
		textureLoader.deleteTexture(&textureSuckBO);
		textureLoader.deleteTexture(&textureBurnBO);
		textureLoader.deleteTexture(&textureAltBurnBO);
		textureLoader.deleteTexture(&textureTrapBO);

		return false;
	}
	
	void ParticlesUnit::update(){
		if(doUpdate){
			if(counting){
				continueCount();
				if(burnupType != YUG_PARTICLE_BURN_NO_BURN){
					burnUpdate();
				}
			}

		}
	}
	void ParticlesUnit::render(){
		if(doRender){
			switch(currentAnimation){
			case ZYUG_BANG:
				renderParticleBang();
				break;
			case ZYUG_SUCK:
				renderParticleSuck();
				break;
			case ZYUG_BURN:
				renderBurnup();
				break;
			case ZYUG_TRAP:
				renderTrapParticles();
				break;
			case ZYUG_RISE:
				renderParticleRise();
				break;
			case ZYUG_CARD_HIT:
				renderParticleCardHit();
				break;
			default:
				break;
			}
		}
	}
	
	void ParticlesUnit::renderParticleBang(){
		if(!doRender)
			return;
		glBindVertexArray(0);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBangBO);

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(! ignoreCamera )
			projectionMatrix *= camera.getWorldToViewMatrix();

		for(unsigned int i = 0; i < NO_OF_PARTICLES; i++){

			glm::vec3 posi = glm::vec3(centre.x + startPosi[i]*currentCount*200,
										centre.y + startPosi[i+99]*currentCount*200,
										 centre.z+(i/1000.0f));
			glm::mat4 tm = glm::translate(projectionMatrix, posi);
			tm = glm::scale(tm, glm::vec3(scales[i]*10,scales[i]*10,1.0f));

			glUniformMatrix4fv( 
				stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &tm[0][0]);
			glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);
			glm::vec4 amtran = glm::vec4(1.0f,1.0f,1.0f,sin(currentCount*3.1415));
			glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);
			
			glDrawArrays(GL_TRIANGLES, 0, vertexNumber );
			
		}
		glBindVertexArray(0);
	}

	void ParticlesUnit::renderParticleRise(){
		if(!doRender)
			return;
		int zLayering = (playerView?-1:1);
		glBindVertexArray(0);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBangBO);
		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(! ignoreCamera )
			projectionMatrix *= camera.getWorldToViewMatrix();
		float tran = sin(currentCount*3.1415f);
		glm::vec4 amtran = glm::vec4(1.0f,1.0f,1.0f,tran)*particleAmtran;
		unsigned int centreSize = risingLightCentres.size();
		for(unsigned int c = 0; c < centreSize; c++){
			for(unsigned int i = 0; i<290; i+=3){
				float lightY = (risingLights[i+1]+(currentCount*5.0f))+risingLightCentres[c].y;
				if(lightY >= -1.0f){
					glm::vec3 posi = glm::vec3(risingLightCentres[c].x + risingLights[i],
											   lightY,
											   risingLightCentres[c].z + (zLayering*(i* 0.000682f-0.098895f)));
					glm::mat4 tm = glm::translate(projectionMatrix, posi);
					tm = glm::scale(tm, glm::vec3(0.05f,0.1f,1.0f));
					glUniformMatrix4fv( 
						stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &tm[0][0]);
					glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);
					glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);
			
					glDrawArrays(GL_TRIANGLES, 0, vertexNumber );
				}
			}
		}
		glBindVertexArray(0);
	}

	void ParticlesUnit::renderParticleSuck(){
		if(!doRender)
			return;
		glBindVertexArray(0);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureSuckBO);

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(! ignoreCamera )
			projectionMatrix *= camera.getWorldToViewMatrix();

		for(unsigned int i = 0; i < NO_OF_PARTICLES; i++){

			glm::vec3 posi = glm::vec3(centre.x + startPosi[i]*(1-currentCount)*400,
										centre.y + startPosi[i+99]*(1-currentCount)*400,
										 centre.z+(i/1000.0f));
			glm::mat4 tm = glm::translate(projectionMatrix, posi);
			tm = glm::scale(tm, glm::vec3(scales[i]*10,scales[i]*10,1.0f));

			glUniformMatrix4fv( 
				stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &tm[0][0]);
			glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);
			float tran = sin(currentCount*3.1415f);
			if(tran < 0.8)
				tran += tran*0.2f;
			glm::vec4 amtran = glm::vec4(1.0f,1.0f,1.0f,tran)*particleAmtran;
			glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);
			
			glDrawArrays(GL_TRIANGLES, 0, vertexNumber );
			
		}
		glBindVertexArray(0);
	}

	void ParticlesUnit::renderParticleCardHit(){
		if(!doRender)
			return;
		glBindVertexArray(0);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBangBO);
		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(! ignoreCamera )
			projectionMatrix *= camera.getWorldToViewMatrix();		
		glm::vec4 amtran = particleAmtran;
		amtran.w = 1- currentCount;
		unsigned int centreSize = risingLightCentres.size();
		for(unsigned int c = 0; c < centreSize; c++){
			glm::vec3 cardCentre = risingLightCentres[c];
			for(unsigned int i = 0; i < HALF_NO_OF_PARTICLES; i++){
				float xsub =  0.25f*currentCount * sin(i*COUNT_2_2PI);
				float x = cardCentre.x + xsub;
				float y = cardCentre.y + 0.2f*(currentCount);
				float z = cardCentre.z;
				z += (playerView? (0.25f*currentCount * (0.5f-cos(i*0.0314f))) 
								: (0.25f*currentCount * (cos(i*0.0314f)-0.5f)));

				glm::vec3 posi = glm::vec3(x,y,z);
				glm::mat4 tm = glm::translate(projectionMatrix, posi);
				glm::vec3 scaleVec = glm::vec3(scales[i]*50,scales[i]*50,1.0f);
				tm = glm::scale(tm, scaleVec);
				glUniformMatrix4fv( 
					stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &tm[0][0]);
				glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);
				glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);		
				glDrawArrays(GL_TRIANGLES, 0, vertexNumber );


				posi.x = cardCentre.x - xsub;;
				tm = glm::translate(projectionMatrix, posi);
				tm = glm::scale(tm, scaleVec);
				glUniformMatrix4fv( 
					stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &tm[0][0]);
				glDrawArrays(GL_TRIANGLES, 0, vertexNumber );
			}
		}
		glBindVertexArray(0);
	}

	void ParticlesUnit::renderTrapParticles(){
		if(!doRender)
			return;
		glBindVertexArray(0);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureTrapBO);

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(! ignoreCamera )
			projectionMatrix *= camera.getWorldToViewMatrix();

		for(unsigned int i = 0; i < HALF_NO_OF_PARTICLES; i++){
			float x = centre.x + 0.3f*(currentCount*sin(i*COUNT_2_4PI + circleZOffset));
			float y = centre.y + 0.5f*(sin(currentCount*YUG_PI)) + i*0.00002f;
			float z = centre.z + 0.3f*(currentCount*cos(i*COUNT_2_4PI + circleZOffset));
			glm::vec3 posi = glm::vec3(x,y,z);
			glm::mat4 tm = glm::translate(projectionMatrix, posi);
			tm = tm * trapRotation;
			tm = glm::scale(tm, glm::vec3(scales[i+40]*30,scales[i+40]*30,1.0f));
			glUniformMatrix4fv( 
				stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &tm[0][0]);
			glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);
			glm::vec4 amtran = particleAmtran;
			amtran.w = sin(currentCount*YUG_PI);
			glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);
			
			glDrawArrays(GL_TRIANGLES, 0, vertexNumber );
			
		}
		for(unsigned int i = 0; i < HALF_NO_OF_PARTICLES; i++){
			float x = centre.x + 0.2f*(currentCount*sin(i*COUNT_2_4PI + circleZOffset));
			float y = centre.y + 0.8f*(sin(currentCount*YUG_PI)) + i*0.00002f;
			float z = centre.z + 0.2f*(currentCount*cos(i*COUNT_2_4PI + circleZOffset));
			glm::vec3 posi = glm::vec3(x,y,z);
			glm::mat4 tm = glm::translate(projectionMatrix, posi);
			tm = tm * trapRotation;
			tm = glm::scale(tm, glm::vec3(scales[i]*30,scales[i]*30,1.0f));
			glUniformMatrix4fv( 
				stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &tm[0][0]);
			glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);
			glm::vec4 amtran = particleAmtran;
			amtran.w = sin(currentCount*YUG_PI);
			glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);
			glDrawArrays(GL_TRIANGLES, 0, vertexNumber );

			
		}
		glBindVertexArray(0);
	}

	void ParticlesUnit::particleBang(glm::vec3 gCentre, float duration){
		centre = gCentre;
		currentAnimation = ZYUG_BANG;
		textureLoader.deleteTexture(&textureBangBO);
		textureLoader.loadTexture("GameData/textures/particles/sparkle.png",&textureBangBO);
		particleAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		ignoreCamera = true;
		startCount(duration);
		//std::cout<<"Partsicles : Started Bang!\n";
	}
		
	void ParticlesUnit::particleSuck(glm::vec3 gCentre, float duration){
		centre = gCentre;
		currentAnimation = ZYUG_SUCK;
		textureLoader.deleteTexture(&textureSuckBO);
		textureLoader.loadTexture("GameData/textures/particles/sparkle.png",&textureSuckBO);
		particleAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		ignoreCamera = true;
		startCount(duration);
		//std::cout<<"Partsicles : Started Suck!\n";
	}

	void ParticlesUnit::particleRise(std::vector<glm::vec3> centres, glm::vec4 colour, float duration, bool pplayerView){
		playerView = playerView;
		risingLightCentres = centres;
		particleAmtran = colour;
		currentAnimation = ZYUG_RISE;
		ignoreCamera = false;
		textureLoader.deleteTexture(&textureBangBO);
		textureLoader.loadTexture("GameData/textures/particles/risingorb3.png",&textureBangBO);
		startCount(duration);
	}

	void ParticlesUnit::particleCardHit(std::vector<glm::vec3> centres, glm::vec4 colour, float duration, bool pplayerView){
		risingLightCentres = centres;
		particleAmtran = colour;
		currentAnimation = ZYUG_CARD_HIT;
		ignoreCamera = false;
		playerView = pplayerView;
		if(playerView){
			zPlayerOffset = 1.0f;
			circleZOffset = YUG_PI;
			circleXOffset = -YUG_HALF_PI; 
		}else{
			zPlayerOffset = -1.0f;
			circleZOffset = -YUG_PI;
			circleXOffset = YUG_HALF_PI;
		}
		trapRotation = glm::mat4(1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1);
		textureLoader.deleteTexture(&textureBangBO);
		textureLoader.loadTexture("GameData/textures/particles/sparkle.png",&textureBangBO);
		startCount(duration);

	}

	void ParticlesUnit::trapParticles(glm::vec3 gCentre, float duration, bool player){
		centre = gCentre;
		currentAnimation = ZYUG_TRAP;
		if(player){
			circleZOffset = YUG_PI;
			circleXOffset = -YUG_HALF_PI; 
		}else{
			circleZOffset = -YUG_PI;
			circleXOffset = YUG_HALF_PI;
		}
		trapRotation = glm::mat4(1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1);
		textureLoader.deleteTexture(&textureTrapBO);
		textureLoader.loadTexture("GameData/textures/particles/sparkle.png",&textureTrapBO);
		particleAmtran = glm::vec4(0.9f,0.6f,0.9f,1.0f);
		ignoreCamera = false;
		startCount(duration);

	}

	void ParticlesUnit::burnup(int type,glm::vec3 start, float width, float height, float duration){
		burnStartPoint = start;
		burnupType = type;
		srand(1);
		currentAnimation = ZYUG_BURN;
		particleAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		ignoreCamera = true;
		burnWidth = width;
		burnHeight = height;
		startCount(duration);
		for(unsigned int i = 0; i < YUG_PARTICLE_BURN_NO; i++){
			burnLifespans[i] = (0.0f);
		}
		for(unsigned int i = 0; i < YUG_PARTICLE_BURN_NO; i++){
			burnUpdate();
		}
		for(unsigned int i = 0; i < YUG_PARTICLE_BURN_NO; i++){
			burnLifespans[i] = (i/(float)YUG_PARTICLE_BURN_NO);
		}
		float temp;
		for(unsigned int i = 0; i < YUG_PARTICLE_BURN_NO; i++){
			temp = burnLifespans[i];
			int oI = rand()%YUG_PARTICLE_BURN_NO;
			burnLifespans[i] = burnLifespans[oI];
			burnLifespans[oI] = temp;
		}

		cardBlocker.doRender = true;
		glm::vec3 cardPosi = mov.addYOffset(start, - (height*0.5f+0.015f)); 
		cardBlocker.position = mov.addZOffset(cardPosi, 0.01f);
		cardBlocker.ignoreCamera = true;

		//std::cout<<"Partsicles : Started Burn!\n";
	}

	void ParticlesUnit::continueCount(){
		currentCount += gameClock.lastLoopTime() * countSpeed;
		if(currentCount >= 1.0f){
			counting = false;
			doUpdate = false;
			doRender = false;
			burnupType = YUG_PARTICLE_BURN_NO_BURN;
		}
	}

	void ParticlesUnit::burnUpdate(){
		float time = gameClock.lastLoopTime()*countSpeed*5.0f;
		for(unsigned int i = 0; i < YUG_PARTICLE_BURN_NO; i++){
			burnLifespans[i] -= time;
			if(burnLifespans[i] <= 0.0f){
				float x = (burnStartPoint.x - burnWidth/(2.0f)) + (i*(burnWidth/YUG_PARTICLE_BURN_NO)+((rand()%10-5)*0.01f));
				float y = burnStartPoint.y + (burnHeight*currentCount*(1+i*0.001f)) + ((rand()%10-5)*0.01f);
				float z = cardBlocker.position.z + i*0.001f;
				burnPositions[i] = glm::vec3(x,y,z);
				burnLifespans[i] = 1.0f;
			}
		}
		cardBlocker.position.y += countSpeed*gameClock.lastLoopTime()*burnHeight;
	}

	void ParticlesUnit::renderBurnup(){
		if(!doRender)
			return;
		cardBlocker.render();
		glBindVertexArray(0);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBurnBO);

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(! ignoreCamera )
			projectionMatrix *= camera.getWorldToViewMatrix();
		unsigned int jump = 1;
		if(currentCount > 0.95f){
			jump = currentCount*currentCount*5;
		}
		for(unsigned int i = 0; i < YUG_PARTICLE_BURN_NO; i+=jump){

			glm::vec3 posi = burnPositions[i];
			glm::mat4 tm = glm::translate(projectionMatrix, posi);
			tm = glm::scale(tm, glm::vec3(0.05f,0.05f,1.0f));

			glUniformMatrix4fv( 
				stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &tm[0][0]);
			glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);
			//glm::vec4 amtran = glm::vec4(1.0f,1.0f,1.0f,burnLifespans[i])*particleAmtran;
			particleAmtran.w = burnLifespans[i];
			glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &particleAmtran[0]);
			glDrawArrays(GL_TRIANGLES, 0, vertexNumber );
			
		}
		glBindVertexArray(0);

	}

	void ParticlesUnit::swapFlames(){
		GLuint temp = textureBurnBO;
		textureBurnBO = textureAltBurnBO;
		textureAltBurnBO = temp;
	}

	void ParticlesUnit::startCount(float duration){
		countSpeed = 1.0f/duration;
		currentCount = 0;
		counting = true;
		doUpdate = true;
		doRender = true;
	}

	void ParticlesUnit::getTextureFromConstellation(int con){
		std::string s = "GameData/textures/particles/cons/";
		s.append(std::to_string(con));
		s.append(".png");
		textureLoader.deleteTexture(&textureSuckBO);
		textureLoader.loadTexture(s.c_str(),&textureSuckBO);
	}

}