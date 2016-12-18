#include <sstream>
#include <Utility\Clock.h>
#include <Utility\InputUnit.h>
#include <Screens\StoryScene.h>
#include <Utility\TextureLoader.h>
#include <Utility\TextPrinter.h>
#include <Game\PlayerData.h>
#include <DefinesAndTypedefs.h>
#include <Screens\ScreenUnit.h>
#include <Game\EnemyData.h>
#include <Screens\Trunk.h>
#include <Screens\TempleMapScreen.h>
#include <Screens\WorldMapScreen.h>
#include <Game\Animation\FadeUnit.h>
#include <Screens\SplashScreen.h>
#include <Game\Duel\Board.h>
#include <Screens\FinalScreen.h>
#include <Utility\SoundUnit.h>

#define ZYUG_CH_START 0
#define ZYUG_CH_ACTIVE 1
#define ZYUG_CH_IDLE 2
#define ZYUG_CH_YESNO1 3
#define ZYUG_CH_YESNO2 4
#define ZYUG_CH_YESNO3 10
#define ZYUG_CH_END 5
#define ZYUG_CH_TALK1 6
#define ZYUG_CH_TALK2 7
#define ZYUG_CH_TALK3 8
#define ZYUG_CH_NEXT_PART 9
#define ZYUG_CH_YESNO_EXIT1 11

namespace Screen{

	void StoryScene::startup(const char* fileLocal){
		BaseScreen::startup();
		fadeUnit.changeZ(-1.0f);
		fadeUnit.sheet.amtran = glm::vec4(0.0001f,0,0,1);
		//std::cout<<"SS: startup start() file="<<fileLocal<<"\n";
		in = std::ifstream(fileLocal);
		std::string s;
		in>>id; //save scene ID
		in>>s; //get BG path
		//std::cout<<"SS: go bg path: "<<id<<" "<<s<<" "<<in.good()<<"\n";
		in>>nextScreenKey;
		in>>nextFileString;
		//std::cout<<"SS: next screen file: "<<nextFileString<<"\n";
		in>>musicStr;
		if(musicStr!="MUSIC"){
			soundUnit.playLoop(musicStr.c_str());
		}
		setupBG(s.c_str());
		setupTextBox();
		setupCursor();
		setupYesNoCursor();
		doRenderText = false;
		cursorTimer = 0;
		cursorTexPos = 0;
		yesNoPos = 0;
		chain = ZYUG_CH_START;
		isEarthquaking = false;
		in>>key;
		//std::cout<<"SS: startup end()\n";
	}

	void StoryScene::cleanup(){
		//std::cout<<"SS: cleanup start()\n";
		bg.cleanup();
		cursor.textureBO = cursorTexs[0];
		textureLoader.deleteTexture(&(cursorTexs[1]));
		textureLoader.deleteTexture(&(cursorTexs[2]));
		cursor.cleanup();
		textBox.cleanup();
		textPrinter.amtran = glm::vec4(1,1,1,1);
		soundUnit.stopAll();
		in.close();
	}

	void StoryScene::readNextPart(){
		//std::cout<<"Story: NextPart: haskey = "<<key<<std::endl;
		switch(key){
		case ':': loadNewChar(); in>>key; break;
		case '&': moveCharIn(); in>>key; break;
		case '^': moveCharOut(); in>>key; break;
		case '{': startYesNo(); in>>key; break;
		case '_': jumpCharIn(); in>>key; break;
		case '@': startNewText(); in>>key; break;
		case ';': startFadeOut(); break;
		case '+': unlockDuelist(); in>>key; break;
		case '=': assignPlotPoint(); in>>key; break; 
		case '~': startFadeIn(); in>>key; break;
		case '[': hideBg(); in>>key; break;
		case ']': showBg(); in>>key; break;
		case '$': startDuel(); in>>key; break;
		case '|': fadeOutBg(); in>>key; break;
		case '/': changeBG(); in>>key; break;
		case '<': readBattleMusicIn(); in>>key; break;
		case '>': earthquake(); in>>key; break;
		case '\'': zoomInBg(); in>>key; break; 
		case '(': soundUnit.playOnce("GameData/sounds/music/ready/itemGot_o.wav");
			in>>key; break;
		default: break;
		}
	}

	void StoryScene::earthquake(){
		isEarthquaking = true;
		in>>quakeTimer;
		shakeTimer = 0;
		soundUnit.stopAll();
		soundUnit.playOnce("GameData/sounds/magic/earthquake.wav");
		chain = ZYUG_CH_IDLE;
	}

	void StoryScene::zoomInBg(){
		bg.scale = glm::vec3(0.84f, 0.48f, 1);
	}

	void StoryScene::readBattleMusicIn(){
		float f;
		std::string lead, main;
		in>>f; theBoard.leadInLength = f;
		in>>lead; theBoard.leadInTrack = lead;
		in>>main; theBoard.mainTrack = main;
	}

	void StoryScene::changeBG(){
		std::string s;
		in>>s;
		textureLoader.deleteTexture(&bg.textureBO);
		textureLoader.loadTexture(s.c_str(), &bg.textureBO);
	}

	void StoryScene::fadeOutBg(){
		bg.interpolateAmtran(glm::vec4(1,1,1,0), 0.3f);
		wait(0.3f);
	}

	void StoryScene::hideBg(){
		bg.amtran = glm::vec4(1,1,1,0);
	}

	void StoryScene::showBg(){
		bg.interpolateAmtran(glm::vec4(1,1,1,1), 0.3f);
	}

	void StoryScene::loadNewChar(){
		std::string s;
		in>>s;
		Game::ModelLoader m;
		m.startup(YUG_PLANE_FILE_PATH, s.c_str());
		m.doRender = true;
		m.ignoreCamera = true;
		m.scale = glm::vec3(0.65f, 0.45f, 1);
		m.position = glm::vec3(-2.2f, 0.2f, -1.998f);
		m.position.z -= chars.size()*0.00001f;
		chars.push_back(m);
	}
	void StoryScene::moveCharIn(){
		int charNo = 0, scenePos = 0;
		in>>charNo; in>>scenePos;
		glm::vec3 start, end;
		float z = -1.998f + (charNo*0.00001f);
		Game::ModelLoader* charPtr = &(chars[charNo]);
		if(scenePos == 0){ //left
			start = glm::vec3(-2.2f, 0.2f, z);
			end = glm::vec3(-0.2f, 0.2f, z);
		}else if(scenePos == 1){ //right
			start = glm::vec3(2.3f, 0.2f, z);
			end = glm::vec3(0.3f, 0.2f, z);
		}else{ //centre
			start = glm::vec3(-2.2f, 0.2f, z);
			end = glm::vec3(0.0f, 0.2f, z);
		}
		//std::cout<<"Story: moveCharIn: old pos = "<<chars[charNo].position.z<<std::endl;
		chars[charNo].position = start;
		chars[charNo].interpolate(end, 0.5f);
		wait(0.5f);
	}

	void StoryScene::moveCharOut(){
		int charNo = 0, scenePos = 0;
		in>>charNo; in>>scenePos;
		glm::vec3 end;
		Game::ModelLoader* charPtr = &(chars[charNo]);
		if(scenePos == 0){ //left
			end = glm::vec3(-2.2f, 0.2f, -1.998f);
		}else{ //right
			end = glm::vec3(2.3f, 0.2f, -1.998f);
		}
		charPtr->interpolate(end, 0.3f);
		wait(0.3f);
	}

	void StoryScene::jumpCharIn(){
		int charNo = 0, scenePos = 0;
		in>>charNo; in>>scenePos;
		glm::vec3 end;
		Game::ModelLoader* charPtr = &(chars[charNo]);
		if(scenePos == 0){ //left
			end = glm::vec3(-0.2f, 0.2f, -1.998f);
		}else if(scenePos == 1){ //right
			end = glm::vec3(0.3f, 0.2f, -1.998f);
		}else{ //centre
			end = glm::vec3(0.0f, 0.2f, -1.998f);
		}
		charPtr->position = end;
	}

	void StoryScene::startNewText(){ // @ Im a goat " watch your butt %
		text.clear();
		std::string s;
		std::stringstream ss;
		in>>s; if(s=="*") s=playerData.name;
		if(s=="*,") s=playerData.name+",";
		while(s[0]!='%'){
			if(s[0]=='\"'){
				text.push_back(ss.str());
				ss.str("");
				in>>s; if(s=="*") s=playerData.name;
				if(s=="*,") s=playerData.name+",";
			}else{
				ss<<s; ss<<' ';
				in>>s; if(s=="*") s=playerData.name;
				if(s=="*,") s=playerData.name+",";
			}
		}
		text.push_back(ss.str());
		textTimer = 0;
		textAmtran = glm::vec4(1,1,1,0);
		chain = ZYUG_CH_TALK1;
		doRenderText = true;
	}

	void StoryScene::startFadeOut(){
		fadeOut(0.5f);
		chain = ZYUG_CH_END;
		wait(0.5f);
	}

	void StoryScene::unlockDuelist(){
		int ul;
		in>>ul;
		playerData.unlockedDuelists[ul].hasUnlocked = true;
		playerData.plotUnlockedDuelists[ul] = true;
	}

	void StoryScene::assignPlotPoint(){
		int pp;
		in>>pp;
		playerData.currentPlotPoint = pp;
	}

	void StoryScene::startFadeIn(){
		fadeIn(0.3f);
		wait(0.1f);
	}

	void StoryScene::startYesNo(){
		text.clear();
		std::string s;
		std::stringstream ss;
		in>>s;
		while(s[0]!='%'){
			if(s[0]=='\"'){
				text.push_back(ss.str());
				ss.str("");
				in>>s;
			}else{
				ss<<s; ss<<' ';
				in>>s;
			}
		}
		text.push_back(ss.str());
		in>>yesStr; in>>noStr;
		textTimer = 0;
		textAmtran = glm::vec4(1,1,1,0);
		chain = ZYUG_CH_YESNO1;
		yesNoCursor.doRender = true;
		yesNoCursor.interpolateAmtran(glm::vec4(1,1,1,0.5f), 0.4f);
		doRenderText = true;
	}

	void StoryScene::startDuel(){
		int duelNumber, fieldType;
		std::string winStr, loseStr;
		in>>duelNumber;
		in>>fieldType; trunkUnit.fieldType = fieldType;
		in>>winStr; screenUnit.winStr = winStr;
		in>>loseStr; screenUnit.loseStr = loseStr;
		std::stringstream ss;
		ss<<"GameData/enemies/"<<duelNumber<<".txt";
		enemyData.loadData(ss.str());
		startFadeOut();
	}

	void StoryScene::earthquakeUpdate(){
		if(!isEarthquaking) return;
		//std::cout<<"eq\n";
		quakeTimer -= gameClock.lastLoopTime();
		if(quakeTimer<=0){
			isEarthquaking = false;
			bg.position = glm::vec3(0.0f,0.206f, -2.00f);
			chain = ZYUG_CH_NEXT_PART;
		}else{
			shakeTimer += gameClock.lastLoopTime();
			if(shakeTimer > 0.2f){
				if(bg.position.x == 0.02f)
					bg.position.x = -0.02f;
				else
					bg.position.x = 0.02f;
				shakeTimer = 0;
			}
		}
	}

	void StoryScene::update(){
		yesNoCursor.update();
		bg.update();
		cursorUpdate();
		earthquakeUpdate();
		for(unsigned int i = 0; i < chars.size(); i++)
			chars[i].update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_CH_START: firstUpdate(); break;
			case ZYUG_CH_NEXT_PART: readNextPart(); break;
			case ZYUG_CH_TALK1: fadeTextInUpdate(); break;
			case ZYUG_CH_TALK3: fadeTextOutUpdate(); break;
			case ZYUG_CH_YESNO1: fadeYesNoInUpdate(); break;
			case ZYUG_CH_YESNO3: fadeYesNoOutUpdate(); break;
			case ZYUG_CH_YESNO_EXIT1: yesNoExitUpdate(); break;
			case ZYUG_CH_END: endUpdate(); break;
			default: break;
			}
		}else{
			continueWaiting();
		}
	}

	void StoryScene::cursorUpdate(){
		cursorTimer += gameClock.lastLoopTime()*3.2f;
		if(cursorTimer>=1.0f){
			cursorTexPos = (cursorTexPos+1)%3;
			cursor.textureBO = cursorTexs[cursorTexPos];
			cursorTimer = 0;
		}
	}

	void StoryScene::endUpdate(){
		if(nextScreenKey=='s'){
			screenUnit.comingScreen = new SplashScreen();
		}else if(nextFileString[0]=='d'){
			//std::cout<<"ss: to trunk\n";
			screenUnit.comingScreen = &trunkUnit;
		}else if(nextFileString[0]=='a'){
			screenUnit.comingScreen = new TempleMapScreen();
		}else if(nextFileString[0]=='c'){
			screenUnit.comingScreen = new TempleMapScreen();
		}else if(nextFileString[0]=='e'){
			screenUnit.comingScreen = new TempleMapScreen();
		}else if(nextFileString[0]=='w'){
			screenUnit.comingScreen = new TempleMapScreen();
			nextFileString[0]='d';
		}else if(nextFileString[0]=='h'){
			screenUnit.comingScreen = new WorldMapScreen();
		}else if(nextFileString[0]=='i'){
			screenUnit.comingScreen = new WorldMapScreen();
		}else if(nextFileString[0]=='j'){
			screenUnit.comingScreen = new WorldMapScreen();
		}else if(nextFileString[0]=='k'){
			screenUnit.comingScreen = new WorldMapScreen();
		}else if(nextFileString[0]=='l'){
			screenUnit.comingScreen = new WorldMapScreen();
		}else if(nextFileString[0]=='m'){
			screenUnit.comingScreen = new WorldMapScreen();
		}else if(nextFileString[0]=='n'){
			screenUnit.comingScreen = new WorldMapScreen();
		}else if(nextFileString[0]=='z'){
			screenUnit.comingScreen = new FinalScreen();
		}else{
			screenUnit.comingScreen = new StoryScene();
		}
		cleanup();
		chain = ZYUG_CH_IDLE;
		toNextScreen();
		
	}

	void StoryScene::toNextScreen(){
		screenUnit.startUpComingScreen(nextFileString.c_str());
	}

	void StoryScene::yesNoExitUpdate(){
		if(yesNoPos==0) nextFileString = yesStr;
		else nextFileString = noStr;
		key = ';';
		chain = ZYUG_CH_NEXT_PART;
	}

	void StoryScene::fadeTextInUpdate(){
		textTimer += gameClock.lastLoopTime()*2.0f;
		if(textTimer >= 1.0f){
			textAmtran = glm::vec4(1,1,1,1);
			chain = ZYUG_CH_TALK2;
			textTimer = 0;
		}else{
			textAmtran.w = textTimer;
		}
	}

	void StoryScene::fadeTextOutUpdate(){
		textTimer += gameClock.lastLoopTime()*2.5f;
		if(textTimer >= 1.0f){
			textAmtran = glm::vec4(1,1,1,0);
			chain = ZYUG_CH_NEXT_PART;
			textTimer = 0;
			doRenderText = false;
		}else{
			textAmtran.w = 1.0f-textTimer;
		}
	}

	void StoryScene::fadeYesNoInUpdate(){
		textTimer += gameClock.lastLoopTime()*2.0f;
		if(textTimer >= 1.0f){
			textAmtran = glm::vec4(1,1,1,1);
			chain = ZYUG_CH_YESNO2;
			textTimer = 0;
		}else{
			textAmtran.w = textTimer;
		}
	}

	void StoryScene::fadeYesNoOutUpdate(){
		textTimer += gameClock.lastLoopTime()*2.5f;
		if(textTimer >= 1.0f){
			textAmtran = glm::vec4(1,1,1,0);
			chain = ZYUG_CH_YESNO_EXIT1;
			textTimer = 0;
			doRenderText = false;
			yesNoCursor.doRender = false;
		}else{
			textAmtran.w = 1.0f-textTimer;
		}
	}

	void StoryScene::firstUpdate(){
		refreshScreenUnit();
		chain = ZYUG_CH_NEXT_PART;
		wait(0.3f);
	}

	void StoryScene::render(){
		bg.render();
		for(unsigned int i = 0; i < chars.size(); i++) chars[i].render();
		textBox.render();
		if(chain==ZYUG_CH_TALK2)cursor.render();
		if(doRenderText){renderText();}
		yesNoCursor.render();
	}

	void StoryScene::renderText(){
		textPrinter.leftAlign = true;
		textPrinter.ignoreCamera = true;
		textPrinter.amtran = textAmtran;
		glm::vec3 posi = glm::vec3(-0.7f,-0.363f,-1.991f);
		for(int i = 0; i < text.size(); i++){
			textPrinter.printText(text[i].c_str(), YUG_TEXT_INFO_FONT, 
				glm::vec3(0.3f, 0.7f, 1), posi, glm::mat4());
			posi.y -= 0.07f;
		}
	}

	void StoryScene::input(){
		if(!isWaiting){
			switch(chain){
			case ZYUG_CH_TALK2: talkInput(); break;
			case ZYUG_CH_YESNO2: yesNoInput(); break;
			default: break;
			}
		}
	}

	void StoryScene::yesNoInput(){
		if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
			yesNoPos = 1;
			yesNoCursor.position.y = -0.433f;
			soundUnit.cursorMove(); wait(0.2f);
		}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
			yesNoPos = 0;
			yesNoCursor.position.y = -0.363f;
			soundUnit.cursorMove(); wait(0.2f);
		}else if(inputUnit.isKeyActive(YUG_KEY_START)||inputUnit.isKeyActive(YUG_KEY_X)){
			yesNoCursor.interpolateAmtran(glm::vec4(1,1,1,0), 0.25f);
			chain = ZYUG_CH_YESNO3;
			soundUnit.cursorSelect();
		}
	}
	void StoryScene::talkInput(){
		if(inputUnit.isKeyActive(YUG_KEY_START)||inputUnit.isKeyActive(YUG_KEY_X)){
			chain = ZYUG_CH_TALK3;
			soundUnit.cursorSelect();
		}
	}

	void StoryScene::setupBG(const char* local){
		bg.startup( YUG_PLANE_FILE_PATH, local);
		bg.doRender = true;
		bg.ignoreCamera = true;
		bg.scale = glm::vec3(0.82f, 0.444f, 1);
		bg.position = glm::vec3(0.0f,0.206f, -2.00f);
	}

	void StoryScene::setupTextBox(){
		textBox.startup( YUG_PLANE_FILE_PATH, 
			YUG_SCREEN_WINDOW_TEXTURE_PATH);
		textBox.doRender = true;
		textBox.ignoreCamera = true;
		textBox.scale = glm::vec3(0.81f,0.217f,1);
		textBox.position = glm::vec3(0,-0.444f, -1.994f);
	}

	void StoryScene::setupCursor(){
		cursor.startup( YUG_PLANE_FILE_PATH, "GameData/textures/screens/singles/textbutton1.png");
		cursorTexs[0] = cursor.textureBO;
		textureLoader.loadTexture("GameData/textures/screens/singles/textbutton2.png", &cursorTexs[1]);
		textureLoader.loadTexture("GameData/textures/screens/singles/textbutton3.png", &cursorTexs[2]);
		cursor.doRender = true;
		cursor.ignoreCamera = true;
		cursor.scale = glm::vec3(0.03f, 0.03f, 1);
		cursor.position = glm::vec3(0.7f, -0.55f, -1.992f);
	}

	void StoryScene::setupYesNoCursor(){
		yesNoCursor.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/models/positionunit/concursor.png");
		yesNoCursor.doRender = false;
		yesNoCursor.ignoreCamera = true;
		yesNoCursor.amtran = glm::vec4(1,1,1,0);
		yesNoCursor.scale = glm::vec3(0.718f, 0.03f, 1);
		yesNoCursor.position = glm::vec3(-0.0f,-0.363f,-1.990f);
	}
}
