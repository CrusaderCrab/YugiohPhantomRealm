#include <Screens\StorySetter.h>
#include <Screens\ShopScreen.h>
#include <Screens\StoryScene.h>
#include <Game\PlayerData.h>
#include <iostream>
namespace Screen{

	BaseScreen* StorySetter::getNextScreen(int cursorPos){
		if(cursorPos=='b'){ return new ShopScreen(); }
		if(cursorPos<='e'){ return new StoryScene(); }
		if( cursorPos=='h' &&
			playerData.plotUnlockedDuelists[17] &&
				playerData.plotUnlockedDuelists[19] &&
				playerData.plotUnlockedDuelists[21] &&
				playerData.plotUnlockedDuelists[23] &&
				playerData.plotUnlockedDuelists[25] &&
				playerData.currentPlotPoint==YUG_PLOT_POINT_MAGES){
				return new StoryScene();
			}
		if(cursorPos=='h' && 
			(playerData.currentPlotPoint!=YUG_PLOT_POINT_SEEN_MAGES &&
			playerData.currentPlotPoint!=YUG_PLOT_POINT_POST_ESCAPE)){
				return new ShopScreen();
		}
		return new StoryScene();
	}

	std::string StorySetter::getNextFilePath(int cursorPos){
		if(cursorPos=='b') return "t";
		if(cursorPos=='d') return "GameData/Story/ss/16exitNo.txt"; 
		if(cursorPos=='e'){
			if(!playerData.plotUnlockedDuelists[4]){
				return "GameData/Story/ss/13doorGuardStart.txt";
			}else{
				return "GameData/Story/ss/17door.txt";
			}
		}
		if(cursorPos=='a'){
			if(playerData.currentPlotPoint==YUG_PLOT_POINT_NIGHT_BEFORE){
				return "GameData/Story/ss/22cellNightBefore.txt";
			}else if(playerData.currentPlotPoint<YUG_PLOT_POINT_TOURNEY){
				return "GameData/Story/ss/21cellPreTourney.txt";
			}else{
				return "GameData/Story/ss/21cellPreTourney.txt";
			}
		}
		if(cursorPos=='c'){
			if(!playerData.plotUnlockedDuelists[1]){
				return "GameData/Story/ss/07teaStart.txt";
			}else if(!playerData.plotUnlockedDuelists[2] ){
				return "GameData/Story/ss/10jonoStart.txt";
			}else if(!playerData.plotUnlockedDuelists[3] ){
				return "GameData/Story/ss/18FuserStart.txt";
			}else if(playerData.currentPlotPoint==YUG_PLOT_POINT_NIGHT_BEFORE){
				return "GameData/Story/ss/24noDuelsNow.txt";
			}else if(playerData.currentPlotPoint==YUG_PLOT_POINT_FIRST_DAY){
				return "GameData/Story/ss/23tourneyIntro.txt";
			}else if(!playerData.plotUnlockedDuelists[5] ){
				return "GameData/Story/ss/26mikoSrStart.txt";
			}else if(!playerData.plotUnlockedDuelists[6] ){
				return "GameData/Story/ss/29weevilStart.txt";
			}else if(!playerData.plotUnlockedDuelists[7] ){
				return "GameData/Story/ss/32rexStart.txt";
			}else if(!playerData.plotUnlockedDuelists[8] ){
				return "GameData/Story/ss/35maiStart.txt";
			}else if(!playerData.plotUnlockedDuelists[9] ){
				return "GameData/Story/ss/38osirStart.txt";
			}else if(!playerData.plotUnlockedDuelists[10] ){
				return "GameData/Story/ss/41shadiStart.txt";
			}else if(!playerData.plotUnlockedDuelists[11] ){
				return "GameData/Story/ss/44keithStart.txt";
			}else if(!playerData.plotUnlockedDuelists[12] ){
				return "GameData/Story/ss/47bakuraStart.txt";
			}else if(!playerData.plotUnlockedDuelists[13] ){
				return "GameData/Story/ss/50sethaStart.txt";
			}else if(!playerData.plotUnlockedDuelists[14] ){
				return "GameData/Story/ss/53isisStart.txt";
			}else if(!playerData.plotUnlockedDuelists[15] ){
				return "GameData/Story/ss/56pegStart.txt";
			}else {
				return "GameData/Story/ss/13cellGuardStart.txt";
			}
		}
		if(cursorPos=='h'){
			if(playerData.plotUnlockedDuelists[17]  &&
				playerData.plotUnlockedDuelists[19]  &&
				playerData.plotUnlockedDuelists[21]  &&
				playerData.plotUnlockedDuelists[23]  &&
				playerData.plotUnlockedDuelists[25]  &&
				playerData.currentPlotPoint==YUG_PLOT_POINT_MAGES){
				return "GameData/Story/ss/a08townAllItems.txt";
			}
			if(playerData.currentPlotPoint==YUG_PLOT_POINT_POST_ESCAPE){
				return "GameData/Story/ss/62visitTown.txt";
			}
			if(playerData.currentPlotPoint==YUG_PLOT_POINT_SEEN_MAGES){
				return "GameData/Story/ss/63mageDiscuss.txt";
			}else{
				return "w";
			}
		}else if(cursorPos=='i'){
			if(playerData.plotUnlockedDuelists[28] ){
				return "GameData/Story/ss/a20setoReturn.txt";
			}else if(playerData.plotUnlockedDuelists[27] ){
				return "GameData/Story/ss/a15HeishinReturn.txt";
			}else if(playerData.plotUnlockedDuelists[26] ){
				return "GameData/Story/ss/a11magicWin.txt";
			}else if(playerData.currentPlotPoint==YUG_PLOT_POINT_POST_MAGES){
				return "GameData/Story/ss/a09templeStart.txt";
			}else if(playerData.currentPlotPoint<YUG_PLOT_POINT_MAGES){
				return "GameData/Story/ss/69templeReturn1.txt";
			}else{
				return "GameData/Story/ss/70templeReturn2.txt";
			}
		}else if(cursorPos=='j'){//for
			if(playerData.plotUnlockedDuelists[17] ){
				return "GameData/Story/ss/77forestEmpty.txt";
			}else if(playerData.plotUnlockedDuelists[16] ){
				return "GameData/Story/ss/72lowForestWin.txt";
			}else if(playerData.currentPlotPoint<YUG_PLOT_POINT_MAGES){
				return "GameData/Story/ss/64forestBegone.txt";
			}else{
				return "GameData/Story/ss/71lowForest1.txt";
			}
		}else if(cursorPos=='k'){//mea
			//std::cout<<"ss: meadow: "<<playerData.currentPlotPoint<<std::endl;
			if(playerData.plotUnlockedDuelists[23] ){
				return "GameData/Story/ss/84meadowEmpty.txt";
			}else if(playerData.plotUnlockedDuelists[22] ){
				return "GameData/Story/ss/79lowMeadowWin.txt";
			}else if(playerData.currentPlotPoint<YUG_PLOT_POINT_MAGES){
				return "GameData/Story/ss/65meadowBegone.txt";
			}else{
				return "GameData/Story/ss/78lowMeadow.txt";
			}
		}else if(cursorPos=='l'){//mou
			if(playerData.plotUnlockedDuelists[25] ){
				return "GameData/Story/ss/91mountEmpty.txt";
			}else if(playerData.plotUnlockedDuelists[24] ){
				return "GameData/Story/ss/86lowMountWin.txt";
			}else if(playerData.currentPlotPoint<YUG_PLOT_POINT_MAGES){
				return "GameData/Story/ss/66mountainBegone.txt";
			}else{
				return "GameData/Story/ss/85lowMount.txt";
			}
		}else if(cursorPos=='m'){//oce
			if(playerData.plotUnlockedDuelists[21] ){
				return "GameData/Story/ss/98oceanEmpty.txt";
			}else if(playerData.plotUnlockedDuelists[20] ){
				return "GameData/Story/ss/93lowOceanWin.txt";
			}else if(playerData.currentPlotPoint<YUG_PLOT_POINT_MAGES){
				return "GameData/Story/ss/68oceanBegone.txt";
			}else{
				return "GameData/Story/ss/92lowOcean.txt";
			}
		}else if(cursorPos=='n'){//des
			if(playerData.plotUnlockedDuelists[19] ){
				return "GameData/Story/ss/a07desertEmpty.txt";
			}else if(playerData.plotUnlockedDuelists[18] ){
				return "GameData/Story/ss/a02lowDesertWin.txt";
			}else if(playerData.currentPlotPoint<YUG_PLOT_POINT_MAGES){
				return "GameData/Story/ss/67desertBegone.txt";
			}else{
				return "GameData/Story/ss/a01lowDesert.txt";
			}
		}
		return std::string();
	}
}
