#pragma once
#include <Game\ModelLoader.h>
#include <Game\WaitUnit.h>

#define YUG_BOARD_UL_HAND 0
#define YUG_BOARD_UL_HAND_BOARD 1
#define YUG_BOARD_UL_BOARD 2
#define YUG_BOARD_UL_BOARD_ATTACK 3
#define YUG_BOARD_UL_NONE 4
#define YUG_BOARD_UL_HIDE 5
#define YUG_BOARD_UL_SHOW 6

#define YUG_BOARD_UL_NO_GUARD 7
#define YUG_BOARD_UL_BAD_GUARD 8
#define YUG_BOARD_UL_GOOD_GUARD 9

namespace Duel{

	class UnderlayWidget : public Game::WaitUnit
	{
	public:
		int displayType;
		int nextType;
		Game::ModelLoader underlay1;
		Game::ModelLoader underlay2;
		Game::ModelLoader guardianWarning;
		GLuint badGuardTBO;
		GLuint goodGuardTBO;
		int renderGuard;

		void startup();
		void cleanup();
		void update();
		void render();

		void show();
		void hide();
		void hideThen(int nextMode);

		void renderHand();
		void renderHandBoard();
		void renderBoard();
		void renderAttack();

		void viewingHand();
		void viewingHandBoard();
		void viewingBoard();
		void viewingBoardAttack();


		void noGuard();
		void badGuard();
		void goodGuard();
	};


}