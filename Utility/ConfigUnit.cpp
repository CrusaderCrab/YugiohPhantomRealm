#include <fstream>
#include <string>
#include <sstream>
#include <Utility\InputUnit.h>
#include <Utility\SoundUnit.h>
#include <Utility\ConfigUnit.h>

namespace Utility{

	void ConfigUnit::startup(){}
	void ConfigUnit::cleanup(){}

	void ConfigUnit::saveConfig(){
		std::ofstream out("GameData/saves/options.txt");
		for(int i = 0; i < YUG_NO_OF_KEYS; i++){
			out<<inputUnit.realKeyBindings[i];
			out<<"\n";
		}
		out<<soundUnit.soundOn; out<<"\n";
		out<<soundUnit.effectsOn; out<<"\n";
		out<<soundUnit.musicOn; out<<"\n";
	}

	void ConfigUnit::loadConfig(){
		std::ifstream in("GameData/saves/options.txt");
		for(int i = 0; i < YUG_NO_OF_KEYS; i++){
			in>>inputUnit.realKeyBindings[i];;
		}
		in>>soundUnit.soundOn;
		in>>soundUnit.effectsOn;
		in>>soundUnit.musicOn;
	}

}
