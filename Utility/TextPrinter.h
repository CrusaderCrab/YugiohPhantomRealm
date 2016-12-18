#pragma once
#include <GL\glew.h>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <string>

#define YUG_TEXT_NAME_FONT 1
#define YUG_TEXT_INFO_FONT 0


namespace Utility{

	class TextPrinter
	{
		GLuint textInfoTextureID;
		GLuint textNameTextureID;
		GLuint textSymbolTextureID;
		GLuint textDamTextureID;
		GLuint textMagTextureID;
		GLuint textVertexID;
		GLuint textUVID;
		GLuint textVAO;
		int currentFont;
		bool onInfoFont();
		bool onNameFont();

		GLuint getCorrectTexture();
		int getLetterNumber(char);
		float getLetterWidth(char letter);
		std::vector<glm::vec2> createNameUVArray(const char* sentence, int sentenceLength);
		std::vector<glm::vec2> createInfoUVArray(const char* sentence, int sentenceLength);
		std::vector<glm::vec3> makeInfoVertices(int sentenceLength);
		std::vector<glm::vec3> makeNameVertices(int sentenceLength, const char* sentence,int charLength);
		void addNumInfoVertices(std::vector<glm::vec3>*,int,int);
		void addNumNameVertices(std::vector<glm::vec3>*,int,int);
		void addNumVertices(std::vector<glm::vec3>*,int,int);

		std::vector<glm::vec3> makeVertices(int sentenceLength, const char* sentence, int charLength);
		std::vector<glm::vec2> makeAlphabetUVs(
			const char* sentence, int font,int sentenceLength);
		std::vector<glm::vec3> makeSymbolVertex();
		std::vector<glm::vec2> makeSymbolUV(int symbol);
		void setupGLState(
			const std::vector<glm::vec3>* vertices, 
			const std::vector<glm::vec2> *uvArray, 
			GLuint textureID);
		void addNumberToUVs(std::vector<glm::vec2>* uvArray, int number, int font);
		void addInfoNumberToUVs(std::vector<glm::vec2>* uvArray, int number);
		void addNameNumberToUVs(std::vector<glm::vec2>* uvArray, int number);
		unsigned int findNoOfDigits(int number);

		TextPrinter(){}
		TextPrinter(const TextPrinter&);
		TextPrinter& operator=(const TextPrinter&);

	public:
		bool initialize();
		bool shutdown();
		bool ignoreCamera;
		bool leftAlign;
		glm::vec4 amtran;
		glm::vec4 defaultAmtran;
		glm::vec4 magicAmtran;

		void printText(const char* sentence, int font,
			float scaleX, float scaleY, 
			float posX, float posY, float posZ,
			glm::vec3 rotationLine, float angle);
		void printText(const char* sentence, int font,
			float scaleX, float scaleY, 
			float posX, float posY, float posZ,
			glm::mat4 rotationMatrix);
		void printText(const char* sentence, int font,
			float scaleX, float scaleY, 
			glm::mat4 projectionWorldMatrix);
		void printText(const char* sentence, int font,
			float scaleX, float scaleY,
			glm::mat4 projectionTranslateMatrix,
			glm::vec3 furtherTranslate,
			glm::mat4 rotationMatrix);

		void printText(const char* sentence, int font,
			glm::mat4 parentsMatrix,
			glm::mat4 furtherMatrix);
		void printText(const char* sentence, int font,
			glm::vec3 scale, glm::vec3 posi, glm::mat4 rot);

		void printSymbol(int symbol,
			float scaleX, float scaleY, 
			float posX, float posY, float posZ,
			glm::vec3 rotationLine, float angle);

		void printSymbol(int symbol,
			float scaleX, float scaleY,
			glm::mat4 projectionTranslateMatrix,
			glm::vec3 furtherTranslate,
			glm::mat4 rotationMatrix);

		void printSymbol(int symbol,
			glm::mat4 parentsMatrix,
			glm::mat4 furtherMatrix);
		void printSymbol(int symbol,
			glm::vec3 scale, glm::vec3 posi, glm::mat4 rot);

		void printTextAndNumber(
			const char* sentence, int number,
			int font,float scaleX, float scaleY,
			glm::mat4 projectionTranslateMatrix,
			glm::vec3 furtherTranslate,
			glm::mat4 rotationMatrix);
		void printTextAndNumber(const char* sentence, int number,
			int font,
			float scaleX, float scaleY, 
			float posX, float posY, float posZ,
			glm::mat4 rotationMatrix);

		void printTextAndNumber(const char* sentence, 
			int number, int font,
			glm::mat4 parentsMatrix,
			glm::mat4 furtherMatrix);

		void printNumber(int number,
			int font,float scaleX, float scaleY,
			float posX, float posY, float posZ,
			glm::mat4 rotationMatrix);

		void printNumber(int number,
			int font, glm::vec3 scale, 
			glm::vec3 posi, glm::mat4 rot);

		void printDamageNumber(int number,
			glm::vec3 scale, glm::vec3 posi);
		std::vector<glm::vec2> makeDamageUVs(
			int number, int digits);

		void printMagicNumber(int number,
			glm::vec3 scale, glm::vec3 posi);
		std::vector<glm::vec2> makeMagicUVs(
			int number, int digits);


		static TextPrinter textPrinterInstance;
	};
#define textPrinter Utility::TextPrinter::textPrinterInstance

}
