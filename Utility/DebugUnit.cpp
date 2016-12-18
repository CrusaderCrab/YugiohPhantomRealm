#include <iostream>
#include <Utility\DebugUnit.h>

namespace Utility{

	DebugUnit DebugUnit::debugUnitInstance;

	bool DebugUnit::initialize(){
		debugCard.setup();
		return true;
	}

	bool DebugUnit::shutdown(){
		
		return true;
	}

	void DebugUnit::printMat4(glm::mat4 mat)
	{
		/*for( int i = 0; i <4; i++){
			for(int j = 0; j <4; j++){
				std::cout<<mat[j][i]<<"\t";
			}
			std::cout<<"\n";
		}*/
	}


	void DebugUnit::printVec3(glm::vec3 vec)
	{
		//std::cout<<vec.x<<" "<<vec.y<<" "<<vec.z<<std::endl;
	}

}