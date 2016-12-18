#pragma once
#include <Game\ModelLoader.h>
namespace Game{
	class Cursor
	{
		Cursor(){}
		Cursor(const Cursor&);
		Cursor& operator=(const Cursor&);
	public:
		glm::mat4 rotate180;
		bool first180;
		bool doRender;
		ModelLoader cursorModel;
		bool initialize();
		bool shutdown();
		void render();
		void update();
		void interpolateToHandPos();

		static Cursor cursorInstance;
	};
#define puzzleCursor Game::Cursor::cursorInstance
}
