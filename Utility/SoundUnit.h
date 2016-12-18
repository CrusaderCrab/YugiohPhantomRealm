#pragma once
#include <string>
//#include <irrKlang.h>
namespace irrklang{ class ISoundEngine; }
namespace Utility{
	class SoundUnit
	{
		irrklang::ISoundEngine* musicSource;

		SoundUnit(){}
		SoundUnit(const SoundUnit&);
		SoundUnit& operator=(const SoundUnit&);
	public:
		bool soundOn;
		bool effectsOn;
		bool musicOn;

		bool musicCounting;
		float timeLeft;
		std::string nextTrack;

		bool initialize();
		bool shutdown();

		void update();
		void playLeadinTrack(const char* track, const char* followup, float length);
		void moveToNextTrack(); 

		void playOnce(const char* filename);
		void playLoop(const char* filename);
		//void playIntroThenLoop(const char* intro,			\\will need a partner function that is continually called to work,
		//	const char* loop, float introLengthInSeconds);		\\or use threading
		void stopAll();
		void stopThisSound(const char* filename);

		void cardSwoosh();
		void cardSwivel();
		void cursorMove();
		void fusionSelectNoise();
		void cursorSelect();
		void cursorCancel();
		void cursorSelectionSuccess();

		void cursorSelectionFail();

		static SoundUnit soundInstance;
	};
#define soundUnit Utility::SoundUnit::soundInstance
}

