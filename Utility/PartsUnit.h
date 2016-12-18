#pragma once
//#include <MainMenu>
//#include <Shop>
//#include <board>
//#include <map>
//#include <cutscene>
//#include <config window>
//#include <startScreen>
//#include <blankUnit>//for no input times, blank render

namespace Utility{
	class PartsUnit//use to init all the parts above, so ready whenever called
	{
	public:
		bool initialize();
		bool shutdown();

		static PartsUnit partsUnitInstance;

	};
#define partsUnit Utility::PartsUnit::partsUnitInstance
}

