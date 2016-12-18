#include <irrKlang.h>
#include <Utility\SoundUnit.h>
#include <Utility\Clock.h>
#include <Utility\ErrorHandler.h>
using namespace irrklang;

namespace Utility{

	SoundUnit SoundUnit::soundInstance;

	bool SoundUnit::initialize(){
		musicSource = createIrrKlangDevice();
		if(!musicSource){
			errorHandler.printError("Sound Unit: irrKlang music engine failed to load.");
			return false;
		}
		soundOn = true;
		effectsOn = true;
		musicOn = true;
		musicCounting = false;
		//musicSource->play2D("GameData/sounds/music/warmupMusic.wav", true);
		return true;
	}
	bool SoundUnit::shutdown(){
		if(musicSource)
			musicSource->drop();
		return true;
	}

	void SoundUnit::update(){
		if(musicCounting){
			musicClock.newLoop();
			timeLeft -= musicClock.lastLoopTime();
			if(timeLeft<=0)
				moveToNextTrack();
		}
	}
	void SoundUnit::playLeadinTrack(const char* track, const char* followup, float length){
		if(!musicOn) return;
		musicCounting = true;
		nextTrack = followup;
		timeLeft = length;
		musicClock.newLoop();
		playOnce(track);
	}
	void SoundUnit::moveToNextTrack(){
		musicCounting = false;
		if(!musicOn) return;
		playLoop(nextTrack.c_str());
		nextTrack = "";
	}

	void SoundUnit::playOnce(const char* filename){
			musicSource->play2D(filename);
	}
	void SoundUnit::playLoop(const char* filename){
		if(soundOn && musicOn){
			musicSource->play2D(filename, true);
		}
	}
	void SoundUnit::stopAll(){
		musicSource->removeAllSoundSources();
	}
	void SoundUnit::stopThisSound(const char* filename){
		musicSource->removeSoundSource(filename);
	}

	void SoundUnit::cardSwoosh(){
		if(soundOn && effectsOn)
			soundUnit.playOnce("GameData/sounds/card/cardSwipe2.wav");
	}
	void SoundUnit::cardSwivel(){
		if(soundOn && effectsOn)
			soundUnit.playOnce("GameData/sounds/card/cardSwipe3.wav");
	}
	void SoundUnit::cursorMove(){
		if(soundOn && effectsOn)
			playOnce("GameData/sounds/menu/gameBeep.wav"); //cursorMove.wav");
	}
	void SoundUnit::cursorSelectionSuccess(){}
	void SoundUnit::cursorSelectionFail(){
		if(soundOn && effectsOn)
			playOnce("GameData/sounds/menu/cursorUnsuccess.wav");
	}

	void SoundUnit::cursorSelect(){
		if(soundOn && effectsOn)
			playOnce("GameData/sounds/menu/cursorSelect4.wav");
	}
	void SoundUnit::cursorCancel(){
		if(soundOn && effectsOn)
			playOnce("GameData/sounds/menu/cursorCancel.wav");
	}








}
