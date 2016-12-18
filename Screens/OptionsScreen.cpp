#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Utility\InputUnit.h>
#include <Utility\SoundUnit.h>
#include <Utility\TextPrinter.h>
#include <Utility\ConfigUnit.h>
#include <Screens\ScreenUnit.h>
#include <Screens\OptionsScreen.h>
#include <Screens\ShopScreen.h>
#include <Screens\MainMenu.h>
#include <DefinesAndTypedefs.h>
#include <Utility\SoundUnit.h>

#include <iostream>

#define ZYUG_CH_IDLE 0
#define ZYUG_CH_START 1
#define ZYUG_CH_ACTIVE 2
#define ZYUG_CH_END 3
#define ZYUG_CURSOR_START glm::vec3(0,0.303f,0.011f)
#define ZYUG_CURSOR_Y_OFF 0.067f;
#define ZYUG_EFFECT_START glm::vec3(0.55f,0.17f,0.01f)
#define ZYUG_MUSIC_START glm::vec3(0.55f,0.236f,0.01f)
#define ZYUG_ES_X -0.39f
#define ZYUG_KEYS_POS glm::vec3(0.0f, 0.12f, -1.999f)
#define ZYUG_KEYS_Y -0.067f;

namespace Screen{
 
	void OptionsScreen::startup(const char* fileLocal){
		BaseScreen::startup();
		fromMainMenu = (fileLocal[0]=='M');
		cursorPos = 0;
		effectPos = soundUnit.effectsOn;
		musicPos = soundUnit.musicOn;
		setupBG();
		setupCursor();
		setupMarkers();
		chain = ZYUG_CH_START;
		wait(0.2f);
		soundUnit.playLoop("GameData/sounds/music/ready/trunk_o.wav");
	}

	void OptionsScreen::cleanup(){
		bg.cleanup();
		cursor.cleanup();
		effectMarker.cleanup();
		musicMarker.cleanup();
		soundUnit.stopAll();
	}

	void OptionsScreen::input(){
		if(!isWaiting){
			if(inputUnit.isKeyActive(YUG_KEY_DOWN)){
				cursorPos++;
				if(cursorPos>12) cursorPos=12;
				updateCursorPos();
				soundUnit.cursorMove();
				wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_UP)){
				cursorPos--;
				if(cursorPos<0)cursorPos=0;
				updateCursorPos();
				soundUnit.cursorMove();
				wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_LEFT)){
				sidePressed(true); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_RIGHT)){
				sidePressed(false); wait(0.2f);
			}else if(inputUnit.isKeyActive(YUG_KEY_X)){
				xPressed(); 
			}else if(inputUnit.isKeyActive(YUG_KEY_O)){
				oPressed();
			}
		}
	}

	void OptionsScreen::xPressed(){
		if(cursorPos==0){
			soundUnit.cursorSelect();
			inputUnit.resetAllKeys();
			wait(0.3f); return;
		}
		if(cursorPos > 2){
			soundUnit.cursorSelect();
			if(cursorPos==10){inputUnit.resetKey(YUG_KEY_START);}
			else if(cursorPos==11){inputUnit.resetKey(YUG_KEY_R1);}
			else if(cursorPos==12){inputUnit.resetKey(YUG_KEY_L1);}
			else inputUnit.resetKey(cursorPos-3);
			wait(0.5f);
		}
	}

	void OptionsScreen::oPressed(){
		soundUnit.cursorCancel();
		fadeOut(0.2f);
		wait(0.21f);
		chain = ZYUG_CH_END;
	}

	void OptionsScreen::toNextScreen(){
		//std::cout<<"OptionsScreen: toNextScreen()";
		cleanup();
		screenUnit.comingScreen = (BaseScreen*)nextScreen;
		screenUnit.startUpComingScreen(nextScreenFile.c_str());
		if(fromMainMenu){ 
			((MainMenu*)nextScreen)->cursorPosition = 2;
			((MainMenu*)nextScreen)->updateButtons();
		}
	}

	void OptionsScreen::sidePressed(bool left){
		if(cursorPos==2){
			soundUnit.cursorMove();
			effectPos += (left?1:-1);
			if(effectPos<0)effectPos = 0;
			if(effectPos>1)effectPos = 1;
			float addedX = (effectPos?ZYUG_ES_X:0);

			effectMarker.position = bg.position + ZYUG_EFFECT_START;
			effectMarker.position.x += addedX;
			soundUnit.effectsOn = (bool)effectPos;
		}
		if(cursorPos==1){
			soundUnit.cursorMove();
			musicPos += (left?1:-1);
			if(musicPos<0)musicPos = 0;
			if(musicPos>1)musicPos = 1;
			float addedX = (musicPos?ZYUG_ES_X:0);

			musicMarker.position = bg.position + ZYUG_MUSIC_START;
			musicMarker.position.x += addedX;
			soundUnit.musicOn = (bool)musicPos;
		}
	}

	void OptionsScreen::updateCursorPos(){
		glm::vec3 posi = bg.position + ZYUG_CURSOR_START;
		posi.y -= cursorPos*ZYUG_CURSOR_Y_OFF;
		//cursor.interpolate(posi,0.2f);
		cursor.position = posi;
	}
	void OptionsScreen::update(){
		bg.update();
		cursor.update();
		effectMarker.update();
		musicMarker.update();
		if(!isWaiting){
			switch(chain){
			case ZYUG_CH_START: firstUpdate(); break;
			case ZYUG_CH_END: finalUpdate(); break;
			default: break;
			}
		}else{
			continueWaiting();
		}
	}

	void OptionsScreen::firstUpdate(){
		beginScreen(0.02f);
		chain = ZYUG_CH_ACTIVE;
		wait(0.2f);
	}

	void OptionsScreen::finalUpdate(){
		Utility::ConfigUnit config;
		config.saveConfig();
		if(fromMainMenu){
			nextScreen = new MainMenu();
		}else{
			//std::cout<<"PLEASE IMPLEMENT OPTIONS BACK TO SHOP MENU\n";
			nextScreen = new ShopScreen();
		}
		chain = ZYUG_CH_IDLE;
		toNextScreen();
	}

	void OptionsScreen::render(){
		bg.render();
		printKeyValues();
		effectMarker.render();
		musicMarker.render();
		cursor.render();
	}

	void OptionsScreen::printKeyValues(){
		textPrinter.ignoreCamera = true;
		textPrinter.leftAlign = true;
		glm::vec3 currentPos = ZYUG_KEYS_POS;
		for(int i = 0; i < 7; i++){
			textPrinter.printText(
				kn[inputUnit.realKeyBindings[i]],
				YUG_TEXT_INFO_FONT,
				glm::vec3(0.3f,0.5f,1.0f),
				currentPos, glm::mat4()
			);
			currentPos.y += ZYUG_KEYS_Y;
		}
		textPrinter.printText(
			kn[inputUnit.realKeyBindings[YUG_KEY_START]],
				YUG_TEXT_INFO_FONT,
				glm::vec3(0.3f,0.5f,1.0f),
				currentPos, glm::mat4()
			);
			currentPos.y += ZYUG_KEYS_Y;
		textPrinter.printText(
			kn[inputUnit.realKeyBindings[YUG_KEY_R1]],
				YUG_TEXT_INFO_FONT,
				glm::vec3(0.3f,0.5f,1.0f),
				currentPos, glm::mat4()
			);
			currentPos.y += ZYUG_KEYS_Y;
		textPrinter.printText(
			kn[inputUnit.realKeyBindings[YUG_KEY_L1]],
				YUG_TEXT_INFO_FONT,
				glm::vec3(0.3f,0.5f,1.0f),
				currentPos, glm::mat4()
			);
			currentPos.y += ZYUG_KEYS_Y;
	}

	void OptionsScreen::setupBG(){
		bg.startup(YUG_PLANE_FILE_PATH, 
			"GameData/textures/screens/Menu/optionsScreen2.png");
		bg.doRender = true;
		bg.ignoreCamera = true;
		bg.scale = glm::vec3(0.82f, 0.63f, 1);
		bg.position = glm::vec3(0.0f, 0.02f, -2.0f);
	}

	void OptionsScreen::setupCursor(){
		cursor.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/models/positionunit/concursor.png");
		cursor.doRender = true;
		cursor.ignoreCamera = true;
		cursor.amtran = glm::vec4(1,1,1,0.5f);
		cursor.scale = glm::vec3(0.718f, 0.04f, 1);
		cursor.position = bg.position + glm::vec3(0,0.303f,0.011f);
	}

	void OptionsScreen::setupMarkers(){
		musicMarker.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/models/positionunit/conmarker.png");
		musicMarker.doRender = true;
		musicMarker.ignoreCamera = true;
		musicMarker.scale = glm::vec3(0.065f, 0.032f, 1);
		musicMarker.amtran = glm::vec4(1,1,1,0.5f);
		musicMarker.position = bg.position + glm::vec3(0.55f,0.236f,0.01f);
		musicMarker.position.x += ((int)soundUnit.musicOn)*-0.39f;

		effectMarker.startup(YUG_PLANE_FILE_PATH,
			"GameData/textures/models/positionunit/conmarker.png");
		effectMarker.doRender = true;
		effectMarker.ignoreCamera = true;
		effectMarker.scale = glm::vec3(0.065f, 0.032f, 1);
		effectMarker.amtran = glm::vec4(1,1,1,0.5f);
		effectMarker.position = bg.position + glm::vec3(0.55f,0.17f,0.01f);
		effectMarker.position.x += ((int)soundUnit.effectsOn)*-0.39f;

	}

	const char* OptionsScreen::kn[256];

	void OptionsScreen::setupNames(){
		kn[  0] = "0x00  ";			kn[  10] = "10  ";		kn[  20] = "Caps Lock";		kn[ 30] = "0x1e  ";			kn[ 40] = "Down  ";
		kn[  1] = "Left Mouse  ";	kn[  11] = "11  ";		kn[  21] = "Hanguel  ";		kn[ 31] = "0x1f  ";			kn[ 41] = "Select  ";
		kn[  2] = "Right Mouse  ";	kn[  12] = "Clear  ";	kn[  22] = "22    ";		kn[ 32] = "Space  ";			kn[ 42] = "Print  ";
		kn[  3] = "3  ";			kn[  13] = "Enter  ";	kn[  23] = "Junja  ";		kn[ 33] = "Page Up  ";			kn[ 43] = "Execute  ";
		kn[  4] = "Middle Mouse  ";	kn[  14] = "14  ";		kn[  24] = "Final  ";		kn[ 34] = "Page Down  ";		kn[ 44] = "0x2c  ";
		kn[  5] = "5  ";			kn[  15] = "15  ";		kn[  25] = "Hanja  ";		kn[ 35] = "End ";			kn[ 45] = "Insert  ";
		kn[  6] = "6  ";			kn[  16] = "Shift  ";	kn[  26] = "26  ";		kn[ 36] = "Home ";			kn[ 46] = "Delete ";
		kn[  7] = "7  ";			kn[  17] = "Ctrl  ";	kn[  27] = "ESC  ";		kn[ 37] = "Left ";			kn[ 47] = "Help ";
		kn[  8] = "Backspace  ";	kn[  18] = "Alt ";		kn[  28] = "0x1C  ";		kn[ 38] = "Up  ";			kn[ 48] = "Zero  ";
		kn[  9] = "Tab  ";			kn[  19] = "Pause";		kn[  29] = "0x1d  ";		kn[ 39] = "Right  ";			kn[ 49] = "One  ";

		kn[ 50] = "Two  ";			kn[ 60] = "0x3c  ";			kn[ 70] = "F  ";			kn[ 80] = "P  ";			kn[ 90] = "Z  ";
		kn[ 51] = "Three ";			kn[ 61] = "0x3d ";			kn[ 71] = "G  ";			kn[ 81] = "Q  ";			kn[ 91] = "Left Windows  ";
		kn[ 52] = "Four";			kn[ 62] = "0x3e ";			kn[ 72] = "H  ";			kn[ 82] = "R  ";			kn[ 92] = "Right Windows ";
		kn[ 53] = "Five";			kn[ 63] = "0x3f ";			kn[ 73] = "I  ";			kn[ 83] = "S  ";			kn[ 93] = "Application  ";
		kn[ 54] = "Six";			kn[ 64] = "0x40 ";			kn[ 74] = "J  ";			kn[ 84] = "T  ";			kn[ 94] = "0x5e  ";
		kn[ 55] = "Seven";			kn[ 65] = "A ";			kn[ 75] = "K ";					kn[ 85] = "U  ";			kn[ 95] = "Sleep  ";
		kn[ 56] = "Eight";			kn[ 66] = "B  ";			kn[ 76] = "L  ";			kn[ 86] = "V  ";			kn[ 96] = "Numpad 0  ";
		kn[ 57] = "Nine";			kn[ 67] = "C  ";			kn[ 77] = "M  ";			kn[ 87] = "W  ";			kn[ 97] = "Numpad 1  ";
		kn[ 58] = "0x3a  ";			kn[ 68] = "D  ";			kn[ 78] = "N  ";			kn[ 88] = "X  ";			kn[ 98] = "Numpad 2  ";
		kn[ 59] = "0x3b  ";			kn[ 69] = "E  ";			kn[ 79] = "O  ";			kn[ 89] = "Y  ";			kn[ 99] = "Numpad 3  ";

		kn[100] = "Numpad 4  ";			kn[110] = "Decimal ";			kn[120] = "F9  ";			kn[130] = "F19  ";			kn[140] = "0x8c  ";
		kn[101] = "Numpad 5  ";			kn[111] = "Divide ";			kn[121] = "F10  ";			kn[131] = "F20  ";			kn[141] = "0x8d  ";
		kn[102] = "Numpad 6  ";			kn[112] = "F1  ";			kn[122] = "F11  ";			kn[132] = "F21  ";			kn[142] = "0x8e  ";
		kn[103] = "Numpad 7  ";			kn[113] = "F2  ";			kn[123] = "F12  ";			kn[133] = "F22  ";			kn[143] = "0x8f  ";
		kn[104] = "Numpad 8  ";			kn[114] = "F3  ";			kn[124] = "F13  ";			kn[134] = "F23  ";			kn[144] = "Num Lock  ";
		kn[105] = "Numpad 9  ";			kn[115] = "F4  ";			kn[125] = "F14  ";			kn[135] = "F24  ";			kn[145] = "Scroll  ";
		kn[106] = "Multiply ";			kn[116] = "F5  ";			kn[126] = "F15  ";			kn[136] = "0x88  ";			kn[146] = "0x92  ";
		kn[107] = "Add ";			kn[117] = "F6  ";			kn[127] = "F16  ";			kn[137] = "0x89  ";				kn[147] = "0x93  ";
		kn[108] = "Separator ";			kn[118] = "F7  ";			kn[128] = "F17  ";			kn[138] = "0x8a  ";			kn[148] = "0x94  ";
		kn[109] = "Subtract ";			kn[119] = "F8  ";			kn[129] = "F18  ";			kn[139] = "0x8b  ";			kn[149] = "0x95  ";

		kn[150] = "0x96  ";				kn[172] = "Brw Start  ";		kn[182] = "Start App 1  ";	kn[192] = "OEM 3";		kn[168] = "Brw Refresh ";
		kn[160] = "Left Shift";			kn[173] = "Volume mute ";		kn[183] = "Start App 2  ";	kn[193] = "0xc1 ";		kn[151] = "0x97  ";
		kn[161] = "Right Shift";		kn[174] = "Volume Down";		kn[184] = "0xb8";			kn[194] = "0xc2 ";		kn[152] = "0x98 ";
		kn[164] = "Left Menu";			kn[175] = "Volume Up";			kn[185] = "0xb9";			kn[195] = "0xc3 ";			kn[153] = "0x99  ";
		kn[165] = "Right Menu";			kn[176] = "Next track";			kn[186] = "OEM 1";			kn[196] = "0xc4 ";			kn[154] = "0x9a  ";
		kn[166] = "Brw Back";			kn[177] = "Previous track";		kn[187] = "Plus";			kn[197] = "0xc5 ";			kn[155] = "0x9b  ";
		kn[167] = "Brw Forward  ";		kn[178] = "Stop Media";			kn[188] = "Comma";			kn[198] = "0xc6 ";			kn[156] = "0x9c  ";
		kn[169] = "Brw Stop  ";			kn[179] = "Media Key";			kn[189] = "Minus";			kn[199] = "0xc7 ";			kn[157] = "0x9d  ";
		kn[170] = "Brw Search  ";		kn[180] = "Start mail";			kn[190] = "Period";			kn[162] = "Left Ctrl ";			kn[158] = "0x9e  ";
		kn[171] = "Brw Favorites";		kn[181] = "Select media";		kn[191] = "OEM 2";			kn[163] = "Right Ctrl ";			kn[159] = "0x9f  ";

		kn[200] = "0xc8  ";			kn[210] = "0xd2  ";			kn[220] = "OEM 5  ";			kn[230] = "0xe6  ";			kn[240] = "0xf0  ";
		kn[201] = "0xc9  ";			kn[211] = "0xd3  ";			kn[221] = "OEM 6  ";			kn[231] = "Packet  ";			kn[241] = "0xf1  ";
		kn[202] = "0xca  ";			kn[212] = "0xd4  ";			kn[222] = "OEM 7  ";			kn[232] = "0xe8  ";			kn[242] = "0xf2  ";
		kn[203] = "0xcb";			kn[213] = "0xd5  ";			kn[223] = "OEM 8  ";			kn[233] = "0xe9  ";			kn[243] = "0xf3  ";
		kn[204] = "0xcc";			kn[214] = "0xd6  ";			kn[224] = "0xe0  ";			kn[234] = "0xea  ";			kn[244] = "0xf4  ";
		kn[205] = "0xcd";			kn[215] = "0xd7  ";			kn[225] = "0xe1  ";			kn[235] = "0xeb  ";			kn[245] = "0xf5  ";
		kn[206] = "0xce";			kn[216] = "0xd8  ";			kn[226] = "OEM 102  ";			kn[236] = "0xec  ";			kn[246] = "Attn  ";
		kn[207] = "0xcf";			kn[217] = "0xd9  ";			kn[227] = "0xe3  ";			kn[237] = "0xed  ";			kn[247] = "CrSel  ";
		kn[208] = "0xd0  ";			kn[218] = "0xda  ";			kn[228] = "0xe4  ";			kn[238] = "0xee  ";			kn[248] = "ExSel  ";
		kn[209] = "0xd1  ";			kn[219] = "OEM 4  ";		kn[229] = "Process Key  ";			kn[239] = "0xef  ";			kn[249] = "EOF  ";

		kn[250] = "Play "; kn[251] = "Zoom "; kn[252] = "NoName "; kn[253] = "PA1 "; kn[254] = "OEM clear "; kn[255] = "0xff ";

	}
}

