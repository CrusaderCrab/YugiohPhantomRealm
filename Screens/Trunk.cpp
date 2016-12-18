#include <algorithm>
#include <Screens\Trunk.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Utility\BlankUnit.h>
#include <Utility\StateUnit.h>
#include <Utility\UpdateUnit.h>
#include <Base\RenderUnit.h>
#include <Utility\InputUnit.h>
#include <Game\PlayerData.h>
#include <Utility\SoundUnit.h>
#include <Utility\TextPrinter.h>
#include <Game\Cards\CardDisplayUnit.h>
#include <Screens\ScreenUnit.h>
#include <Screens\FreeDuelScreen.h>
#include <Game\Duel\Board.h>
#include <iostream>

#include <Game\Cards\Magic\Forest.h>
#include <Game\Cards\Magic\Sogen.h>
#include <Game\Cards\Magic\Wasteland.h>
#include <Game\Cards\Magic\Mountain.h>
#include <Game\Cards\Magic\Umi.h>
#include <Game\Cards\Magic\Yami.h>

#define ZYUG_SHEET_CENTRE glm::vec3(0.0f,0.0f, -2.75f)
#define ZYUG_SHEET_X_OFF 2.24f
#define ZYUG_SHEET_SCALE glm::vec3(1.12f,0.9f,1.0f)
#define ZYUG_SCROLL_SCALE glm::vec3(0.025f,0.08f, 1.0f)
#define ZYUG_SCROLL_TOP 0.515f
#define ZYUG_SCROLL_BOT -0.635f
#define ZYUG_SCROLL_DIFF 1.15f
#define ZYUG_SCROLL_TOP_VEC glm::vec3(1.07f, 0.515f, -2.749f)
#define ZYUG_SORT_UNUSED_VEC glm::vec3(-0.117f, 0.82f, -2.749f)
#define ZYUG_SORT_DECK_VEC glm::vec3(-0.34f, 0.82f, -2.749f)
#define ZYUG_SORT_OFF 0.125f
#define ZYUG_SORT_SCALE glm::vec3(0.05f,0.03f, 1.0f)
#define ZYUG_MAIN_SCALE glm::vec3(1.11f,0.10f, 0.1f)
#define ZYUG_MAIN_START_VEC glm::vec3(-0.035f, 0.517f, -2.748f)
#define ZYUG_MAIN_Y_OFF -0.165f

#define ZYUG_VIEWING_DECK 0
#define ZYUG_VIEWING_UNUSED 1
#define ZYUG_VIEWING_NEITHER 2

#define ZYUG_CH_START 0
#define ZYUG_CH_IDLE 1
#define ZYUG_CH_IN_USE 2
#define ZYUG_CH_SWITCHING_UNUSED 4
#define ZYUG_CH_SWITCHING_DECK 5
#define ZYUG_CH_END 3
#define ZYUG_CH_DISPLAY_OVER 6
#define ZYUG_CH_CLOSED 7

#define ZYUG_SORT_NO 0
#define ZYUG_SORT_NAME 1
#define ZYUG_SORT_ATK 2
#define ZYUG_SORT_DEF 3
#define ZYUG_SORT_TYPE 4

namespace Screen{

	TrunkCard::TrunkCard():
	noInTrunk(0), noInDeck(0){

	}
	TrunkCard::TrunkCard(int tN, int dN):
		noInTrunk(tN), noInDeck(dN) {
	}
	TrunkCard::~TrunkCard(){
	}

	Trunk Trunk::trunkInstance; 

	bool Trunk::initialize(){
		setupSheets();
		setupScrollBar();
		setupSortCursor();
		setupMainCursor();
		return true;
	}
	bool Trunk::shutdown(){
		mainCursorModel.cleanup();
		sortCursorModel.cleanup();
		scrollBarModel.cleanup();
		deckSheet.cleanup();
		unusedSheet.cleanup();
		return true;
	}

	void Trunk::toThisScreen(){
		startup();
	}

	void Trunk::startup(const char* fileLocal){
		miscInfo = fileLocal[0];
		//std::cout<<"Trunk: got misc "<<miscInfo;
		startup();
	}

	void Trunk::startup(){
		BaseScreen::startup();///a
		//a/Game::WaitUnit::startup();
		//a/updateUnit.newUpdater(this);
		//a/stateUnit.renderer->newRenderer(this);
		//a/blankUnit.transitionInput(this, 0.5f);
		closed = false;
		//fadeUnit.changeZ(-2.76f);
		//fadeUnit.fadeTo(glm::vec4(0.0001f, 0.0f, 0.0f, 0.0f), 0.5f);

		loadTrunkCards();
		loadDeck();
		addDeckCardsToTrunk();
		addTrunkNoToDeck();
		totalCardsInTrunk = 0;
		for(TrunkCard& t: trunkCards){
			totalCardsInTrunk += t.noInDeck;
			totalCardsInTrunk += t.noInTrunk;
		}
		if(totalCardsInTrunk > 9999) totalCardsInTrunk = 9999;

		trunkSortType = ZYUG_SORT_NO;
		sort(trunkCards.begin(), trunkCards.end(),  
			[](TrunkCard& a, TrunkCard& b)
			{return a.c.cardNumber < b.c.cardNumber;}
		);
		trunkCursorPos = 0;
		trunkDistanceFromTop = 0;
		trunkDistanceFromBottom = 7;

		deckSortType = ZYUG_SORT_NO;
		sort(deckCards.begin(), deckCards.end(),  
			[](TrunkCard& a, TrunkCard& b)
			{return a.c.cardNumber < b.c.cardNumber;}
		);
		deckCursorPos = 0;
		deckDistanceFromTop = 0;
		deckDistanceFromBottom = 7;
		moveMainCursor();
		moveScrollBar();
		sortCursorModel.position = ZYUG_SORT_UNUSED_VEC;
		unusedSheet.position = ZYUG_SHEET_CENTRE;
		deckSheet.position = ZYUG_SHEET_CENTRE;
		deckSheet.position.x += ZYUG_SHEET_X_OFF;

		viewing = ZYUG_VIEWING_UNUSED;
		chain = ZYUG_CH_START;
		cardDisplayOver = true;
		wait(0.5f);
		fadeIn(0.2f);
		fadeUnit.changeZ(-2.7f);
		soundUnit.playLoop("GameData/sounds/music/ready/trunk_o.wav");
	}

	void Trunk::cleanup(){
		soundUnit.stopAll();
		trunkCards.clear();
		deckCards.clear();
	}

	void Trunk::render(){
		unusedSheet.render();
		deckSheet.render();
		scrollBarModel.render();
		sortCursorModel.render();
		mainCursorModel.render();
		renderTrunkCards();
		renderDeckCards();
		renderMiscNumbers();
	}
	void Trunk::renderMiscNumbers(){
		glm::vec3 trunkStart = unusedSheet.position;
		glm::vec3 deckStart = deckSheet.position;
		glm::vec3 offset = glm::vec3(-0.83f, 0.66f, +0.003f);
		glm::vec3 scale = glm::vec3(0.35f, 0.7f, 1.0f);
		textPrinter.printNumber(totalCardsInTrunk, YUG_TEXT_INFO_FONT,
			scale, trunkStart+offset, glm::mat4());

		offset.x += 1.625f;
		offset.y += 0.025f;
		textPrinter.printNumber(deckCards.size(), YUG_TEXT_INFO_FONT,
			scale, trunkStart+offset, glm::mat4());

		offset = glm::vec3(-0.88f, 0.695f, +0.003f);
		textPrinter.printNumber(totalCardsInTrunk, YUG_TEXT_INFO_FONT,
			scale, deckStart+offset, glm::mat4());

		offset.x += 1.65f;
		offset.y -= 0.037f;
		textPrinter.printNumber(deckCards.size(), YUG_TEXT_INFO_FONT,
			scale, deckStart+offset, glm::mat4());

	}
	void Trunk::renderTrunkCards(){
		glm::vec3 startPos = unusedSheet.position
							+ glm::vec3( -1.15f, 0.575f, +0.003f);
		float yOff = 0.165f;
		int topIndex = trunkCursorPos - trunkDistanceFromTop;
		for(unsigned int i = 0; i < 8; i++){
			int index = topIndex + i;
			if(0 <= index && index < trunkCards.size()){
				renderCard(trunkCards[index], startPos);
				startPos.y -= yOff;
			}
		}
	}
	void Trunk::renderDeckCards(){
		glm::vec3 startPos = deckSheet.position
							+ glm::vec3( -1.15f, 0.575f, +0.003f);
		float yOff = 0.165f;
		int topIndex = deckCursorPos - deckDistanceFromTop;
		for(unsigned int i = 0; i < 8; i++){
			int index = topIndex + i;
			if(0 <= index && index < deckCards.size()){
				renderCard(deckCards[index], startPos);
				startPos.y -= yOff;
			}
		}
	}

	void Trunk::renderCard(TrunkCard& t, glm::vec3 startPos){
		Card::CardData& tc = t.c;
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;

		glm::vec3 offset = glm::vec3(0.05f, -0.02f, 0.0f);
		glm::vec3 scale = glm::vec3(0.33f, 0.7f, 1.0f);
		textPrinter.printNumber(tc.cardNumber,YUG_TEXT_INFO_FONT,
			scale,startPos+offset, glm::mat4() );

		offset = glm::vec3(0.3f, -0.06f, 0.0f);
		scale = glm::vec3(0.5f, 1.4f, 1.0f);
		if(tc.name.size() < 13){
			textPrinter.printText(tc.name.data(), YUG_TEXT_INFO_FONT, 
				scale, startPos+offset, glm::mat4() );
		}else{
			std::vector<char> vc = tc.name;
			vc.resize(12); vc.push_back('\0');
			textPrinter.printText(vc.data(), YUG_TEXT_INFO_FONT, 
				scale, startPos+offset, glm::mat4() );
		}

		if(tc.monMagTrap == YUG_MONSTER_CARD){
			offset = glm::vec3(1.21f, -0.02f, 0.0f);
			scale = glm::vec3(0.35f, 0.7f, 1.0f);
			glm::vec3 posi = startPos+offset;
			textPrinter.printTextAndNumber("/",tc.attack,YUG_TEXT_INFO_FONT,
				scale.x, scale.y, 
				posi.x, posi.y, posi.z, glm::mat4() );
			posi.y -= 0.07f;
			textPrinter.printTextAndNumber("@",tc.defense,YUG_TEXT_INFO_FONT,
				scale.x, scale.y, 
				posi.x, posi.y, posi.z, glm::mat4() );
		}

		offset = glm::vec3(1.545f, -0.045f, 0.0f);
		scale = glm::vec3(0.9f, 0.9f, 1.0f);
		textPrinter.printSymbol(tc.actualType, scale, startPos+offset, glm::mat4());
		
		if(tc.monMagTrap == YUG_MONSTER_CARD){
			offset.y -= 0.01f;
			offset.x += 0.143f;
			textPrinter.printSymbol(tc.constellations[0], scale, startPos+offset, glm::mat4());
			offset.x += 0.12f;
			textPrinter.printSymbol(tc.constellations[1], scale, startPos+offset, glm::mat4());
		}

		offset = glm::vec3(1.86f, -0.09f, 0.0f);
		scale = glm::vec3(0.33f, 0.7f, 1.0f);
		textPrinter.printNumber(t.noInTrunk, YUG_TEXT_INFO_FONT,
			scale, startPos+offset, glm::mat4() );
		offset.x += 0.13f;
		textPrinter.printNumber(t.noInDeck, YUG_TEXT_INFO_FONT,
			scale, startPos+offset, glm::mat4() );
		

	}


	void Trunk::input(){
		cardDisplayOver = true;
		if(chain == ZYUG_CH_IN_USE && viewing!=ZYUG_VIEWING_NEITHER && !isWaiting){
			if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
				currentCursorDown();
				wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
				currentCursorUp();
				wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_LEFT)){
				if(viewing==ZYUG_VIEWING_DECK){
					switchView();
					wait(0.2f);
				}
			}else if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){
				if(viewing==ZYUG_VIEWING_UNUSED){
					switchView();
					wait(0.2f);
				}
			}else if(inputUnit.isKeyActive(YUG_KEY_START)){
				switchCurrentSort();
				wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_X)){
				if(viewing==ZYUG_VIEWING_DECK){
					removeCurrentCard();
					wait(0.2f);
				}else{
					addCurrentCard();
					wait(0.2f);
				}
			}else if(inputUnit.isKeyActive(YUG_KEY_O)){
				exitTrunk();
				wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_TRI)){
				viewCurrentCard();
				wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_R1)){
				sprintDown();
				wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_L1)){
				sprintUp();
				wait(0.2f);
			}
		}
	}

	void Trunk::sprintUp(){
		int& cPos = (viewing==ZYUG_VIEWING_UNUSED)? trunkCursorPos : deckCursorPos;
		int& disTop = (viewing==ZYUG_VIEWING_UNUSED)? trunkDistanceFromTop : deckDistanceFromTop;
		int& disBot = (viewing==ZYUG_VIEWING_UNUSED)? trunkDistanceFromBottom : deckDistanceFromBottom;
		//std::cout<<"ccU: "<<cPos<<" "<<disTop<<" / "<<disBot<<std::endl;
		if(cPos<=0){
			cPos = 0;
			soundUnit.cursorSelectionFail(); 
			return;
		}
		if(cPos < 20){
			cPos = 0;
		}else{
			cPos-=20;
		}
		disTop = 0;
		disBot = 7;
		moveMainCursor();
		moveScrollBar();
		soundUnit.cursorMove();
	}
	void Trunk::sprintDown(){
		int& cPos = (viewing==ZYUG_VIEWING_UNUSED)? trunkCursorPos : deckCursorPos;
		int& disTop = (viewing==ZYUG_VIEWING_UNUSED)? trunkDistanceFromTop : deckDistanceFromTop;
		int& disBot = (viewing==ZYUG_VIEWING_UNUSED)? trunkDistanceFromBottom : deckDistanceFromBottom;
		int maxDown = (viewing==ZYUG_VIEWING_UNUSED) ?trunkCards.size()-1 : deckCards.size()-1;
		//std::cout<<"ccD: "<<cPos<<" "<<disTop<<" / "<<disBot<<" "<<maxDown<<std::endl;
		if(cPos>=maxDown || disTop>=maxDown){
			cPos = maxDown;
			soundUnit.cursorSelectionFail(); return;
		}
		if(cPos+20 >= maxDown){
			cPos = maxDown;
		}else{
			cPos+=20;
		}
		disTop = 7;
		disBot = 0;
		moveMainCursor();
		moveScrollBar();
		soundUnit.cursorMove();

	}

	void Trunk::switchCurrentSort(){
		int& sortNo = (viewing==ZYUG_VIEWING_UNUSED)? trunkSortType : deckSortType;
		std::vector<TrunkCard>& cards = (viewing==ZYUG_VIEWING_UNUSED)? trunkCards : deckCards;
		sortNo = (sortNo+1)%5;
		sortCards(cards, sortNo);

		moveMainCursor();
		moveScrollBar();
		moveSortCursor();
		soundUnit.cursorSelect();
	}

	void Trunk::currentCursorUp(){
		int& cPos = (viewing==ZYUG_VIEWING_UNUSED)? trunkCursorPos : deckCursorPos;
		int& disTop = (viewing==ZYUG_VIEWING_UNUSED)? trunkDistanceFromTop : deckDistanceFromTop;
		int& disBot = (viewing==ZYUG_VIEWING_UNUSED)? trunkDistanceFromBottom : deckDistanceFromBottom;
		//std::cout<<"ccU: "<<cPos<<" "<<disTop<<" / "<<disBot<<std::endl;
		if(cPos<=0){
			cPos = 0;
			soundUnit.cursorSelectionFail(); 
			return;
		}
		cPos--;
		disTop--; if(disTop<0) disTop = 0;
		disBot++; if(disBot>7) disBot = 7;
		moveMainCursor();
		moveScrollBar();
		soundUnit.cursorMove();
	}
	void Trunk::currentCursorDown(){
		int& cPos = (viewing==ZYUG_VIEWING_UNUSED)? trunkCursorPos : deckCursorPos;
		int& disTop = (viewing==ZYUG_VIEWING_UNUSED)? trunkDistanceFromTop : deckDistanceFromTop;
		int& disBot = (viewing==ZYUG_VIEWING_UNUSED)? trunkDistanceFromBottom : deckDistanceFromBottom;
		int maxDown = (viewing==ZYUG_VIEWING_UNUSED) ?trunkCards.size()-1 : deckCards.size()-1;
		//std::cout<<"ccD: "<<cPos<<" "<<disTop<<" / "<<disBot<<" "<<maxDown<<std::endl;
		if(cPos>=maxDown || disTop>=maxDown){
			soundUnit.cursorSelectionFail(); return;
		}
		cPos++;
		disBot--; if(disBot<0) disBot = 0;
		disTop++; if(disTop>7) disTop = 7;
		moveMainCursor();
		moveScrollBar();
		soundUnit.cursorMove();
	}

	void Trunk::switchView(){
		if(viewing==ZYUG_VIEWING_UNUSED){
			chain = ZYUG_CH_SWITCHING_DECK;
			glm::vec3 v = ZYUG_SHEET_CENTRE;
			v.x -= ZYUG_SHEET_X_OFF;
			unusedSheet.interpolate(v, 0.5f);
			deckSheet.interpolate(ZYUG_SHEET_CENTRE, 0.5f);
		}else{
			chain = ZYUG_CH_SWITCHING_UNUSED;
			glm::vec3 v = ZYUG_SHEET_CENTRE;
			v.x += ZYUG_SHEET_X_OFF;
			unusedSheet.interpolate(ZYUG_SHEET_CENTRE, 0.5f);
			deckSheet.interpolate(v, 0.5f);
		}
		soundUnit.cursorSelect();
		viewing = ZYUG_VIEWING_NEITHER;
	}

	void Trunk::viewCurrentCard(){
		if( viewing == ZYUG_VIEWING_UNUSED){
			chain = ZYUG_CH_DISPLAY_OVER;
			cardDisplayOver = false;
			cardCreator.setupParents(&trunkCards[trunkCursorPos].c);
			cardDisplayUnit.displayCard( trunkCards[trunkCursorPos].c.bigRender );
		}else{
			if(deckCards.size() > 0){
				chain = ZYUG_CH_DISPLAY_OVER;
				cardDisplayOver = false;
				cardCreator.setupParents(&deckCards[deckCursorPos].c);
				cardDisplayUnit.displayCard( deckCards[deckCursorPos].c.bigRender );
			}else{
				soundUnit.cursorSelectionFail();
			}
		}

	}
	void Trunk::removeCurrentCard(){
		if(deckCards.size()==0){
			soundUnit.cursorSelectionFail();
			return;
		}
		soundUnit.cardSwoosh();
		int no = deckCards[deckCursorPos].c.cardNumber;
		auto iter = find_if(trunkCards.begin(), trunkCards.end(),
							[&](TrunkCard& a)
							{return a.c.cardNumber == no;}
		);
		if(iter != trunkCards.end()){
			(*iter).noInDeck--;
			(*iter).noInTrunk++;
		}else{
			//std::cout<<"TRK: no card found?\n";
		}

		deckCards.erase(deckCards.begin() + deckCursorPos);
		if(deckCursorPos >= deckCards.size()){
			deckCursorPos--;	
		}
		if(deckDistanceFromTop >= deckCards.size()){
			deckDistanceFromTop--;
			deckDistanceFromBottom++;
			if(deckDistanceFromBottom<0) deckDistanceFromBottom = 0;
			if(deckDistanceFromTop<0) deckDistanceFromTop = 0;
		}
		moveMainCursor();
		moveScrollBar();

		for(unsigned int i = 0; i < deckCards.size(); i++){
			if(deckCards[i].c.cardNumber == no){
				deckCards[i].noInDeck = (*iter).noInDeck;
				deckCards[i].noInTrunk = (*iter).noInTrunk;
			}
		}
	}
	void Trunk::addCurrentCard(){
		TrunkCard& t = trunkCards[trunkCursorPos];
		if(deckCards.size() >= 40 || t.noInDeck >= 3 || t.noInTrunk <= 0){
			soundUnit.cursorSelectionFail();
			return;
		}
		soundUnit.cardSwoosh();
		t.noInDeck++;
		t.noInTrunk--;
		if(deckCards.size() <= 0){
			deckCursorPos = 0;
		}
		deckCards.push_back(t);
		for(unsigned int i = 0; i < deckCards.size(); i++){
			if(deckCards[i].c.cardNumber == t.c.cardNumber){
				deckCards[i].noInDeck = t.noInDeck;
				deckCards[i].noInTrunk = t.noInTrunk;
			}
		}
	
	
	}
	void Trunk::exitTrunk(){
		if(deckCards.size()==40){
			soundUnit.cursorSelect();
			chain = ZYUG_CH_END;
		}else{
			soundUnit.cursorSelectionFail();
		}
	}


	void Trunk::moveMainCursor(){
		int& disTop = (viewing==ZYUG_VIEWING_UNUSED)? 
			trunkDistanceFromTop : deckDistanceFromTop;
		glm::vec3 v = ZYUG_MAIN_START_VEC;
		v.y += (ZYUG_MAIN_Y_OFF*disTop);
		mainCursorModel.interpolate(v, 0.2f);
	}
	void Trunk::moveScrollBar(){
		float cPos = (viewing==ZYUG_VIEWING_UNUSED)? trunkCursorPos : deckCursorPos;
		int maxDown = (viewing==ZYUG_VIEWING_UNUSED) ?trunkCards.size()-1 : deckCards.size()-1;
		if(maxDown <= 0){
			scrollBarModel.interpolate(ZYUG_SCROLL_TOP_VEC, 0.2f);
			return;
		}
		float dif = cPos/maxDown;
		float dist = (ZYUG_SCROLL_DIFF) * dif;
		glm::vec3 v = ZYUG_SCROLL_TOP_VEC;
		v.y -= dist;
		scrollBarModel.interpolate(v, 0.2f);
	}
	void Trunk::moveSortCursor(){
		if(viewing==ZYUG_VIEWING_UNUSED){
			glm::vec3 v = ZYUG_SORT_UNUSED_VEC;
			v.x += trunkSortType * ZYUG_SORT_OFF;
			sortCursorModel.interpolate(v, 0.2f);
		}else{
			glm::vec3 v = ZYUG_SORT_DECK_VEC;
			v.x += deckSortType * ZYUG_SORT_OFF;
			sortCursorModel.interpolate(v, 0.2f);
		}
	}

	void Trunk::sortCards(std::vector<TrunkCard>& cards, int type){
		switch(type){
		case ZYUG_SORT_NO:
			sort(cards.begin(), cards.end(),  
				[](TrunkCard& a, TrunkCard& b)
				{return a.c.cardNumber < b.c.cardNumber;}
			);	
			return;
		/*case ZYUG_SORT_NAME:
			sort(cards.begin(), cards.end(), &Screen::Trunk::lessThanName);	
			return;*/
		case ZYUG_SORT_ATK:
			sort(cards.begin(), cards.end(),  
				[](TrunkCard& a, TrunkCard& b)
			{   if(a.c.monMagTrap != YUG_MONSTER_CARD) return false;
				return a.c.attack > b.c.attack;}
			);	
			return;
		case ZYUG_SORT_DEF:
			sort(cards.begin(), cards.end(),  
				[](TrunkCard& a, TrunkCard& b)
				{   if(a.c.monMagTrap != YUG_MONSTER_CARD) return false;
					return a.c.defense > b.c.defense;}
			);	
			return;
		default:
			sort(cards.begin(), cards.end(),  
				[](TrunkCard& a, TrunkCard& b)
				{return a.c.actualType < b.c.actualType;}
			);	
			return;

		}
	}

	void Trunk::update(){
		unusedSheet.update();
		deckSheet.update();
		scrollBarModel.update();
		sortCursorModel.update();
		mainCursorModel.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_CH_START: startUpdate(); break;
			case ZYUG_CH_SWITCHING_UNUSED: switchUpdate(); break;
			case ZYUG_CH_SWITCHING_DECK: switchUpdate(); break;
			case ZYUG_CH_END: endUpdate(); break;
			case ZYUG_CH_DISPLAY_OVER:
				if(cardDisplayOver) cardDisplayOverUpdate();
				break;
			case ZYUG_CH_CLOSED:
				/*closed = true;
				chain = ZYUG_CH_IDLE;
				cleanup();
				nextScreen->toThisScreen();*/
				///
				finalUpdate();///
				break;
			default: break;
			}
		}else{
			continueWaiting();
		}
	}

	void Trunk::finalUpdate(){
		closed = true;
		chain = ZYUG_CH_IDLE;
		cleanup();
		if(miscInfo=='f'){//freeduel
			//std::cout<<"Going to free duel duel\n";
			cleanup();
			screenUnit.winScreen = new FreeDuelScreen();
			theBoard.startup(miscInfo);
		}else if(miscInfo=='r'){//return to freeduel
			cleanup();
			//std::cout<<"returning to free duel screen\n";
			screenUnit.comingScreen = new FreeDuelScreen();
			screenUnit.startUpComingScreen("");
		}else if(miscInfo=='d'){
			cleanup();
			//std::cout<<"Going to campaign duel\n";
			theBoard.startup(miscInfo);
		}
		if(fieldType != 0){
			Card::Forest forest; Card::Sogen sogen; Card::Wasteland waste;
			Card::Mountain mount; Card::Umi umi; Card::Yami yami;
			if(fieldType == 1){
				forest.setNewFieldTexture(); forest.setFieldBoosts();
				forest.setFieldWeakens(); forest.setFieldAmtran();
			}
			if(fieldType == 2){
				waste.setNewFieldTexture(); waste.setFieldBoosts();
				waste.setFieldWeakens(); waste.setFieldAmtran();
			}
			if(fieldType == 3){
				umi.setNewFieldTexture(); umi.setFieldBoosts();
				umi.setFieldWeakens(); umi.setFieldAmtran();
			}
			if(fieldType == 4){
				sogen.setNewFieldTexture(); sogen.setFieldBoosts();
				sogen.setFieldWeakens(); sogen.setFieldAmtran();
			}
			if(fieldType == 5){
				mount.setNewFieldTexture(); mount.setFieldBoosts();
				mount.setFieldWeakens(); mount.setFieldAmtran();
			}
			if(fieldType == 6){
				yami.setNewFieldTexture(); yami.setFieldBoosts();
				yami.setFieldWeakens(); yami.setFieldAmtran();
			}
		}
	}

	void Trunk::startUpdate(){
		chain = ZYUG_CH_IN_USE;
	}

	void Trunk::cardDisplayOverUpdate(){
		chain = ZYUG_CH_IN_USE;
		if( viewing == ZYUG_VIEWING_UNUSED){
			trunkCards[trunkCursorPos].c.cleanup();
			trunkCards[trunkCursorPos].c.pictureTBO = YUG_UNBIND;
		}else{
			deckCards[deckCursorPos].c.cleanup();
			deckCards[deckCursorPos].c.pictureTBO = YUG_UNBIND;
		}
	}

	void Trunk::switchUpdate(){
		viewing = (chain == ZYUG_CH_SWITCHING_UNUSED )?
			ZYUG_VIEWING_UNUSED : ZYUG_VIEWING_DECK;
		chain = ZYUG_CH_IN_USE;
		moveMainCursor();
		moveScrollBar();
		moveSortCursor();
		wait(0.6f);
	}

	void Trunk::endUpdate(){
		writeDeckCards();
		writeTrunkCards();
		fadeUnit.fadeTo(glm::vec4(0.001f,0.0f,0.0f, 1.0f), 1.0f);
		chain = ZYUG_CH_CLOSED;
		wait(0.25f);
		fadeOut(0.2f);
	}

	void Trunk::writeTrunkCards(){
		for(unsigned int i = 0; i < YUG_MAX_NO_OF_CARDS; i++){
			playerData.trunk[i] = 0;
		}
		for(unsigned int i = 0; i < trunkCards.size(); i++){
			playerData.trunk[trunkCards[i].c.cardNumber-100] 
				= trunkCards[i].noInTrunk;
			trunkCards[i].c.cleanup();
		}
	}
	void Trunk::writeDeckCards(){
		for(unsigned int i = 0; i < deckCards.size(); i++){
			playerData.deck.internalDeck[i] = deckCards[i].c.cardNumber;
			deckCards[i].c.cleanup();
		}
	}

	void Trunk::loadTrunkCards(){
		for(unsigned int i = 0; i < YUG_MAX_NO_OF_CARDS; i++){
			if(playerData.trunk[i] != 0){
				TrunkCard t; 
				t.noInTrunk = playerData.trunk[i];
				t.noInDeck = 0;
				trunkCards.push_back(t);
				Card::CardData& ct = trunkCards[trunkCards.size()-1].c;
				ct = cardCreator.createCard(i+100);
				ct.smallRender.parentCard = YUG_UNBIND;
				ct.smallRender.doRender = false;
				ct.bigRender.parentCard = YUG_UNBIND;
				ct.bigRender.doRender = false;
				ct.pictureTBO = YUG_UNBIND;
			}
		}
	}
	void Trunk::loadDeck(){
		for(unsigned int i = 0; i < 40; i++){
			TrunkCard t;
			deckCards.push_back(t);
			Card::CardData& ct = deckCards[deckCards.size()-1].c;
			ct = cardCreator.createCard(playerData.deck.getNoAt(i));
			ct.smallRender.parentCard = YUG_UNBIND;
			ct.smallRender.doRender = false;
			ct.bigRender.parentCard = YUG_UNBIND;
			ct.bigRender.doRender = false;
			ct.pictureTBO = YUG_UNBIND;
		}
	}
	void Trunk::addDeckCardsToTrunk(){
		for(TrunkCard& t  : deckCards){
			auto iter = find_if(trunkCards.begin(), trunkCards.end(),
				[&](TrunkCard& tc)
				{ return tc.c.cardNumber == t.c.cardNumber;}
			);
			if(iter!=trunkCards.end()){
				(*iter).noInDeck++;
			}else{
				TrunkCard nt(0, 1);
				trunkCards.push_back(nt);

				Card::CardData& ct = trunkCards[trunkCards.size()-1].c;
				ct = cardCreator.createCard(t.c.cardNumber);
				ct.smallRender.parentCard = YUG_UNBIND;
				ct.smallRender.doRender = false;
				ct.bigRender.parentCard = YUG_UNBIND;
				ct.bigRender.doRender = false;
				ct.pictureTBO = YUG_UNBIND;
			}
		}
	}
	void Trunk::addTrunkNoToDeck(){
		for(TrunkCard& t  : deckCards){
			auto iter = find_if(trunkCards.begin(), trunkCards.end(),
				[&](TrunkCard& tc)
				{ return tc.c.cardNumber == t.c.cardNumber;}
			);
			if(iter!=trunkCards.end()){
				t.noInDeck = (*iter).noInDeck;
				t.noInTrunk = (*iter).noInTrunk;
			}
		}
	}

	bool Trunk::lessThanName(TrunkCard& cardA, TrunkCard& cardB){
		for(unsigned int i = 0; i < cardA.c.name.size()-2; i++){
			if(i==cardB.c.name.size()-2) return false;
			char xx = cardA.c.name[i];
			char yy = cardB.c.name[i];
			if(xx<yy) return true;
			if(yy<xx) return false;
		}
		if(cardA.c.name.size() == cardB.c.name.size()) return false;
		return true;
	}

	void Trunk::setupSheets(){
		unusedSheet.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/trunkunused.png");
		deckSheet.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/trunkdeck.png");

		unusedSheet.ignoreCamera = true;
		unusedSheet.position = glm::vec3(ZYUG_SHEET_CENTRE);
		unusedSheet.scale = ZYUG_SHEET_SCALE;

		deckSheet.ignoreCamera = true;
		glm::vec3 v = ZYUG_SHEET_CENTRE;
		v.x += ZYUG_SHEET_X_OFF;
		deckSheet.position = v;
		deckSheet.scale = ZYUG_SHEET_SCALE;
	}

	void Trunk::setupScrollBar(){
		scrollBarModel.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/trunkscrollcursor.png");
		scrollBarModel.ignoreCamera = true;
		scrollBarModel.position = ZYUG_SCROLL_TOP_VEC;
		scrollBarModel.scale = ZYUG_SCROLL_SCALE;
	}

	void Trunk::setupSortCursor(){
		sortCursorModel.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/trunksortcursor.png");
		sortCursorModel.ignoreCamera = true;
		sortCursorModel.position = ZYUG_SORT_UNUSED_VEC;
		sortCursorModel.scale = ZYUG_SORT_SCALE;
	}

	void Trunk::setupMainCursor(){
		mainCursorModel.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/trunkcursor.png");
		mainCursorModel.ignoreCamera = true;
		mainCursorModel.position = ZYUG_MAIN_START_VEC;
		mainCursorModel.scale = ZYUG_MAIN_SCALE;
	}
}
