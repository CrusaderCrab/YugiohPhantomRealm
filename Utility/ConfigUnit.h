#pragma once

namespace Utility{

	class ConfigUnit
	{
	public:
		void startup();
		void cleanup();
		void loadConfig();
		void saveConfig();
	};

}