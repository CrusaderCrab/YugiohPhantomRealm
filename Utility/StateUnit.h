#pragma once
#include <GL\glew.h>
#include <string>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
namespace Game{ class YugiohUnit; }
				class RenderUnit;
				class QApplication;
namespace Utility{

#define YUG_TEXTURE_SHADER_PROGRAM 1
#define YUG_COLOUR_SHADER_PROGRAM 2

	class StateUnit
	{
		StateUnit(){}
		StateUnit(const StateUnit&);
		StateUnit& operator=(const StateUnit&);

		bool createShaderPrograms();
		void setupRotationMatrices();
		std::string readShaderCode(std::string fileName);
		bool checkShaderCompile(GLuint vertexID);
		bool checkShaderLink(GLuint programID);
		bool locateTextureUniforms();
		bool locateColourUniforms();
		bool makePerspectiveID();
		int activeShader;
	public:
		bool initialize();
		bool shutdown();

		bool isActiveShaderProgram(int shaderDefine);
		void useShaderProgram(int shaderDefine);

		bool setDefaultPerspectiveMatrix();
		glm::vec3 relativeInterpolate(glm::vec3 position,
			glm::vec3 movement);
		glm::vec3 relativeInterpolate(glm::vec3 position,
			float x, float y, float z);

		std::vector<std::string> breakUpText(std::vector<char>text,int maxLineLength);
		const char* getConstellationName(int con);
		const char* getTypeName(int type);

		//uniform shader IDs
		GLuint textureMatrixUniformLocation;
		GLuint textureSamplerUniformLocation;
		GLuint textureAmtranUniformLocation;
		GLuint colourColourUniformLocation;
		GLuint colourMatrixUniformLocation;
		//GPU openGL shader program IDs
		GLuint textureProgramID;
		GLuint colourProgramID;
		GLuint particleProgramID;
		//camera perspective for entire game
		glm::mat4 perspectiveMatrix;

		glm::mat4 defenseFaceupBoardRotation;
		glm::mat4 attackFaceupBoardRotation;
		glm::mat4 defenseFacedownBoardRotation;
		glm::mat4 attackFacedownBoardRotation;

		glm::mat4 defenseFaceupUpRotation;
		glm::mat4 attackFaceupUpRotation;
		glm::mat4 defenseFacedownUpRotation;
		glm::mat4 attackFacedownUpRotation; 

		RenderUnit* renderer;
		Game::YugiohUnit* updater;
		QApplication* application;
		void takeAppPointer(QApplication* app);

		int freeDuelHorz;
		int freeDuelVerts;

		static StateUnit StateUnitInstance;
	
	};
#define stateUnit Utility::StateUnit::StateUnitInstance
}

