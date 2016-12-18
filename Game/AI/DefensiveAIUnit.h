#pragma once
#include <Game\AI\AIUnit.h>

namespace AI{
	class DefensiveAIUnit : public AIUnit
	{
	public:
		DefensiveAIUnit(int ability, int low, int high, int danger, int disNum);
		void calcHandMove();
		void setupHandState();
		void decideHandPath();
		void emptyBoardHandPath();
		void breakThroughHandPath();
		void bestAttackHandPath();
		void canHoldHandPath();
		void badPositonHandPath();
		void iHaveNoCardsHandPath();
		void firstTurnHandPath();

		bool defenseOnlyMon(int myCol);
		void fieldRecursion();

	};
}

