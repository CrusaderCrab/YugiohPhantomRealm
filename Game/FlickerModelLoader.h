#pragma once
#include <Game\ModelLoader.h>
#include <vector>

namespace Game{

	class FlickerModelLoader : public ModelLoader
	{
	public:
		std::vector <GLuint> textures;
		float flickerSpeed;
		float currentDuration;
		bool isFlickering;
		int currentTexture;


		void startup(const char* objFileName, const char* firstTexture);
		virtual void cleanup();
		void addTexture(const char* textureName);
		void addTextures(std::vector<char*> textureNames);

		virtual void render();
		virtual void update();
		void changeTexture(int textureNumber);

		void continueFlicker();
		virtual void getNextTexture();
		void startFlickering(float speed);
		void stopFlickering();
	};
}
