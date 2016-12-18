#include <Game\Duel\FusionUnit.h>
#include <Game\Cards\CardCreatorUnit.h>
#include <Utility\Clock.h>
#include <Game\Duel\Board.h>
#include <iostream>
#include <Utility\BlankUnit.h>
#include <Utility\StateUnit.h>
#include <Game\Duel\PositionUnit.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\VectorUnit.h>
#include <Game\Animation\ParticlesUnit.h>
#include <Game\Animation\FadeUnit.h>
#include <Utility\SoundUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <Utility\TextureLoader.h>
#include <Utility\DebugUnit.h>

namespace Duel{

	void FusionUnit::cardPassover(){
		positionUnit.card = cards[cards.size()-1];
		positionUnit.card.faceUp = true;
		cardCreator.setupParents(&(positionUnit.card));
		cards.clear();
		markings.clear();
		chain = YUG_FUS_CH_FINISHED;
		if(boardFusion){
			positionUnit.playMagicFaceDown();
			boardFusion = false;
		}else if(positionUnit.card.monMagTrap == YUG_MONSTER_CARD){
			positionUnit.chain = YUG_POSITION_CH_CONS;
			blankUnit.transitionInput(&(positionUnit),0.2f);
			positionUnit.conPosition();
		}else{
			positionUnit.playingMagic();
		}
		theBoard.currentPlayer->hideHand();
		positionUnit.doRender = true;
	}
	
	void FusionUnit::badStartAnimation(){
		currentCentre = (theBoard.playerControlling()?pos.fusCentre:pos.fusEnCentre);
		priCardStrong = isPriCardStrong();
		saveReturnToPoint();
		saveShuntDirection();
		soundUnit.playOnce("GameData/sounds/fusion/badFusion.wav");
		interpolateBadCardsTogether();
		chain = YUG_FUS_CH_BAD_SHUNT;
		wait(pos.wait[2]);
	}

	void FusionUnit::badShuntAnimation(){
		if(priCardStrong){
			cards[secCard].smallRender.interpolate(shuntDirection,pos.wait[2]);
		}else{
			cards[priCard].smallRender.interpolate(shuntDirection,pos.wait[2]);
		}
		chain = YUG_FUS_CH_BAD_RETURN;
		wait(pos.wait[1]);
	}

	float FusionUnit::getStrongWeakPriZOffset(){
		if(theBoard.playerControlling()){
			return (priCard?pos.weakStrongZOffset:-pos.weakStrongZOffset);
		}
		return (priCard?pos.weakStrongEnZOffset:-pos.weakStrongEnZOffset);
	}
	void FusionUnit::interpolateBadCardsTogether(){
		cards[priCard].smallRender.interpolate(mov.addZOffset(currentCentre, getStrongWeakPriZOffset()), pos.wait[2]);
		cards[secCard].smallRender.interpolate(currentCentre, pos.wait[2]);
		
	}
	void FusionUnit::saveShuntDirection(){
		//std::cout<<"pri v: ";debugUnit.printVec3(cards[priCard].smallRender.position);
		//std::cout<<"\nsec v: ";debugUnit.printVec3(cards[secCard].smallRender.position);
		glm::vec3 v = (cards[priCard].smallRender.position + cards[secCard].smallRender.position);
		//std::cout<<"\nsum v: ";debugUnit.printVec3(v);
		shuntDirection = glm::vec3( -2*(v.x), -2*(v.y), v.z);
		//std::cout<<"\nshunt v: ";debugUnit.printVec3(shuntDirection);
	}

	void FusionUnit::badReturnAnimation(){
		if(priCardStrong){
			cards[priCard].smallRender.interpolate(returnToPoint,pos.wait[3]);
		}else{
			cards[secCard].smallRender.interpolate(returnToPoint,pos.wait[3]);
		}
		chain = YUG_FUS_CH_BAD_END;
		wait(pos.wait[3]);
	}

	void FusionUnit::badEnd(){
		if(priCardStrong){
			cards[secCard].smallRender.cleanup();
			cards[secCard].cleanup();
			cards[secCard] = cards[priCard];
			cardCreator.setupParents(&(cards[secCard]));
			cards[priCard] = cardCreator.blankCard();
		}else{
			cards[priCard].smallRender.cleanup();
			cards[priCard].cleanup();
			cards[priCard] = cardCreator.blankCard();
		}
		
		if(wasDowning){
			markings[secCard].upDown = YUG_MARKINGS_DOWN;
			chain = YUG_FUS_CH_FUSEDOWN;
		}else{
			markings[secCard].upDown = YUG_MARKINGS_UP;
			chain = YUG_FUS_CH_FUSEUP;
		}
		wait(pos.wait[1]);
	}

	void FusionUnit::goodStartAnimation(){
		//std::cout<<"Good fusion animation started\n";
		currentCentre = (theBoard.playerControlling()?pos.fusCentre:pos.fusEnCentre);
		saveReturnToPoint();
		saveStartVectors();
		rotationDuration = 0.0f;
		rotationSpeed = 1/1.0f;
		chain = YUG_FUS_CH_GOOD_ROTATING;
		fadeUnit.changeZ(pos.nc_z[0]);

		centreGlow.doRender = true;
		soundUnit.playOnce("GameData/sounds/fusion/goodFusionFull4.wav");
		centreGlow.scale = pos.fusCentreStartScale;
		centreGlow.amtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		centreGlow.position = currentCentre; centreGlow.position.z += (theBoard.playerControlling()?0.003f:-0.003f);
		particlesShoot = false;
	}

	void FusionUnit::saveStartVectors(){
		priStartV = cards[priCard].smallRender.position;
		secStartV = cards[secCard].smallRender.position;

	}

	void FusionUnit::goodRotationAnimation(){
		float cl = gameClock.lastLoopTime()*rotationSpeed;
		rotationDuration += cl;
		float amtran = 1 + rotationDuration*3;
		if(rotationDuration < 1.0f){
			cards[priCard].smallRender.position = (1-rotationDuration)*mov.rotateVectorZ(priStartV, 9.424f*rotationDuration);
			cards[priCard].smallRender.amtran = glm::vec4(amtran,amtran,amtran,1.0f);
			cards[secCard].smallRender.position = (1-rotationDuration)*mov.rotateVectorZ(secStartV, 9.424f*rotationDuration);
			cards[secCard].smallRender.amtran = glm::vec4(amtran,amtran,amtran,1.0f);
			growOrb(rotationDuration);
		}else{
			chain = YUG_FUS_CH_GOOD_FADE_IN;
			//std::cout<<"rotation done\n";
		}
	}

	void FusionUnit::growOrb(float time){
		centreGlow.scale = 2*time*pos.fusCentreStartScale;
		float tran = (time < 0.5? time*2 : 1.0f);
		centreGlow.amtran.w = tran;
	}

	void FusionUnit::goodFadeInAnimation(){
		fadeUnit.sheet.amtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		fadeUnit.fadeTo(YUG_FADE_WHITE,0.005f);
		chain = YUG_FUS_CH_GOOD_FADE_OUT;
		wait(0.005f);
	}

	void FusionUnit::goodFadeOutAnimation(){
		fadeUnit.fadeTo(YUG_FADE_CLEAR,0.05f);
		//fadeUnit.doRender = false;
		particleUnit.particleBang(glm::vec3(0.0f,0.0f,pos.nc_z[6]),0.2f);
		//droopy particles
		//glow fade
		centreGlow.interpolateAmtran(YUG_FADE_CLEAR,0.01f);
		centreGlow.scaleInterpolate(glm::vec3(0.001f,0.001f,0.001f),0.001f);
		//centreGlow.doRender = false;
		cards[priCard].smallRender.doRender = false;
		cards[secCard].smallRender.doRender = false;
		setupNewCard();
		setupMassiveCard();
		chain = YUG_FUS_CH_GOOD_CARD_REVEAL1;
		wait(0.15f);
	}

	void FusionUnit::setupNewCard(){
		cardCreator.setupParents(&newCard);
		newCard.smallRender.startup();
		newCard.smallRender.ignoreCamera = false;
		newCard.bigRender.startup();
		newCard.bigRender.ignoreCamera = false;
		newCard.bigRender.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		newCard.smallRender.position = currentCentre;
		float zOff = (theBoard.playerControlling()?0.2f:-0.2f);
		newCard.bigRender.position = mov.addZOffset(currentCentre,zOff);//ori offset:0.0015f
		newCard.smallRender.scale = pos.bCardScale;
		newCard.bigRender.scale = pos.fusBigCardScale;
		if(theBoard.playerControlling()){
			newCard.bigRender.rotationMatrix = pos.cardPBackLeft;
			newCard.smallRender.rotationMatrix = pos.cardPBackLeft;
		}else{
			newCard.bigRender.rotationMatrix = pos.cardEBackLeft;
			newCard.smallRender.rotationMatrix = pos.cardEBackLeft;
		}
		//std::cout<<"Big Card "<<newCard.bigRender.position.x<<" "<<newCard.bigRender.position.y<<" "<<newCard.bigRender.position.z<<std::endl;
	}
	void FusionUnit::setupMassiveCard(){
		massiveCard = newCard.bigRender;
		massiveCard.ignoreCamera = true;
		massiveCard.position = pos.fusMassiveCardPosition;
		massiveCard.scale = pos.fusMassiveCardScale;
		massiveCard.rotationMatrix = pos.atkFaceupUp;
		massiveCard.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		massiveCard.doRender = false;
	}

	void FusionUnit::goodCardReveal1Animation(){
		renderNewCard = true;
		newCard.bigRender.doRender = true;

		if(theBoard.playerControlling()){
			newCard.bigRender.rotate(pos.atkFaceupUp, 0.2f);
		}else{
			newCard.bigRender.rotate(pos.eAtkFaceupUp, 0.2f);
		}
		renderMassiveCard = true;
		massiveCard.doRender = true;
		massiveCard.interpolate(mov.addYOffset(pos.fusMassiveCardPosition,pos.fusMassiveCardYOffset),1.5f);
		chain = YUG_FUS_CH_GOOD_CARD_FB1;
		wait(1.0f);
	}

	void FusionUnit::goodCardFB1Animation(){
		if(theBoard.playerControlling()){
			newCard.bigRender.rotate(pos.cardPBackRight, pos.wait[2]);
		}else{
			newCard.bigRender.rotate(pos.cardEBackRight, pos.wait[2]);
		}
		centreGlow.doRender = false;
		chain = YUG_FUS_CH_GOOD_CARD_FB2;
		wait(pos.wait[2]);
	}

	void FusionUnit::goodCardFB2Animation(){
		newCard.bigRender.doRender = false;
		newCard.smallRender.doRender = true;
		if(theBoard.playerControlling()){
			newCard.smallRender.rotate(pos.atkFaceupUp, pos.wait[2]);
		}else{
			newCard.smallRender.rotate(pos.eAtkFaceupUp, pos.wait[2]);
		}
		chain = YUG_FUS_CH_GOOD_RETURN;
		wait(pos.wait[3]);
	}

	void FusionUnit::goodCardReturnAnimation(){
		massiveCard.doRender = false;
		renderMassiveCard = false;
		newCard.smallRender.interpolate(returnToPoint, pos.wait[2]);
		chain = YUG_FUS_CH_GOOD_END;
		wait(pos.wait[3]);
	}

	void FusionUnit::goodEnd(){
		cards[secCard].smallRender.cleanup();
		cards[secCard].cleanup();
		cards[priCard].smallRender.cleanup();
		cards[priCard].cleanup();
		cards[priCard] = cardCreator.blankCard();
		cards[secCard] = newCard;
		cardCreator.setupParents(&(cards[secCard]));
		newCard = cardCreator.blankCard();
		newCard.smallRender.doRender = false;
		newCard.bigRender.doRender = false;
		renderNewCard = false;
		if(wasDowning){
			markings[secCard].upDown = YUG_MARKINGS_DOWN;
			chain = YUG_FUS_CH_FUSEDOWN;
		}else{
			markings[secCard].upDown = YUG_MARKINGS_UP;
			chain = YUG_FUS_CH_FUSEUP;
		}
		wait(pos.wait[1]);

	}

	bool FusionUnit::isPriCardStrong(){
		if(cards[secCard].monMagTrap == YUG_MONSTER_CARD)
			return false;
		if(cards[priCard].monMagTrap != YUG_MONSTER_CARD)
			return false;
		return true;
	}

	void FusionUnit::saveReturnToPoint(){
		returnToPoint = cards[secCard].smallRender.position;
		//std::cout<<"--------return point: --------\n";debugUnit.printVec3(returnToPoint);
	}

	void FusionUnit::interpolateCardsToStartPositions(){
		int noOfCards = markings.size();
		switch(noOfCards){
		case 2:
			twoCardInterpolate();
			break;
		case 3:
			threeCardInterpolate();
			break;
		case 4:
			fourCardInterpolate();
			break;
		case 5:
			fiveCardInterpolate();
			break;
		default:
			sixCardInterpolate();
		}
	}
	void FusionUnit::rotateCardsToStartPosition(){
		for(unsigned int i = 0; i <cards.size(); i++){
			if(theBoard.playerControlling()){
				cards[i].smallRender.rotate(pos.atkFaceupUp,pos.wait[2]);
			}else{
				cards[i].smallRender.rotate(pos.eAtkFaceupUp,pos.wait[2]);
			}
		}
	}

	void FusionUnit::equipStartAnimation(){
		//std::cout<<"Good fusion animation started\n";
		currentCentre = (theBoard.playerControlling()?pos.fusCentre:pos.fusEnCentre);
		saveReturnToPoint();
		saveStartVectors();
		rotationDuration = 0.0f;
		rotationSpeed = 1/1.0f;
		chain = YUG_FUS_CH_EQ_PULL_IN;
		particlesShoot = false;
		if(theBoard.playerControlling()){
			endUnit.noOfMagicsUsed++;
			endUnit.noOfMagicsPlayed++;
		}
	}

	void FusionUnit::equipPullInAnimation(){
		soundUnit.playOnce("GameData/sounds/fusion/equipFusion.wav");
		float x = (theBoard.playerControlling()?0.4f:-0.4f);
		float z = x*0.05f;
		glm::vec3 mPos = mov.addXOffset(currentCentre, -x);
		glm::vec3 ePos = mov.addXOffset(currentCentre, x);
		cards[equipMonster].smallRender.interpolate(
			mov.addZOffset(mPos,z),0.3f);
		cards[equipCard].smallRender.interpolate(
			mov.addZOffset(ePos,-z),0.3f);
		wait(0.4f);
		chain = YUG_FUS_CH_EQ_FIRST_SLIDE;
	}

	void FusionUnit::equipFirstSlideAnimation(){
		float x = (theBoard.playerControlling()?0.1f:-0.1f);
		float z = x*0.05f;
		glm::vec3 mP = mov.addXOffset(currentCentre,x);
		glm::vec3 eP = mov.addXOffset(currentCentre,-x);
		cards[equipMonster].smallRender.interpolate(
			mov.addZOffset(mP,z),0.2f);
		cards[equipCard].smallRender.interpolate(
			mov.addZOffset(eP,-z),0.2f);
		chain = YUG_FUS_CH_EQ_SECOND_SLIDE;
		wait(0.2f);
	}
	
	void FusionUnit::equipSecondSlideAnimation(){
		float z = (theBoard.playerControlling()?-0.02f:0.02f);
		cards[equipMonster].smallRender.interpolate(
			currentCentre,0.2f);
		cards[equipCard].smallRender.interpolate(
			mov.addZOffset(currentCentre,z),0.2f);
		chain = YUG_FUS_CH_EQ_GLOW1;
		wait(0.2f);
	}

	void FusionUnit::equipGlow1Animation(){
		cards[equipCard].smallRender.doRender = false;
		textureLoader.deleteTexture(&centreGlow.textureBO);
		textureLoader.loadTexture("GameData/textures/particles/equipglow.png",
			&centreGlow.textureBO);
		centreGlow.position = cards[equipCard].smallRender.position;
		centreGlow.position.y+=0.015f;
		centreGlow.scale = glm::vec3(0.3f,0.4f,1.0f);
		centreGlow.amtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		centreGlow.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,2.0f),0.3f);
		centreGlow.doRender = true;
		wait(0.3f);
		chain = YUG_FUS_CH_EQ_GLOW2;
	}
	void FusionUnit::equipGlow2Animation(){
		textureLoader.deleteTexture(&centreGlow.textureBO);
		textureLoader.loadTexture("GameData/textures/particles/equipglow.png",
			&centreGlow.textureBO);
		centreGlow.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,0.0f),0.15f);
		centreGlow.doRender = true;
		wait(0.2f);
		chain = YUG_FUS_CH_EQ_CARD_REVEAL1;
	}


	void FusionUnit::equipSetUpBigCard(){
		cards[equipMonster].bigRender.startup();
		cards[equipMonster].bigRender.ignoreCamera = false;
		cards[equipMonster].bigRender.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		cards[equipMonster].smallRender.position = currentCentre;
		cards[equipMonster].bigRender.position = mov.addZOffset(currentCentre,0.0015f);
		cards[equipMonster].smallRender.scale = pos.bCardScale;
		cards[equipMonster].bigRender.scale = pos.fusBigCardScale;
		if(theBoard.playerControlling()){
			cards[equipMonster].bigRender.rotationMatrix = pos.cardPBackLeft;
			cards[equipMonster].smallRender.rotationMatrix = pos.cardPBackLeft;
		}else{
			cards[equipMonster].bigRender.rotationMatrix = pos.cardEBackLeft;
			cards[equipMonster].smallRender.rotationMatrix = pos.cardEBackLeft;
		}
	}

	void FusionUnit::equipCardReveal1Animation(){
		centreGlow.doRender = false;
		textureLoader.deleteTexture(&centreGlow.textureBO);
		textureLoader.loadTexture("GameData/textures/particles/glowingorb2.png",
				&centreGlow.textureBO);
		if(theBoard.playerControlling()){
			cards[equipMonster].smallRender.rotate(pos.cardPBackRight, 0.3f);
		}else{
			cards[equipMonster].smallRender.rotate(pos.cardEBackRight, 0.3f);
		}
		chain = YUG_FUS_CH_EQ_CARD_REVEAL2;
		wait(0.3f);
	}

	void FusionUnit::equipCardReveal2Animation(){
		equipSetUpBigCard();
		equiping = true;
		boostStats();
		cards[equipMonster].bigRender.doRender = true;
		cards[equipMonster].smallRender.doRender = false;
		if(theBoard.playerControlling()){
			cards[equipMonster].bigRender.rotate(pos.atkFaceupUp, 0.2f);
		}else{
			cards[equipMonster].bigRender.rotate(pos.eAtkFaceupUp, 0.2f);
		}
		chain = YUG_FUS_CH_EQ_CARD_FB1;
		wait(1.5f);
	}

	void FusionUnit::boostStats(){
		if(cards[equipCard].cardNumber == 799){
			int increase = 1000;
			cards[equipMonster].bigRender.emphasis(YUG_BIG_CARD_EMPHASIS_BOTH);
			cards[equipMonster].bigRender.countStatUp(increase, 1.0f);
			cards[equipMonster].alterAttack(increase);
			cards[equipMonster].alterDefense(increase);
			cards[equipMonster].atkStatBoost += 1000;
			cards[equipMonster].defStatBoost += 1000;
		}else{
			int increase = 500;
			cards[equipMonster].bigRender.emphasis(YUG_BIG_CARD_EMPHASIS_BOTH);
			cards[equipMonster].bigRender.countStatUp(increase, 1.0f);
			cards[equipMonster].alterAttack(increase);
			cards[equipMonster].alterDefense(increase);
			cards[equipMonster].atkStatBoost += 500;
			cards[equipMonster].defStatBoost += 500;
		}
	}

	void FusionUnit::equipCardFB1Animation(){
		if(theBoard.playerControlling()){
			cards[equipMonster].bigRender.rotate(pos.cardPBackRight, pos.wait[2]);
		}else{
			cards[equipMonster].bigRender.rotate(pos.cardEBackRight, pos.wait[2]);
		}
		chain = YUG_FUS_CH_EQ_CARD_FB2;
		wait(pos.wait[2]);
	}

	void FusionUnit::equipCardFB2Animation(){
		equiping = false;
		cards[equipMonster].bigRender.doRender = false;
		cards[equipMonster].bigRender.cleanup();
		cards[equipMonster].smallRender.doRender = true;
		if(theBoard.playerControlling()){
			cards[equipMonster].smallRender.rotate(pos.atkFaceupUp, pos.wait[2]);
		}else{
			cards[equipMonster].smallRender.rotate(pos.eAtkFaceupUp, pos.wait[2]);
		}
		chain = YUG_FUS_CH_EQ_RETURN;
		wait(pos.wait[3]);
	}

	void FusionUnit::equipCardReturnAnimation(){
		cards[equipMonster].smallRender.interpolate(returnToPoint, pos.wait[2]);
		chain = YUG_FUS_CH_EQ_END;
		wait(pos.wait[3]);
	}

	void FusionUnit::equipEnd(){
		if(equipMonster == priCard){
			cards[secCard].smallRender.cleanup();
			cards[secCard].cleanup();
			cards[secCard] = cards[priCard];
			cardCreator.setupParents(&(cards[secCard]));
			cards[priCard] = cardCreator.blankCard();
		}else{
			cards[priCard].smallRender.cleanup();
			cards[priCard].cleanup();
			cards[priCard] = cardCreator.blankCard();
		}
		if(wasDowning){
			markings[secCard].upDown = YUG_MARKINGS_DOWN;
			chain = YUG_FUS_CH_FUSEDOWN;
		}else{
			markings[secCard].upDown = YUG_MARKINGS_UP;
			chain = YUG_FUS_CH_FUSEUP;
		}
		wait(pos.wait[1]);
	}

	void FusionUnit::animationUpdates(){
		switch(chain){

		case YUG_FUS_CH_BAD_START:
			badStartAnimation();
			break;
		case YUG_FUS_CH_BAD_SHUNT:
			badShuntAnimation();
			break;
		case YUG_FUS_CH_BAD_RETURN:
			badReturnAnimation();
			break;
		case YUG_FUS_CH_BAD_END:
			badEnd();
			break;
		case YUG_FUS_CH_GOOD_START:
			goodStartAnimation();
			break;
		case YUG_FUS_CH_GOOD_ROTATING:
			goodRotationAnimation();
			break;
		case YUG_FUS_CH_GOOD_FADE_IN:
			goodFadeInAnimation();
			break;
		case YUG_FUS_CH_GOOD_FADE_OUT:
			goodFadeOutAnimation();
			break;
		case YUG_FUS_CH_GOOD_CARD_REVEAL1:
			goodCardReveal1Animation();
			break;
		case YUG_FUS_CH_GOOD_CARD_REVEAL2:
			break;
		case YUG_FUS_CH_GOOD_CARD_FB1:
			goodCardFB1Animation();
			break;
		case YUG_FUS_CH_GOOD_CARD_FB2:
			goodCardFB2Animation();
			break;
		case YUG_FUS_CH_GOOD_RETURN:
			goodCardReturnAnimation();
			break;
		case YUG_FUS_CH_GOOD_END:
			goodEnd();
			break;

		case YUG_FUS_CH_EQUIP_START:
			equipStartAnimation();
			break;
		case YUG_FUS_CH_EQ_PULL_IN:
			equipPullInAnimation();
			break;
		case YUG_FUS_CH_EQ_FIRST_SLIDE:
			equipFirstSlideAnimation();
			break;
		case YUG_FUS_CH_EQ_SECOND_SLIDE:
			equipSecondSlideAnimation();
			break;
		case YUG_FUS_CH_EQ_GLOW1:
			equipGlow1Animation();
			break;
		case YUG_FUS_CH_EQ_GLOW2:
			equipGlow2Animation();
			break;
		case YUG_FUS_CH_EQ_CARD_REVEAL1:
			equipCardReveal1Animation();
			break;
		case YUG_FUS_CH_EQ_CARD_REVEAL2:
			equipCardReveal2Animation();
			break;
		case YUG_FUS_CH_EQ_CARD_FB1:
			equipCardFB1Animation();
			break;
		case YUG_FUS_CH_EQ_CARD_FB2:
			equipCardFB2Animation();
			break;
		case YUG_FUS_CH_EQ_RETURN:
			equipCardReturnAnimation();
			break;
		case YUG_FUS_CH_EQ_END:
			equipEnd();
			break;

		default:
			break;
		}
	}

	void FusionUnit::twoCardInterpolate(){
		if(theBoard.playerControlling()){
			cards[0].smallRender.interpolate(pos.fusPos1,pos.wait[2]);
			cards[1].smallRender.interpolate(pos.fusPos2,pos.wait[2]);
		}else{
			cards[0].smallRender.interpolate(pos.fusEnPos1,pos.wait[2]);
			cards[1].smallRender.interpolate(pos.fusEnPos2,pos.wait[2]);
		}
	}
	void FusionUnit::threeCardInterpolate(){
		if(theBoard.playerControlling()){
			cards[0].smallRender.interpolate(pos.fusPos3,pos.wait[2]);
			cards[1].smallRender.interpolate(pos.fusPos4,pos.wait[2]);
			cards[2].smallRender.interpolate(pos.fusPos5,pos.wait[2]);
		}else{
			cards[0].smallRender.interpolate(pos.fusEnPos3,pos.wait[2]);
			cards[1].smallRender.interpolate(pos.fusEnPos4,pos.wait[2]);
			cards[2].smallRender.interpolate(pos.fusEnPos5,pos.wait[2]);
		}
	}
	void FusionUnit::fourCardInterpolate(){
		if(theBoard.playerControlling()){
			cards[0].smallRender.interpolate(pos.fusPos13,pos.wait[2]);//3
			cards[1].smallRender.interpolate(pos.fusPos8,pos.wait[2]);//1
			cards[2].smallRender.interpolate(pos.fusPos9,pos.wait[2]);//6
			cards[3].smallRender.interpolate(pos.fusPos14,pos.wait[2]);//2
		}else{
			cards[0].smallRender.interpolate(pos.fusEnPos13,pos.wait[2]);
			cards[1].smallRender.interpolate(pos.fusEnPos8,pos.wait[2]);
			cards[2].smallRender.interpolate(pos.fusEnPos9,pos.wait[2]);
			cards[3].smallRender.interpolate(pos.fusEnPos14,pos.wait[2]);
		}
	}
	void FusionUnit::fiveCardInterpolate(){
		if(theBoard.playerControlling()){
			cards[0].smallRender.interpolate(pos.fusPos3,pos.wait[2]);
			cards[1].smallRender.interpolate(pos.fusPos7,pos.wait[2]);
			cards[2].smallRender.interpolate(pos.fusPos8,pos.wait[2]);
			cards[3].smallRender.interpolate(pos.fusPos9,pos.wait[2]);
			cards[4].smallRender.interpolate(pos.fusPos10,pos.wait[2]);
		}else{
			cards[0].smallRender.interpolate(pos.fusEnPos3,pos.wait[2]);
			cards[1].smallRender.interpolate(pos.fusEnPos7,pos.wait[2]);
			cards[2].smallRender.interpolate(pos.fusEnPos8,pos.wait[2]);
			cards[3].smallRender.interpolate(pos.fusEnPos9,pos.wait[2]);
			cards[4].smallRender.interpolate(pos.fusEnPos10,pos.wait[2]);
		}
	}
	void FusionUnit::sixCardInterpolate(){
		if(theBoard.playerControlling()){
			cards[0].smallRender.interpolate(pos.fusPos13,pos.wait[2]);//3
			cards[1].smallRender.interpolate(pos.fusPos1,pos.wait[2]);//7
			cards[2].smallRender.interpolate(pos.fusPos8,pos.wait[2]);//11
			cards[3].smallRender.interpolate(pos.fusPos9,pos.wait[2]);//6
			cards[4].smallRender.interpolate(pos.fusPos2,pos.wait[2]);//12
			cards[5].smallRender.interpolate(pos.fusPos14,pos.wait[2]);//10
		}else{
			cards[0].smallRender.interpolate(pos.fusEnPos13,pos.wait[2]);
			cards[1].smallRender.interpolate(pos.fusEnPos1,pos.wait[2]);
			cards[2].smallRender.interpolate(pos.fusEnPos8,pos.wait[2]);
			cards[3].smallRender.interpolate(pos.fusEnPos9,pos.wait[2]);
			cards[4].smallRender.interpolate(pos.fusEnPos2,pos.wait[2]);
			cards[5].smallRender.interpolate(pos.fusEnPos14,pos.wait[2]);
		}

	}



	void FusionUnit::initializeCentreGlow(){
		centreGlow.startup(YUG_PLANE_FILE_PATH, "GameData/textures/particles/glowingorb.png");
		centreGlow.doRender = false;
		centreGlow.ignoreCamera = false;
		centreGlow.amtran = glm::vec4(1.0f,1.0f,1.0f,0.0f);
		centreGlow.scale = pos.fusCentreStartScale;
		centreGlow.position = pos.fusCentre;
	}

	void FusionUnit::initializeCards(){
		newCard = cardCreator.blankCard();
		massiveCard = Card::BigCardRender();
		newCard.smallRender.doRender = false;
		newCard.bigRender.doRender = false;
		massiveCard.doRender = false;
		renderNewCard = false;
		renderMassiveCard = false;
	}

}