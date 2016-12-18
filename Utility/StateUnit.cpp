#include <Utility\StateUnit.h>//contains glew.h, glm.hpp
#include <fstream>
#include <streambuf>
#include <gtc\matrix_transform.hpp>
#include <Utility\ErrorHandler.h>
#include <DefinesAndTypedefs.h>
#include <iostream>

namespace {

	const char* conNames[] = {
		"Mercury",
		"Sun",
		"Moon",
		"Venus",	
		"Mars",
		"Jupiter",
		"Saturn",
		"Uranus",
		"Pluto",
		"Neptune",
		"Too Far!"
	};
	const char* typeNames[] = {
		"Dragon",
		"Spellcaster",
		"Zombie",
		"Warrior",
		"Beast-Warrior",
		"Beast",
		"Winged Beast",
		"Fiend",
		"Fairy",
		"Insect",
		"Dinosaur",
		"Reptile",
		"Fish",
		"Sea Dragon",
		"Machine",
		"Thunder",
		"Aqua",
		"Pyro",
		"Rock",
		"Plant",
		"Magic",
		"Boost",
		"Ritual",
		"Trap",
		"Too Far5!",
		"Too Far6!"
	};




}


namespace Utility{

	StateUnit StateUnit::StateUnitInstance;

	bool StateUnit::initialize(){
		bool successfulInit = true;
		//errorHandler.printError("State Unit: initializing");
		//errorHandler.printError("State Unit: 1");
		successfulInit &= createShaderPrograms();
		//errorHandler.printError("State Unit: 2");
		useShaderProgram(YUG_COLOUR_SHADER_PROGRAM);
		successfulInit &= locateColourUniforms();
		//errorHandler.printError("State Unit: 3");
		useShaderProgram(YUG_TEXTURE_SHADER_PROGRAM);
		//errorHandler.printError("State Unit: 4");
		successfulInit &= locateTextureUniforms();
		//errorHandler.printError("State Unit: 5");
		successfulInit &= setDefaultPerspectiveMatrix();
		//errorHandler.printError("State Unit: 6");
		setupRotationMatrices();
		freeDuelHorz=0;
		freeDuelVerts=0;
		return successfulInit;
	}

	void StateUnit::setupRotationMatrices(){
		defenseFaceupBoardRotation = glm::mat4(0,0,1,0, 1,0,0,0, 0,1,0,0, 0,0,0,1); 
		attackFaceupBoardRotation = glm::mat4(1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1); 
		defenseFacedownBoardRotation = glm::mat4(0,0,-1,0, 1,0,0,0, 0,-1,0,0, 0,0,0,1); 
		attackFacedownBoardRotation = glm::mat4(-1,0,0,0, 0,0,-1,0, 0,-1,0,0, 0,0,0,1); 
		
		defenseFaceupUpRotation = glm::mat4(0,-1,0,0, 1,0,0,0, 0,0,1,0, 0,0,0,1); 
		attackFaceupUpRotation = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1); 
		defenseFacedownUpRotation = glm::mat4(0,1,0,0, 1,0,0,0, 0,0,-1,0, 0,0,0,1); 
		attackFacedownUpRotation = glm::mat4(-1,0,0,0, 0,1,0,0, 0,0,-1,0, 0,0,0,1); 

	}

	bool StateUnit::shutdown(){
		//delete Shader Program()?
		return true;
	}

	bool StateUnit::isActiveShaderProgram(int shaderDefine){
		return shaderDefine == activeShader;
	}
	void StateUnit::useShaderProgram(int programID){
		if(programID == YUG_TEXTURE_SHADER_PROGRAM)
			glUseProgram(textureProgramID);
		if(programID == YUG_COLOUR_SHADER_PROGRAM)
			glUseProgram(colourProgramID);
	}

	bool StateUnit::createShaderPrograms(){
		bool success = true;
		
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* adapter[1];
		//texture Shader Program
		std::string shaderCode = 
			readShaderCode(
			"GameData/Shaders/textureVertexShader.glsl");
		adapter[0] = shaderCode.c_str();
		glShaderSource(vertexShaderID, 1, adapter, 0);

		shaderCode = 
			readShaderCode("GameData/Shaders/textureFragmentShader.glsl");
		adapter[0] = shaderCode.c_str();
		glShaderSource(fragmentShaderID, 1, adapter, 0);

		glCompileShader(vertexShaderID);
		success &= checkShaderCompile(vertexShaderID);
		glCompileShader(fragmentShaderID);
		success &= checkShaderCompile(fragmentShaderID);

		textureProgramID = glCreateProgram();
		glAttachShader(textureProgramID, vertexShaderID);
		glAttachShader(textureProgramID, fragmentShaderID);
		glLinkProgram(textureProgramID);

		success &= checkShaderLink(textureProgramID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		//colour shader Program
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		shaderCode = 
			readShaderCode("GameData/Shaders/colourVertexShader.glsl");
		adapter[0] = shaderCode.c_str();
		glShaderSource(vertexShaderID, 1, adapter, 0);

		shaderCode = 
			readShaderCode("GameData/Shaders/colourFragmentShader.glsl");
		adapter[0] = shaderCode.c_str();
		glShaderSource(fragmentShaderID, 1, adapter, 0);

		glCompileShader(vertexShaderID);
		checkShaderCompile(vertexShaderID);
		glCompileShader(fragmentShaderID);
		checkShaderCompile(fragmentShaderID);

		colourProgramID = glCreateProgram();
		glAttachShader(colourProgramID, vertexShaderID);
		glAttachShader(colourProgramID, fragmentShaderID);
		glLinkProgram(colourProgramID);

		success &= checkShaderLink(colourProgramID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return success;

	}

	std::string StateUnit::readShaderCode(std::string fileName){
		std::ifstream input;
		input.open(fileName);
		if(!input.good()){
			errorHandler.printError("Shader File failed to load.\r\n");
			errorHandler.printError(fileName.c_str());
			return NULL;
		}
		return std::string(
			std::istreambuf_iterator<char>(input),
			std::istreambuf_iterator<char>()
		);
	}
	bool StateUnit::checkShaderCompile(GLuint shaderID){
		GLint compileStatus;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
		if(compileStatus != GL_TRUE){
			GLint infoLogLength;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* buffer = new GLchar[infoLogLength];
			GLsizei neededParameter;
			glGetShaderInfoLog(shaderID, infoLogLength, &neededParameter, buffer);
			errorHandler.printError(buffer);
			delete [] buffer;
			return false;
		}
		return true;
	}
	bool StateUnit::checkShaderLink(GLuint programID){
		GLint linkStatus = GL_TRUE;
		glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
		if(linkStatus != GL_TRUE){
			GLint infoLogLength;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* buffer = new GLchar[infoLogLength];
			GLsizei neededParameter;
			glGetProgramInfoLog(programID, infoLogLength, &neededParameter, buffer);
			errorHandler.printError(buffer);
			delete [] buffer;
			return false;
		}
		return true;
	}
	bool StateUnit::locateTextureUniforms(){
		textureMatrixUniformLocation = glGetUniformLocation(textureProgramID, "modelProjectionMatrix");
		textureSamplerUniformLocation = glGetUniformLocation(textureProgramID, "gSampler");
		textureAmtranUniformLocation = glGetUniformLocation(textureProgramID, "amtran");
		return true;
	}
	bool StateUnit::locateColourUniforms(){
		colourMatrixUniformLocation =  glGetUniformLocation(colourProgramID, "modelProjectionMatrix");
		colourColourUniformLocation =  glGetUniformLocation(colourProgramID, "inputColour");
		return true;
	}
	bool StateUnit::setDefaultPerspectiveMatrix(){
		perspectiveMatrix = glm::perspective(YUG_PI/5.0f, YUG_SCREEN_RATIO, DEFAULT_NEAR_PLANE, DEFAULT_FAR_PLANE);
		return true;
	}
	void StateUnit::takeAppPointer(QApplication* app){
		application = app;
	}

	glm::vec3 StateUnit::relativeInterpolate(glm::vec3 position,
			glm::vec3 movement){
				return position + movement;
	}
	glm::vec3 StateUnit::relativeInterpolate(glm::vec3 position,
		float x, float y, float z){
			return position + glm::vec3(x,y,z);
	}

	int countNextWord(std::vector<char>text, int startingPoint){
		int count = 0;
		for(int i = startingPoint; i <text.size(); i++){
			if(text[i] == ' ')
				return count;
			count++;
		}
		return count;
	}

	int spaceLeft(std::string s, int maxLineLength){
		return maxLineLength - s.size();
	}

	std::vector<std::string> StateUnit::breakUpText(std::vector<char>text,int maxLineLength)
	{
		//std::cout<<"SU: breaking text up\n";
		std::vector<std::string> brokenText;
		brokenText.push_back(std::string() );
		int index = 0;
		while(index < text.size()){
			int nextLength = countNextWord(text, index);
			if(nextLength >= maxLineLength){
				maxLineLength = nextLength+1;
			}
			if(nextLength < spaceLeft(brokenText.back(), maxLineLength)){
				for(int j = 0; j <nextLength && index < text.size(); j++){
					brokenText.back().push_back(text[index++]);
				}
				if(spaceLeft(brokenText.back(), maxLineLength) >= 1 && index < text.size()){
					brokenText.back().push_back(text[index]);
				}
				index++;
			}//room for word
			else
			{
				brokenText.push_back(std::string());
			}
		}
		return brokenText;
	}


	const char* StateUnit::getConstellationName(int con){
		return conNames[con-YUG_MERCURY];
	}
	const char* StateUnit::getTypeName(int type){
		return typeNames[type - YUG_DRAGON];
	}

}