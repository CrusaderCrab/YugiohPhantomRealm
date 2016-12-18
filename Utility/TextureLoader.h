#pragma once
#include <GL\glew.h>
#include <vector>
namespace Utility{

	class TextureLoader
	{
		TextureLoader(){}//for singleton structure
		TextureLoader(const TextureLoader&);
		TextureLoader& operator=(const TextureLoader&);
		void secondLoadTexture(const char* fileName, GLuint* textureID);
	public:
		bool initialize();
		bool shutdown();
		void loadTexture(const char* fileName, GLuint* textureID);
		void deleteTexture(GLuint* textureID);
		std::vector<GLuint> currentIds;
		GLuint badTexture;
		static TextureLoader textureLoaderInstance;//universal object
	};

}
#define textureLoader Utility::TextureLoader::textureLoaderInstance
