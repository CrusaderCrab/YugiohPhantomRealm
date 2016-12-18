#pragma once
#include <Game\Duel\GamePlayer.h>
#include <Game\AI\AIUnit.h>
namespace Duel{
	class EnemyPlayer : public GamePlayer
	{
	public:
		AI::AIUnit* ai;
		bool startup(int info);
		void input();
		void newTurn();

	};
}

