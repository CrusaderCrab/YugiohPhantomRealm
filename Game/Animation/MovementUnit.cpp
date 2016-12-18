#include <Game\Animation\MovementUnit.h>
#include <gtc\matrix_transform.hpp>
#include <gtx\rotate_vector.hpp>
#include <Utility\DebugUnit.h>

namespace Animation{

	MovementUnit MovementUnit::MovementUnitInstance;

	bool MovementUnit::initialize(){
		return true;
	}

	bool MovementUnit::shutdown(){
		return true;
	}
	void MovementUnit::update(){

	}

	glm::vec3 MovementUnit::rotateAround(glm::vec3 centre, float distance, float angle){
		glm::vec3 posi = glm::vec3(distance,0.0f,0.0f);
		posi = posi * glm::rotate(posi,angle,glm::vec3(0,0,1));
		posi = glm::vec3(posi.x+centre.x,posi.y+centre.y,posi.z+centre.z);
		return posi;
	}
	glm::vec3 MovementUnit::rotateAroundXZ(glm::vec3 centre, glm::vec3 rotateLine, float distance, float angle){
		glm::vec3 posi = glm::vec3(-rotateLine.z, rotateLine.y,rotateLine.x);
		posi *= distance;
		posi = glm::rotate(posi, angle, rotateLine);
		posi += centre;
		return posi;
	}

	glm::vec3 MovementUnit::rotateVectorX(glm::vec3 startV, float angle){
		return startV * glm::mat3(1,0,0, 
								  0,cos(angle),-sin(angle),
								  0,sin(angle),cos(angle)) ;
	}

	glm::vec3 MovementUnit::rotateVectorZ(glm::vec3 startV, float angle){
		return glm::rotate(startV, angle, glm::vec3(0,0,1));
	}

	glm::vec3 MovementUnit::addOffset(glm::vec3 origin, glm::vec3 offset){
		return origin + offset;

	}

	glm::vec3 MovementUnit::addXOffset(glm::vec3 origin, float x){
		return glm::vec3(origin.x+x,	origin.y,	origin.z);
	}
	glm::vec3 MovementUnit::addYOffset(glm::vec3 origin, float y){
		return glm::vec3(origin.x,	origin.y+y,	origin.z);
	}
	glm::vec3 MovementUnit::addZOffset(glm::vec3 origin, float z){
		return glm::vec3(origin.x,	origin.y,	origin.z+z);
	}

	void MovementUnit::flipBig(Card::CardData* card, float duration){

	}
	void MovementUnit::flipSmall(Card::CardData* card, float duration){

	}



}