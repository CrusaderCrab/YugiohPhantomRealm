#pragma once

namespace Game{ class YugiohUnit; }
namespace Utility{

	class UpdateUnit
	{
		Game::YugiohUnit* oldUpdater;
		Game::YugiohUnit* currentUpdater;

		UpdateUnit() {}
		UpdateUnit(const UpdateUnit&);
		UpdateUnit& operator=(const UpdateUnit&);
	public:
		bool initialize();
		bool shutdown();

		void newUpdater(Game::YugiohUnit* newUpdater);
		void swapUpdaters();
		void returnToOldUpdater();
		void setOldUpdater(Game::YugiohUnit* newUpdater);
		static UpdateUnit updateUnitInstance;
		void update();
	};
#define updateUnit Utility::UpdateUnit::updateUnitInstance
}
