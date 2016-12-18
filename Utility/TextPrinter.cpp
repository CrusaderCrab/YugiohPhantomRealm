#include <Utility\TextPrinter.h>//contains glew.h, glm.hpp
#include <vector>
#include <gtc\matrix_transform.hpp>
#include <Game\Animation\Camera.h>
#include <Utility\StateUnit.h>
#include <Utility\TextureLoader.h>
#include <Utility\ErrorHandler.h>
#include <DefinesAndTypedefs.h>
#include <string>
#include <Utility\InputUnit.h>
#include <Utility\DebugUnit.h>
#include <iostream>

#include <Utility\textOffsets.inl>

#define YUG_TEXT_VERT_X 0.075f
#define YUG_TEXT_VERT_Y 0.05f
#define YUG_TEXT_VERTS_PER_GLYPH 6
//alphabet
#define YUG_TEXT_LOWER_LETTER_OFFSET 26

#define YUG_INFOTEXT_GLYPH_WIDTH 0.0072727272f //0.007368421f//28
#define YUG_INFOTEXT_GLYPH_SPACE 0.013506493f//0.01368421f//52
#define YUG_INFOTEXT_GLYPH_HEIGHT 1.0f

//symbols
#define YUG_TEXT_SYM_GLYPH_WIDTH     0.01666f//0.014285f
#define YUG_TEXT_SYM_GLYPH_HEIGHT   1.0f
#define YUG_TEXT_SYM_GLYPH_SPACE 1.0f
#define YUG_TEXT_SYM_GLYPH_OFFSET 0.075f

//damage
#define YUG_TEXT_DAM_WIDTH	0.09090909f
#define YUG_TEXT_DAM_HEIGHT 1.0f

#define YUG_TEXT_MAG_WIDTH 0.0769231f
#define YUG_TEXT_MAG_HEIGHT 1.0f

namespace Utility{

	TextPrinter TextPrinter::textPrinterInstance;

	bool TextPrinter::initialize(){
		amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		defaultAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		magicAmtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		leftAlign = true;
		ignoreCamera = false;
		//marking as empty
		textInfoTextureID = YUG_NULL_ID;
		textNameTextureID = YUG_NULL_ID;
		textSymbolTextureID = YUG_NULL_ID;
		//getting textures
		textureLoader.loadTexture("GameData/textures/text/NewNameTextSource03.png", &textNameTextureID);//nameFontSource.png", &textNameTextureID);
		textureLoader.loadTexture("GameData/textures/text/infoFontSource3.png", &textInfoTextureID);
		textureLoader.loadTexture("GameData/textures/text/newsymbolSource02.png", &textSymbolTextureID); //textSymbolTexture.png", &textSymbolTextureID);
		textureLoader.loadTexture("GameData/textures/text/damageFontSource.png", &textDamTextureID);
		textureLoader.loadTexture("GameData/textures/text/magicFontSource.png", &textMagTextureID);
		//making buffers for vertex, uv
		glGenBuffers(1, &textVertexID);
		glGenBuffers(1, &textUVID);
		
		//making VAO
		glBindVertexArray(YUG_UNBIND);
		glGenVertexArrays(1, &textVAO);
		glBindVertexArray(textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textVertexID);
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, textVertexID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, textUVID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
		glBindVertexArray(YUG_UNBIND);
		return true;
	}
	bool TextPrinter::shutdown(){
		glDeleteBuffers(1, &textVertexID);
		glDeleteBuffers(1, &textUVID);
		glDeleteVertexArrays(1, &textVAO);
		textureLoader.deleteTexture(&textNameTextureID);
		textureLoader.deleteTexture(&textInfoTextureID);
		textureLoader.deleteTexture(&textSymbolTextureID);
		textureLoader.deleteTexture(&textDamTextureID);
		textureLoader.deleteTexture(&textMagTextureID);
		return true;
	}

	GLuint TextPrinter::getCorrectTexture(){
		if(onNameFont())
			return textNameTextureID;
		return textInfoTextureID;
	}

	int TextPrinter::getLetterNumber(char glyph){
		if('A' <= glyph && glyph <= 'Z'){
				return glyph - 'A';
			}
			//lower letters
			else if('a' <= glyph && glyph <= 'z'){
				return YUG_TEXT_LOWER_LETTER_OFFSET + (glyph - 'a');
			}
			else if('0'  <= glyph && glyph <= '9'){
				return  (2*YUG_TEXT_LOWER_LETTER_OFFSET)
					+(glyph - '0');
			}
			else
			{
				switch(glyph){
				case '-': return  63;
				case ',': return  64;
				case '.': return  65;
				case '?': return  66;
				case '!': return  67;
				case '[': return  68;
				case ']': return  69;
				case '#': return  70;
				case '/': return 71;
				case '@': return 72;
				case ' ': return 73;
				case '\'': return 68;
				default:
					//errorHandler.printError("TextPrinter.printText, unknown letter wanted.");
					return 0;
			}//end switch
		}
	}

	float TextPrinter::getLetterWidth(char letter){
		if(!onNameFont())
			return 1;
		else
			return 100*textWidths[getLetterNumber(letter)];
	}

	std::vector<glm::vec3> TextPrinter::makeInfoVertices(int sentenceLength){
		float centre = 0;
		if(!leftAlign)
			centre = (sentenceLength*YUG_TEXT_VERT_X);
		//making vert array
		std::vector<glm::vec3>vertices(
			YUG_TEXT_VERTS_PER_GLYPH*sentenceLength);
		for( int i = 0; 
			i < YUG_TEXT_VERTS_PER_GLYPH*sentenceLength; 
			i+=YUG_TEXT_VERTS_PER_GLYPH)
		{//first triangle
			float leftMovement = (i/((float)YUG_TEXT_VERTS_PER_GLYPH))*0.15f;//so letters don't overlap
			vertices[i+0] = glm::vec3(((YUG_TEXT_VERT_X)+leftMovement)-centre, YUG_TEXT_VERT_Y, 0.0f);
			vertices[i+1] = glm::vec3(((YUG_TEXT_VERT_X)+leftMovement)-centre, -YUG_TEXT_VERT_Y, 0.0f);
			vertices[i+2] = glm::vec3(((-YUG_TEXT_VERT_X)+leftMovement)-centre, -YUG_TEXT_VERT_Y, 0.0f);
		//second triangle
			vertices[i+3] = glm::vec3(((YUG_TEXT_VERT_X)+leftMovement)-centre, YUG_TEXT_VERT_Y, 0.0f);
			vertices[i+4] = glm::vec3(((-YUG_TEXT_VERT_X)+leftMovement)-centre, -YUG_TEXT_VERT_Y, 0.0f);
			vertices[i+5] = glm::vec3(((-YUG_TEXT_VERT_X)+leftMovement)-centre, YUG_TEXT_VERT_Y, 0.0f);
		}

		return vertices;
	}

	std::vector<glm::vec3> TextPrinter::makeNameVertices(
		int sentenceLength, const char* sentence, int charLength)
	{
		float letterWidth;
		float lastLetterWidth;
		float leftMovement = 0;
		float centre = 0;
		if(!leftAlign){
			centre = (sentenceLength*YUG_TEXT_VERT_X);
		}
		//making vert array
		std::vector<glm::vec3>vertices(
			YUG_TEXT_VERTS_PER_GLYPH*sentenceLength);
		for( int i = 0; 
			i < YUG_TEXT_VERTS_PER_GLYPH*sentenceLength; 
			i+=YUG_TEXT_VERTS_PER_GLYPH)
		{//calcs
			lastLetterWidth = 100 * 0.00766f;
			letterWidth = 100 * 0.00766f;
			if( (i/6) < sentenceLength){
				letterWidth = getLetterWidth( sentence[(i/6)] );
			}
			if( ((i/6)-1) >= 0 &&  ((i/6)-1) < sentenceLength ){
					lastLetterWidth = getLetterWidth( sentence[(i/6)-1] );
			}
			//first triangle						//YUG_TEXT_VERT_X = 0.075f
			leftMovement += (lastLetterWidth*0.075f + letterWidth*0.075f);//so letters don't overlap
			vertices[i+0] = glm::vec3(((letterWidth*YUG_TEXT_VERT_X)+leftMovement)-centre, YUG_TEXT_VERT_Y, 0.0f);
			vertices[i+1] = glm::vec3(((letterWidth*YUG_TEXT_VERT_X)+leftMovement)-centre, -YUG_TEXT_VERT_Y, 0.0f);
			vertices[i+2] = glm::vec3(((letterWidth*-YUG_TEXT_VERT_X)+leftMovement)-centre, -YUG_TEXT_VERT_Y, 0.0f);
		//second triangle
			vertices[i+3] = glm::vec3(((letterWidth*YUG_TEXT_VERT_X)+leftMovement)-centre, YUG_TEXT_VERT_Y, 0.0f);
			vertices[i+4] = glm::vec3(((letterWidth*-YUG_TEXT_VERT_X)+leftMovement)-centre, -YUG_TEXT_VERT_Y, 0.0f);
			vertices[i+5] = glm::vec3(((letterWidth*-YUG_TEXT_VERT_X)+leftMovement)-centre, YUG_TEXT_VERT_Y, 0.0f);
		}
		return vertices;
	}

	std::vector<glm::vec3> TextPrinter::makeVertices(int sentenceLength, const char* sentence, int charLength){
		if(onNameFont()){
			return makeNameVertices(sentenceLength, sentence, charLength);
		}
		else{
			return makeInfoVertices(sentenceLength);
		}
	}

	std::vector<glm::vec2> TextPrinter::createNameUVArray(const char* sentence, int sentenceLength){
		//making uv texture array
		std::vector<glm::vec2>uvArray(
			YUG_TEXT_VERTS_PER_GLYPH*sentenceLength);
		//possible x and y values for this letter in uv
		float x1;
		float x2;
		float y1;
		float y2;
		int columnOffset = 0;
		for(int i = 0;
			i < sentenceLength * YUG_TEXT_VERTS_PER_GLYPH;
			i += YUG_TEXT_VERTS_PER_GLYPH)
		{
			char glyph = sentence[i/YUG_TEXT_VERTS_PER_GLYPH];
			int lNumber = getLetterNumber(glyph);
			x1 = textStartPoints[lNumber];
			x2 = x1 + textWidths[lNumber];
			y1 = 0;
			y2 = YUG_INFOTEXT_GLYPH_HEIGHT;
			//putting uv co-ords into uv array
			uvArray[i+0] = glm::vec2(x2, y1);
			uvArray[i+1] = glm::vec2(x2, y2);
			uvArray[i+2] = glm::vec2(x1, y2);
			uvArray[i+3] = glm::vec2(x2, y1);
			uvArray[i+4] = glm::vec2(x1, y2);
			uvArray[i+5] = glm::vec2(x1, y1);
		}//end uv for loop
		return uvArray;
	}

	std::vector<glm::vec2> TextPrinter::makeAlphabetUVs(
		const char* sentence, int font, int sentenceLength){

			if(onNameFont())
				return createNameUVArray(sentence, sentenceLength);
			else
				return createInfoUVArray(sentence, sentenceLength);
	}
	std::vector<glm::vec2> TextPrinter::createInfoUVArray(const char* sentence, int sentenceLength){
		//making uv texture array
		std::vector<glm::vec2>uvArray(
			YUG_TEXT_VERTS_PER_GLYPH*sentenceLength);
		//possible x and y values for this letter in uv
		float x1;
		float x2;
		float y1;
		float y2;
		int columnOffset = 0;
		for(int i = 0;
			i < sentenceLength * YUG_TEXT_VERTS_PER_GLYPH;
			i += YUG_TEXT_VERTS_PER_GLYPH)
		{
			char glyph = sentence[i/YUG_TEXT_VERTS_PER_GLYPH];
			//capital letter
			columnOffset = getLetterNumber(glyph);
			x1 = columnOffset * YUG_INFOTEXT_GLYPH_SPACE;
			x2 = x1 + YUG_INFOTEXT_GLYPH_WIDTH;
			y1 = 0;
			y2 = YUG_INFOTEXT_GLYPH_HEIGHT;
			//putting uv co-ords into uv array
			uvArray[i+0] = glm::vec2(x2, y1);
			uvArray[i+1] = glm::vec2(x2, y2);
			uvArray[i+2] = glm::vec2(x1, y2);
			uvArray[i+3] = glm::vec2(x2, y1);
			uvArray[i+4] = glm::vec2(x1, y2);
			uvArray[i+5] = glm::vec2(x1, y1);
		}//end uv for loop
		return uvArray;
	}

	void TextPrinter::setupGLState(
			const std::vector<glm::vec3>* vertices, 
			const std::vector<glm::vec2>* uvArray, 
			GLuint textureID)
	{
		//getting right shader program in
		if(!stateUnit.isActiveShaderProgram(YUG_TEXTURE_SHADER_PROGRAM)){
			stateUnit.useShaderProgram(YUG_TEXTURE_SHADER_PROGRAM);
		}
		//putting data into OpenGL
		glBindVertexArray(YUG_UNBIND);

		glBindBuffer(GL_ARRAY_BUFFER, textVertexID);
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(glm::vec3), vertices->data(),GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, textUVID);
		glBufferData(GL_ARRAY_BUFFER, uvArray->size() * sizeof(glm::vec2), uvArray->data(),GL_STATIC_DRAW);

		glBindVertexArray(textVAO);//alpha and symbol might clash?
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &amtran[0]);

	}

	void TextPrinter::printText(const char* sentence, int font,
			float scaleX, float scaleY, 
			float posX, float posY, float posZ,
			glm::vec3 rotationLine, float angle)
	{
		currentFont = font;
		unsigned int length = strlen(sentence);
		std::vector<glm::vec3>vertices = makeVertices(length, sentence, length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font,length);
		
		setupGLState(&vertices, &uvArray, getCorrectTexture());

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMatrix *= camera.getWorldToViewMatrix();
		glm::mat4 translationMatrix = glm::translate(projectionMatrix, glm::vec3(posX, posY, posZ) );
		glm::mat4 fullTransfromMatrix = glm::rotate(translationMatrix, angle, rotationLine);
		glm::mat4 finalMatrix = glm::scale(fullTransfromMatrix, glm::vec3(scaleX, scaleY, 1.0f));

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, length*YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);
	}

	void TextPrinter::printText(const char* sentence, int font,
			float scaleX, float scaleY, 
			float posX, float posY, float posZ,
			glm::mat4 rotationMatrix)
	{
		currentFont = font;
		unsigned int length = strlen(sentence);
		std::vector<glm::vec3>vertices = makeVertices(length, sentence,length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font,length);
		
		setupGLState(&vertices, &uvArray, getCorrectTexture());

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMatrix *= camera.getWorldToViewMatrix();
		glm::mat4 translationMatrix = glm::translate(projectionMatrix, glm::vec3(posX, posY, posZ) );
		glm::mat4 fullTransfromMatrix = translationMatrix*rotationMatrix ;
		glm::mat4 finalMatrix = glm::scale(fullTransfromMatrix, glm::vec3(scaleX, scaleY, 1.0f));

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, length*YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);
	}

	void TextPrinter::printText(const char* sentence, int font,
			float scaleX, float scaleY, 
			glm::mat4 projectionWorldMatrix)
	{
		currentFont = font;
		unsigned int length = strlen(sentence);
		std::vector<glm::vec3>vertices = makeVertices(length, sentence,length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font,length);
		
		setupGLState(&vertices, &uvArray, getCorrectTexture());

		glm::mat4 finalMatrix = 
			glm::scale(projectionWorldMatrix, glm::vec3(scaleX, scaleY, 1.0f));

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, length*YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);
	}

	void TextPrinter::printText(const char* sentence, int font,
			float scaleX, float scaleY,
			glm::mat4 projectionTranslateMatrix,
			glm::vec3 furtherTranslate,
			glm::mat4 rotationMatrix)
	{
		currentFont = font;
		unsigned int length = strlen(sentence);
		std::vector<glm::vec3>vertices = makeVertices(length, sentence,length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font,length);
		
		setupGLState(&vertices, &uvArray, getCorrectTexture());
		
		glm::mat4 rm = projectionTranslateMatrix * rotationMatrix;
		glm::mat4 tm = glm::translate(rm,furtherTranslate);
		glm::mat4 finalMatrix = glm::scale(tm, glm::vec3(scaleX, scaleY, 1.0f));

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, length*YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);

	}

	void TextPrinter::printText(const char* sentence, int font,
		glm::vec3 scale, glm::vec3 posi, glm::mat4 rot)
	{
		currentFont = font;
		unsigned int length = strlen(sentence);
		std::vector<glm::vec3>vertices = makeVertices(length, sentence,length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font,length);
		
		setupGLState(&vertices, &uvArray, getCorrectTexture());
		
		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMatrix *= camera.getWorldToViewMatrix();
		glm::mat4 translationMatrix = glm::translate(projectionMatrix, posi);
		glm::mat4 fullTransfromMatrix = translationMatrix*rot ;
		glm::mat4 finalMatrix = glm::scale(fullTransfromMatrix, scale);

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, length*YUG_TEXT_VERTS_PER_GLYPH );
		glBindVertexArray(YUG_UNBIND);
	}

	std::vector<glm::vec3> TextPrinter::makeSymbolVertex(){
		std::vector<glm::vec3>vertex(YUG_TEXT_VERTS_PER_GLYPH);
		vertex[0] = glm::vec3(YUG_TEXT_SYM_GLYPH_OFFSET, YUG_TEXT_SYM_GLYPH_OFFSET, 0.0f);
		vertex[1] = glm::vec3(YUG_TEXT_SYM_GLYPH_OFFSET, -YUG_TEXT_SYM_GLYPH_OFFSET,0.0f);
		vertex[2] = glm::vec3(-YUG_TEXT_SYM_GLYPH_OFFSET, -YUG_TEXT_SYM_GLYPH_OFFSET,0.0f);
		vertex[3] = glm::vec3(YUG_TEXT_SYM_GLYPH_OFFSET, YUG_TEXT_SYM_GLYPH_OFFSET,0.0f);
		vertex[4] = glm::vec3(-YUG_TEXT_SYM_GLYPH_OFFSET, -YUG_TEXT_SYM_GLYPH_OFFSET,0.0f);
		vertex[5] = glm::vec3(-YUG_TEXT_SYM_GLYPH_OFFSET, YUG_TEXT_SYM_GLYPH_OFFSET,0.0f);
		return vertex;
	}

	std::vector<glm::vec2> TextPrinter::makeSymbolUV(int symbol){
		float x1 = symbol *YUG_TEXT_SYM_GLYPH_WIDTH;
		float x2 = x1 + YUG_TEXT_SYM_GLYPH_WIDTH;
		std::vector<glm::vec2>uv(YUG_TEXT_VERTS_PER_GLYPH);
		uv[0] = glm::vec2(x2, 0.0f);
		uv[1] = glm::vec2(x2, 1.0f);
		uv[2] = glm::vec2(x1, 1.0f);
		uv[3] = glm::vec2(x2, 0.0f);
		uv[4] = glm::vec2(x1, 1.0f);
		uv[5] = glm::vec2(x1, 0.0f);
		return uv;
	}

	void TextPrinter::printSymbol(int symbol,
		float scaleX, float scaleY, 
		float posX, float posY, float posZ,
		glm::vec3 rotationLine, float angle)
	{
		std::vector<glm::vec3>vertices = makeSymbolVertex();
		std::vector<glm::vec2>uvArray = makeSymbolUV(symbol);

		setupGLState(&vertices, &uvArray, textSymbolTextureID);
		

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMatrix *= camera.getWorldToViewMatrix();
		glm::mat4 translationMatrix = glm::translate(projectionMatrix, glm::vec3(posX, posY, posZ) );
		glm::mat4 fullTransfromMatrix = glm::rotate(translationMatrix, angle, rotationLine);
		glm::mat4 finalMatrix = glm::scale(fullTransfromMatrix, glm::vec3(scaleX, scaleY, 1.0f));

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);

	}

	void TextPrinter::printSymbol(int symbol,
			float scaleX, float scaleY,
			glm::mat4 projectionTranslateMatrix,
			glm::vec3 furtherTranslate,
			glm::mat4 rotationMatrix)
	{
		
		std::vector<glm::vec3>vertices = makeSymbolVertex();
		std::vector<glm::vec2>uvArray = makeSymbolUV(symbol);
		
		setupGLState(&vertices, &uvArray, textSymbolTextureID);

		
		glm::mat4 projectionMat = stateUnit.perspectiveMatrix;
		projectionMat *= camera.getWorldToViewMatrix();
		
		glm::mat4 rm = projectionTranslateMatrix * rotationMatrix;
		glm::mat4 tm = glm::translate(rm,furtherTranslate);
		glm::mat4 finalMatrix = glm::scale(tm, glm::vec3(scaleX, scaleY, 0.0f));

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);

	}

	void TextPrinter::printSymbol(int symbol,
			glm::vec3 scale, glm::vec3 posi, glm::mat4 rot)
	{

		std::vector<glm::vec3>vertices = makeSymbolVertex();
		std::vector<glm::vec2>uvArray = makeSymbolUV(symbol);

		setupGLState(&vertices, &uvArray, textSymbolTextureID);
		

		glm::mat4 projectionMatrix = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMatrix *= camera.getWorldToViewMatrix();
		glm::mat4 translationMatrix = glm::translate(projectionMatrix, posi );
		glm::mat4 fullTransfromMatrix = translationMatrix * rot;
		glm::mat4 finalMatrix = glm::scale(fullTransfromMatrix, scale);

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);


	}

	void TextPrinter::addInfoNumberToUVs(std::vector<glm::vec2>* uvArray, int number){
		int noOfDigits = findNoOfDigits(number);
		float x1;
		float x2;
		float y1;
		float y2;
		int columnOffset = 0;
		int digits[4];//max no of digits
		digits[0] = number/1000;
		digits[1] = (number%1000)/100;
		digits[2] = (number%100)/10;
		digits[3] = number%10;
		int temp = 4-noOfDigits;
		for(int i = 0; i < temp; i++){
			x1 = YUG_INFOTEXT_GLYPH_SPACE * (73);//73 = SPACE
			x2 = x1+ YUG_INFOTEXT_GLYPH_WIDTH;
			y1 = 0;/////////////////////////////////////////////////////
			y2 = y1 + YUG_INFOTEXT_GLYPH_HEIGHT;
			uvArray->push_back( glm::vec2(x2, y1) );
			uvArray->push_back( glm::vec2(x2, y2) );
			uvArray->push_back( glm::vec2(x1, y2) );
			uvArray->push_back( glm::vec2(x2, y1) );
			uvArray->push_back( glm::vec2(x1, y2) );
			uvArray->push_back( glm::vec2(x1, y1) );
		}
		for(int i = temp; i < 4; i++){
			x1 = YUG_INFOTEXT_GLYPH_SPACE * (digits[i] + (2*YUG_TEXT_LOWER_LETTER_OFFSET));
			x2 = x1+ YUG_INFOTEXT_GLYPH_WIDTH;
			y1 = 0;/////////////////////////////////////////////////////
			y2 = y1 + YUG_INFOTEXT_GLYPH_HEIGHT;
			uvArray->push_back( glm::vec2(x2, y1) );
			uvArray->push_back( glm::vec2(x2, y2) );
			uvArray->push_back( glm::vec2(x1, y2) );
			uvArray->push_back( glm::vec2(x2, y1) );
			uvArray->push_back( glm::vec2(x1, y2) );
			uvArray->push_back( glm::vec2(x1, y1) );
		 }
	}
	void TextPrinter::addNameNumberToUVs(std::vector<glm::vec2>* uvArray, int number){
		int noOfDigits = findNoOfDigits(number);
		float x1;
		float x2;
		float y1;
		float y2;
		char digits[4];//max no of digits
		digits[0] = '0'+ number/1000;
		digits[1] = '0'+ (number%1000)/100;
		digits[2] = '0'+ (number%100)/10;
		digits[3] = '0'+ number%10;
		int temp = 4-noOfDigits;
		for(int i = 0; i < temp; i++){
			x1 = textStartPoints[73];//73 = SPACE
			x2 = x1+ textWidths[73];
			y1 = 0;/////////////////////////////////////////////////////
			y2 = YUG_INFOTEXT_GLYPH_HEIGHT;
			uvArray->push_back( glm::vec2(x2, y1) );
			uvArray->push_back( glm::vec2(x2, y2) );
			uvArray->push_back( glm::vec2(x1, y2) );
			uvArray->push_back( glm::vec2(x2, y1) );
			uvArray->push_back( glm::vec2(x1, y2) );
			uvArray->push_back( glm::vec2(x1, y1) );
		}
		for(int i = temp; i < 4; i++){
			int lNumber = getLetterNumber(digits[i]);
			x1 = textStartPoints[lNumber];
			x2 = x1 + textWidths[lNumber];
			y1 = 0;
			y2 = YUG_INFOTEXT_GLYPH_HEIGHT;
			uvArray->push_back( glm::vec2(x2, y1) );
			uvArray->push_back( glm::vec2(x2, y2) );
			uvArray->push_back( glm::vec2(x1, y2) );
			uvArray->push_back( glm::vec2(x2, y1) );
			uvArray->push_back( glm::vec2(x1, y2) );
			uvArray->push_back( glm::vec2(x1, y1) );
		 }
	}
			

	void TextPrinter::addNumberToUVs(std::vector<glm::vec2>* uvArray, int number, int font){
		if(font == YUG_TEXT_NAME_FONT)
			addNameNumberToUVs(uvArray, number);
		else
			addInfoNumberToUVs(uvArray, number);
	}

	void TextPrinter::printTextAndNumber(
			const char* sentence, int number,
			int font,float scaleX, float scaleY,
			glm::mat4 projectionTranslateMatrix,
			glm::vec3 furtherTranslate,
			glm::mat4 rotationMatrix)
	{
		currentFont = font;
		unsigned int length = strlen(sentence);
		unsigned int noOfDigits = 4;//findNoOfDigits(number);
		std::vector<glm::vec3>vertices = makeVertices(length+noOfDigits, sentence,length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font, length);
		addNumberToUVs(&uvArray, number, font);
		setupGLState(&vertices, &uvArray, getCorrectTexture());

		
		glm::mat4 projectionMat = stateUnit.perspectiveMatrix;
		projectionMat *= camera.getWorldToViewMatrix();
		
		glm::mat4 rm = projectionTranslateMatrix * rotationMatrix;
		glm::mat4 tm = glm::translate(rm,furtherTranslate);
		glm::mat4 finalMatrix = glm::scale(tm, glm::vec3(scaleX, scaleY, 0.0f));

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, (noOfDigits+length)*YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);
	}

	void TextPrinter::printTextAndNumber(const char* sentence, int number,
			int font,
			float scaleX, float scaleY, 
			float posX, float posY, float posZ,
			glm::mat4 rotationMatrix){
		currentFont = font;

		unsigned int length = strlen(sentence);
		unsigned int noOfDigits = 4;//findNoOfDigits(number);
		std::vector<glm::vec3>vertices = makeVertices(length+noOfDigits,sentence,length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font, length);
		addNumberToUVs(&uvArray, number, font);
		setupGLState(&vertices, &uvArray, getCorrectTexture());

		
		glm::mat4 projectionMat = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMat *= camera.getWorldToViewMatrix();
		glm::mat4 tm = glm::translate(projectionMat, glm::vec3(posX, posY, posZ));
		glm::mat4 rtm = tm * rotationMatrix;
		glm::mat4 finalMatrix = glm::scale(rtm, glm::vec3(scaleX, scaleY, 0.0f));

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, (noOfDigits+length)*YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);
	}

	void TextPrinter::printNumber(int number,
			int font,float scaleX, float scaleY,
			float posX, float posY, float posZ,
			glm::mat4 rotationMatrix){
			unsigned int noOfDigits = findNoOfDigits(number);
			std::vector<glm::vec3>vertices = makeVertices(noOfDigits, 0,0);
			std::vector<glm::vec2>uvArray;
			addNumberToUVs(&uvArray, number, font);
			setupGLState(&vertices, &uvArray, getCorrectTexture());
	
		
			glm::mat4 projectionMat = stateUnit.perspectiveMatrix;
			if(!ignoreCamera)
				projectionMat *= camera.getWorldToViewMatrix();
		
			glm::mat4 tm = glm::translate(projectionMat, 
				glm::vec3(posX, posY, posZ));
			glm::mat4 rtm = tm * rotationMatrix;
			glm::mat4 finalMatrix = glm::scale(rtm, glm::vec3(scaleX, scaleY, 0.0f));

			glUniformMatrix4fv( 
				stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
			glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

			// Draw call
			glDrawArrays(GL_TRIANGLES, 0, (noOfDigits)*YUG_TEXT_VERTS_PER_GLYPH );

			//glDisable(GL_BLEND);
			glBindVertexArray(YUG_UNBIND);
	}

	void TextPrinter::printNumber(int number,
			int font, glm::vec3 scale, 
			glm::vec3 posi, glm::mat4 rot)
	{
		unsigned int noOfDigits = 4;//findNoOfDigits(number);
		std::vector<glm::vec3>vertices = makeVertices(noOfDigits, 0,0);
		std::vector<glm::vec2>uvArray;
		addNumberToUVs(&uvArray, number, font);
		setupGLState(&vertices, &uvArray, getCorrectTexture());
	
		
		glm::mat4 projectionMat = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMat *= camera.getWorldToViewMatrix();
		glm::mat4 tm = glm::translate(projectionMat, posi);
		glm::mat4 rtm = tm * rot;
		glm::mat4 finalMatrix = glm::scale(rtm, scale);

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, (noOfDigits)*YUG_TEXT_VERTS_PER_GLYPH );
		glBindVertexArray(YUG_UNBIND);
	}

	void TextPrinter::printText(const char* sentence, int font,
			glm::mat4 parentsMatrix,
			glm::mat4 furtherMatrix)
	{
		currentFont = font;
		unsigned int length = strlen(sentence);
		std::vector<glm::vec3>vertices = makeVertices(length, sentence,length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font,length);
		
		setupGLState(&vertices, &uvArray, getCorrectTexture());
		

		glm::mat4 finalMatrix = parentsMatrix * furtherMatrix;

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, length*YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);
	}
	void TextPrinter::printSymbol(int symbol,
			glm::mat4 parentsMatrix,
			glm::mat4 furtherMatrix)
	{
		
		std::vector<glm::vec3>vertices = makeSymbolVertex();
		std::vector<glm::vec2>uvArray = makeSymbolUV(symbol);
		
		setupGLState(&vertices, &uvArray, textSymbolTextureID);


		glm::mat4 finalMatrix = parentsMatrix * furtherMatrix;

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);

	}
	void TextPrinter::printTextAndNumber(const char* sentence, 
			int number, int font,
			glm::mat4 parentsMatrix,
			glm::mat4 furtherMatrix)
	{
		currentFont = font;
		unsigned int length = strlen(sentence);
		unsigned int noOfDigits = 4;//findNoOfDigits(number);
		std::vector<glm::vec3>vertices = makeVertices(length+noOfDigits, sentence,length);
		std::vector<glm::vec2>uvArray = makeAlphabetUVs(sentence, font, length);
		addNumberToUVs(&uvArray, number, font);
		setupGLState(&vertices, &uvArray, getCorrectTexture());
	
		glm::mat4 finalMatrix = parentsMatrix * furtherMatrix;

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, (noOfDigits+length)*YUG_TEXT_VERTS_PER_GLYPH );

		//glDisable(GL_BLEND);
		glBindVertexArray(YUG_UNBIND);
	}

	
	void TextPrinter::printDamageNumber(int number,
			glm::vec3 scale, glm::vec3 posi)
	{
		currentFont = YUG_TEXT_INFO_FONT;
		int length = 5;
		if(number < 1000)
			length = 4;
		if(number < 100 )
			length = 3;
		std::vector<glm::vec3>vertices = makeVertices(length, 0,0);
		std::vector<glm::vec2>uvArray = makeDamageUVs(number, length-1);
		setupGLState(&vertices, &uvArray, textDamTextureID);

		glm::mat4 projectionMat = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMat *= camera.getWorldToViewMatrix();
		glm::mat4 tm = glm::translate(projectionMat, posi);
		glm::mat4 finalMatrix = glm::scale(tm, scale);

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, (length)*YUG_TEXT_VERTS_PER_GLYPH );
		glBindVertexArray(YUG_UNBIND);
		
	}

	std::vector<glm::vec2> TextPrinter::makeDamageUVs(
			int number, int digits)
	{
		float x1;
		float x2;
		float y1;
		float y2;
		std::vector<glm::vec2>uvs((digits+1)*YUG_TEXT_VERTS_PER_GLYPH);//room for digits and minus symbol
		int remainder;//stat values: 24,18,12,6,0
		for(int i = ((digits)*YUG_TEXT_VERTS_PER_GLYPH); i > 0; i-=YUG_TEXT_VERTS_PER_GLYPH){
			remainder = number%10;//going backwards through array getting number from the back
			number /= 10;
			x1 = YUG_TEXT_DAM_WIDTH*remainder;
			x2 = x1 + YUG_TEXT_DAM_WIDTH;
			y1 = 0.0f;
			y2 = YUG_TEXT_DAM_HEIGHT;
			uvs[i + 0] = glm::vec2( glm::vec2(x2, y1) );
			uvs[i + 1] = glm::vec2( glm::vec2(x2, y2) );
			uvs[i + 2] = glm::vec2( glm::vec2(x1, y2) );
			uvs[i + 3] = glm::vec2( glm::vec2(x2, y1) );
			uvs[i + 4] = glm::vec2( glm::vec2(x1, y2) );
			uvs[i + 5] = glm::vec2( glm::vec2(x1, y1) );
		}
		x1 = YUG_TEXT_DAM_WIDTH*10;//adding the minus symbol
		x2 = x1 + YUG_TEXT_DAM_WIDTH;
		y1 = 0.0f;
		y2 = YUG_TEXT_DAM_HEIGHT;
		uvs[0] = glm::vec2( glm::vec2(x2, y1) );
		uvs[1] = glm::vec2( glm::vec2(x2, y2) );
		uvs[2] = glm::vec2( glm::vec2(x1, y2) );
		uvs[3] = glm::vec2( glm::vec2(x2, y1) );
		uvs[4] = glm::vec2( glm::vec2(x1, y2) );
		uvs[5] = glm::vec2( glm::vec2(x1, y1) );
		return uvs;
	}

	void TextPrinter::printMagicNumber(int number,
			glm::vec3 scale, glm::vec3 posi)
	{
		currentFont = YUG_TEXT_INFO_FONT;
		int length = 4;
		if(number < 1000)
			length = 3;
		if(number < 100 )
			length = 2;
		std::vector<glm::vec3>vertices = makeVertices(length, 0,0);
		std::vector<glm::vec2>uvArray = makeMagicUVs(number, length);
		setupGLState(&vertices, &uvArray, textMagTextureID);
		glUniform4fv(stateUnit.textureAmtranUniformLocation, 1, &magicAmtran[0]);

		glm::mat4 projectionMat = stateUnit.perspectiveMatrix;
		if(!ignoreCamera)
			projectionMat *= camera.getWorldToViewMatrix();
		glm::mat4 tm = glm::translate(projectionMat, posi);
		glm::mat4 finalMatrix = glm::scale(tm, scale);

		glUniformMatrix4fv( 
			stateUnit.textureMatrixUniformLocation, 1, GL_FALSE, &finalMatrix[0][0]);
		glUniform1f( stateUnit.textureSamplerUniformLocation, 0.2f);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, (length)*YUG_TEXT_VERTS_PER_GLYPH );
		glBindVertexArray(YUG_UNBIND);
		
	}

	std::vector<glm::vec2> TextPrinter::makeMagicUVs(
		int number, int digits)
	{
		float x1;
		float x2;
		float y1;
		float y2;
		std::vector<glm::vec2>uvs((digits)*YUG_TEXT_VERTS_PER_GLYPH);//room for digits and minus symbol
		int remainder;//stat values: 24,18,12,6,0
		for(int i = ((digits-1)*YUG_TEXT_VERTS_PER_GLYPH); i >= 0; i-=YUG_TEXT_VERTS_PER_GLYPH){
			remainder = number%10;//going backwards through array getting number from the back
			number /= 10;
			x1 = YUG_TEXT_MAG_WIDTH*remainder;
			x2 = x1 + YUG_TEXT_MAG_WIDTH;
			y1 = 0.0f;
			y2 = YUG_TEXT_MAG_HEIGHT;
			uvs[i + 0] = glm::vec2( glm::vec2(x2, y1) );
			uvs[i + 1] = glm::vec2( glm::vec2(x2, y2) );
			uvs[i + 2] = glm::vec2( glm::vec2(x1, y2) );
			uvs[i + 3] = glm::vec2( glm::vec2(x2, y1) );
			uvs[i + 4] = glm::vec2( glm::vec2(x1, y2) );
			uvs[i + 5] = glm::vec2( glm::vec2(x1, y1) );
		}
		return uvs;
	}

	unsigned int TextPrinter::findNoOfDigits(int number){
		if(number < 0 || number > 9999){
			//errorHandler.printError("Text printer passed number too big/too small");
			number = 9191;
		}
		if(number < 10)
			return 1;
		if(number < 100)
			return 2;
		if(number < 1000)
			return 3;
		return 4;
	}

	bool TextPrinter::onNameFont(){
		return currentFont == YUG_TEXT_NAME_FONT;
	}
	bool TextPrinter::onInfoFont(){
		return currentFont == YUG_TEXT_INFO_FONT;
	}


}