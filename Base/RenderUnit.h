#pragma once
#include <GL\glew.h>
#include <QtOpenGL\qglwidget>
#include <Game\YugiohUnit.h>
class RenderUnit : public QGLWidget 
{
protected:
	void initializeGL();
	void paintGL();
	void paintEvent();
public:
	bool initialize();
	bool shutdown();
	void newRenderer(Game::YugiohUnit* newRenderer);
	void swapBackToOldRenderer();
	void returnToOldRenderer();
	void setOldRenderer(Game::YugiohUnit* newRenderer);
	void render();
	Game::YugiohUnit* currentRenderer;
	Game::YugiohUnit* oldRenderer;
};

