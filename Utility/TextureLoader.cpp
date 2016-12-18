//#include "windows.h"
//#include "psapi.h"

#include <Utility\TextureLoader.h>//contains glew.h
#include <ThirdParty\lodepng.h>
#include <DefinesAndTypedefs.h>
#include <Utility\ErrorHandler.h>
#include <iostream>
#include <algorithm>


namespace Utility{

	//universal object
	TextureLoader TextureLoader::textureLoaderInstance;

	bool TextureLoader::initialize()
	{
		badTexture = 0;
		loadTexture("GameData/textures/models/badTexture.png", &badTexture);
		return (badTexture != 0);
	}


	bool TextureLoader::shutdown()
	{
		return false;
	}

	void  TextureLoader::loadTexture(const char* fileName, GLuint* textureID){
		glBindVertexArray(YUG_UNBIND);
		if((*textureID) != YUG_NULL_ID )
			deleteTexture(textureID);
		//get image as png
		std::vector<unsigned char> image;
		unsigned int iWidth, iHeight;
		
		unsigned int error = lodepng::decode(image, iWidth, iHeight, fileName);
		if(error != 0 ){
			errorHandler.printError("textureLoader: failed to load png: ");
			errorHandler.printError(fileName);
			errorHandler.printError("textureLoader: Going to crash now\n");
		}
		
		//send to openGL
		glGenTextures(1, textureID);

		/*auto it = find(currentIds.begin(), currentIds.end(), *textureID );
		if(it != currentIds.end()){
			std::cout<<"Texture Loader: dup ids: "<<(*it)<<std::endl;
		}else{
			std::cout<<"Texture Loader: genned id: "<<*textureID<<std::endl;
		}*/
		currentIds.push_back(*textureID);

		glBindTexture(GL_TEXTURE_2D, (*textureID));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

		image.clear();
		

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D,0);
		
	}

	void TextureLoader::deleteTexture(GLuint* textureID){
		if( textureID == &badTexture ) return;
		
		glBindVertexArray(YUG_UNBIND);
		/*auto it = find(currentIds.begin(), currentIds.end(), *textureID );
		if(it == currentIds.end()){
			std::cout<<"Texture Loader: couldn't find id in vector: "<<(*textureID)<<std::endl;
			std::cout<<"Texture loader size: "<<currentIds.size()<<std::endl;
		}else{
			std::cout<<"Texture Loader: deleted id: "<<*textureID<<std::endl;
			currentIds.erase(it);
		}*/
		//std::cout<<"texture Loader: delete ID: "<<*textureID<<std::endl;
		glDeleteTextures(1, textureID);
		glFlush(); glFinish();
		*textureID = YUG_UNBIND;

	}

}

