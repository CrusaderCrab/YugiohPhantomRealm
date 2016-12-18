#include <Game\Animation\FadeUnit.h>
#include <Game\VectorUnit.h>
#include <Utility\Clock.h>
#include <DefinesAndTypedefs.h>

namespace Animation{

	FadeUnit FadeUnit::FadeUnitInstance;

	bool FadeUnit::initialize(){
		sheet.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/models/whitescreenUV.png");
		currentZ = pos.nc_z[0];
		sheet.position = glm::vec3(0.0f,0.0f,currentZ);
		sheet.scale = glm::vec3(3.0f, 3.0f, 1.0f);
		sheet.amtran = YUG_FADE_CLEAR;
		sheet.ignoreCamera = true;
		renderBlock = false;
		fading = false;
		currentDuration = 0.0f;
		doRender = false;
		return true;
	}
	bool FadeUnit::shutdown(){
		sheet.cleanup();
		return true;
	}

	void FadeUnit::update(){
		if(fading)
			continueFading();
	}
	void FadeUnit::render(int location){
		if(doRender){
			if(!(renderBlock && location == YUG_FADE_GLOBAL))
				sheet.render();
		}
	}

	void FadeUnit::fadeTo(glm::vec4 newAmtran, float duration){
		doRender = true;
		fading = true;
		currentDuration = 0.0f;
		fadeSpeed = 1.0f/duration;

		pastAmtran = sheet.amtran;
		destinationAmtran = newAmtran;
	}
	
	void FadeUnit::continueFading(){
		currentDuration += fadeSpeed * gameClock.lastLoopTime();
		if(currentDuration < 1.0f){
			sheet.amtran = glm::mix(
				pastAmtran,
				destinationAmtran,
				currentDuration);
		}else{
			sheet.amtran = destinationAmtran;
			fading = false;
			if(destinationAmtran == YUG_FADE_CLEAR)
				doRender = false;
		}
	}

	void FadeUnit::changeZ(float newZ){
		currentZ = newZ;
		sheet.position = glm::vec3(0.0f,0.0f,currentZ);
	}

}