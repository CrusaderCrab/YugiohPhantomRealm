#pragma once
#include <GL\glew.h>

#include <Game\YugiohUnit.h>

class DebugCard : public Game::YugiohUnit
{
public:

	GLuint statsVBO;
	GLuint statsIBO;
	GLuint statsTBO;
	GLuint statsVAO;

	void setup();

	void draw();
};

