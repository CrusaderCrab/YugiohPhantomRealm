#pragma once
#include <Game\AI\AIUnit.h>
namespace AI{
	class DebugAIUnit : public AIUnit
	{
	public:
		void setUpHighLowStats();
		void calcHandMove();
		//void calcBoardMove();
		void setupHandState();
		void decideHandPath();
		void emptyBoardHandPath();
		void breakThroughHandPath();
		void bestAttackHandPath();
		void canHoldHandPath();
		void badPositonHandPath();
		void iHaveNoCardsHandPath();
		void firstTurnHandPath();
	};
}

