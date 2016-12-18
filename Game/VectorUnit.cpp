#include <Game/VectorUnit.h>
#include <gtc/matrix_transform.hpp>

#define ZYUG_HALF_PI 1.5707963f

namespace Game{

	VectorUnit VectorUnit::vectorUnitInstance;
	float VectorUnit::nc_z[20];
	float VectorUnit::wait[10];

	bool VectorUnit::initialize(){

		t1 = glm::vec3(	0.0f,	2.3f,	0.8f);
		t2 = glm::vec3(	0.0f,	2.3f,	1.0f);
		t3 = glm::vec3(	0.0f,	2.3f,	1.2f);

		wait[0] = 0.05f; wait[5] = 1.0f;
		wait[1] = 0.1f;  wait[6] = 2.0f;
		wait[2] = 0.2f;  wait[7] = 3.0f;
		wait[3] = 0.3f;  wait[8] = 4.0f;
		wait[4] = 0.4f;  wait[9] = 5.0f;

		noOfnc_zPlanes = 20;
		nc_z[0] = -0.5f; nc_z[10] = -1.8f;
		nc_z[1] = -0.6f; nc_z[11] = -1.9f;
		nc_z[2] = -0.7f; nc_z[12] = -2.0f;
		nc_z[3] = -0.8f; nc_z[13] = -2.1f;
		nc_z[4] = -0.9f; nc_z[14] = -2.2f;
		nc_z[5] = -1.0f; nc_z[15] = -2.7f;
		nc_z[6] = -1.1f; nc_z[16] = -2.75f;
		nc_z[7] = -1.3f; nc_z[17] = -2.8f;
		nc_z[8] = -1.5f; nc_z[18] = -2.85f;
		nc_z[9] = -1.7f; nc_z[19] = -2.8f;

		noOfWaitTimes = 10;
			//------------card positio
			//normal rotations
		 atkFaceupUp =		glm::mat4();
		 atkFacedownUp =	glm::mat4(-1,0,0,0, 0,1,0,0, 0,0,-1,0, 0,0,0,1); 
		 atkFaceupFlat =	glm::mat4(1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1);
		 atkFacedownFlat =	glm::mat4(-1,0,0,0, 0,0,-1,0, 0,-1,0,0, 0,0,0,1);
		 defFaceupFlat =	glm::mat4(0,0,1,0, 1,0,0,0, 0,1,0,0, 0,0,0,1); 
		 defFacedownFlat =	glm::mat4(0,0,-1,0, 1,0,0,0, 0,-1,0,0, 0,0,0,1); 
		 atkFaceupUpLeftLook = glm::rotate(glm::mat4(),  ZYUG_HALF_PI, glm::vec3(0.0f,1.0f,0.0f) );
		 atkFaceupUpRightLook= glm::rotate(glm::mat4(),  -ZYUG_HALF_PI, glm::vec3(0.0f,1.0f,0.0f) );
			//enemy rotations
		 eAtkFaceupUp =		glm::mat4(-1,0,0,0, 0,1,0,0, 0,0,-1,0, 0,0,0,1); 
		 eAtkFacedownUp =	glm::mat4();
		 eAtkFaceupFlat =	glm::mat4(-1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1);
		 eAtkFacedownFlat = glm::mat4(1,0,0,0, 0,0,1,0, 0,-1,0,0, 0,0,0,1);
		 eDefFaceupFlat =	glm::mat4(0,0,-1,0, -1,0,0,0, 0,1,0,0, 0,0,0,1);
		 eDefFacedownFlat = glm::mat4(0,0,1,0, -1,0,0,0, 0,-1,0,0, 0,0,0,1);
			//side rotations
		 cardPBackLeft  =	glm::mat4(0,0,1,0, 0,1,0,0, -1,0,0,0, 0,0,0,1); 
		 cardPBackRight  =	glm::mat4(0,0,-1,0, 0,1,0,0, 1,0,0,0, 0,0,0,1); 
		 cardEBackLeft  =	cardPBackRight; 
		 cardEBackRight  =	cardPBackLeft; 

		 cardBatAtkBackLeft = glm::transpose(
			 glm::mat4(	/*0.196136,		0,		0.980577,		0,
						0,		1,		0,		0,
						-0.980577,		0,		0.196136,		0,
						0 ,		0,		0,		1));*/
						-0.208889 ,       0 ,      0.977939   ,     0,
						0    ,   1 ,      0 ,      0,
						-0.977939  ,     0 ,      -0.208889   ,     0,
						0   ,    0    ,   0 ,      1));

		cardBatDefBackLeft = glm::transpose(
			glm::mat4(	/*-0.196136,		0,		0.980577,		0,
						0,		1,		0,		0,
						-0.980577,		0,		-0.196136,		0,
						0 ,		0,		0,		1));*/
						0.208889 ,       0 ,      0.977939   ,     0,
						0    ,   1 ,      0 ,      0,
						-0.977939  ,     0 ,      0.208889   ,     0,
						0   ,    0    ,   0 ,      1));

		
			//-----------board postion
		 bCardScale					= glm::vec3(	0.75f,	0.75f,	1.0f);
		 bBoardPosition				= glm::vec3(	0.0f,	-5.0f,	0.0f);
		 bBoardScale				= glm::vec3(	0.4f,	0.4f,	0.4f);
		 bBoardRotation				= glm::mat4();
		 bBoardCursorScale			= glm::vec3(	0.3f,	0.3f,	1.0f);
		 bBoardCursorOffset			= glm::vec3(	0.0f,	0.01f,	0.00f);
			//board cam
		 bPlayerCamViewDirection	= glm::vec3(	0.0f,	-1.0f,	-0.002f);
		 bPlayerCamPosOrigin		= glm::vec3(	0.0f,	2.3f,	0.6f);
		 bPlayerCamPosPMonZ			= 0.0f;
		 bPlayerCamPosPMagZ			= 0.4f;
		 bPlayerCamPosEMonZ			= -0.7f;
		 bPlayerCamPosEMagZ			= -1.1f;

		 bEnemyCamViewDirection		= glm::vec3(	0.0f,	-1.0f,	0.002f);
		 bEnemyCamPosOrigin			= glm::vec3(	0.0f,	2.3f,	-0.6f);
		 bEnemyCamPosPMonZ			= 0.6f;
		 bEnemyCamPosPMagZ			= 1.0f;
		 bEnemyCamPosEMonZ			= 0.0f;
		 bEnemyCamPosEMagZ			= -0.4f;//-1

			//board cards
		 bPlayerCardPosOrigin		= glm::vec3(	-0.856f,	-0.55f,	0.8f);
		 bPlayerCardPosOffset		= glm::vec3(	0.421f,	0.0f,	-0.42f);
		 bEnemyCardPosOrigin		= glm::vec3(	0.841f,	-0.55f,	-0.38f);
		 bEnemyCardPosOffset		= glm::vec3(	-0.424f,	0.0f,	-0.42f);
		 bShadowPosOffset			= glm::vec3(	0.0f,	-0.01f,	0.0f);
			//magics
		 bPlayerMagicConfirmPos		=  glm::vec3(	0.0f,	0.0f,	1.0f);
		 bEnemyMagicConfirmPos		=  glm::vec3(	0.0f,	0.0f,	-1.0f);
			//lifepoints
		 bLPModelPos				= glm::vec3(	0.3f,	0.25f,	nc_z[5]);
		 bLPModelOffPos				= glm::vec3(	0.5f,	0.25f,	nc_z[5]);
		 bLPMModelScale				= glm::vec3(	0.08f,	0.05f,	0.06f);
		 bLPPlayerLPOffset			= glm::vec3(	-0.025f,	-0.055f,	0.1f);
		 bLPPlayerCardOffset		= glm::vec3(	-0.03f,	-0.035f,		0.1f);
		 bLPEnemyLPOffset			= glm::vec3(	-0.025f,	0.0075f,	0.1f);
		 bLPEnemyCardOffset			= glm::vec3(	-0.03f,	-0.0125f,	0.1f);
		 bLPDeckScale				= glm::vec3(	0.12f,	0.19f,	1.0f);
		 bLPLPNumScale				= glm::vec3(	0.11f,	0.3f,	1.0f);
		 bLPLPScale					= glm::vec3(	0.13f,	0.15f,	1.0f);


			//field
		 bFieldModelPos				= glm::vec3(	-0.3f,	0.25f,	nc_z[5]);
		 bFieldModelOffPos			= glm::vec3(	-0.5f,	0.25f,	nc_z[5]);
		 bFieldModelScale			= glm::vec3(	0.06f,	0.04f,	0.3f);
		 bFieldTextOffset			= glm::vec3(	0.042f,	-0.03f,	0.1f);
		 bFieldTextScale			= glm::vec3(	0.2f,	0.5f,	1.0f);

			//underlays
		 bUnder1ShownPos			= glm::vec3(	0.0f,	-0.68f, nc_z[16]);
		 bUnder1HiddenPos			= glm::vec3(	0.0f,	-1.2f,	nc_z[16]);
		 bUnder1Scale				= glm::vec3(	1.12f,	0.215f,	0.0f);
		 bUnder1TextScale			= glm::vec3(	0.0f,	0.0f,	1.0f);
		 bUnder1TextOffset			= glm::vec3(	-0.95f,	-0.07f,	+0.001f);

		 bUnder2ShownPos			= glm::vec3(	0.0f,	-0.29f, nc_z[16]-0.002f);
		 bUnder2HalfShownPos		= glm::vec3(	0.0f,	-0.48f,	nc_z[16]-0.002f);
		 bUnder2HiddenPos			= glm::vec3(	0.0f,	-1.4f,	nc_z[16]);
		 bUnder2Scale				= glm::vec3(	1.276f,	0.215f,	0.0f);
		 bUnder2TextOffset			= glm::vec3(	-0.95f,	0.125f,	+0.001f);
		 bUnder2GuardOffset1		= glm::vec3(	0.0f,	0.0f,	0.0f);
		 bUnder2GuardOffset2		= glm::vec3(	0.0f,	0.0f,	0.0f);
		 bUnderguardWarnOffset		= glm::vec3(	0.8f,	-0.068f,	0.001f);
		 bUnderguardWarnScale		= glm::vec3(	0.2f,	0.075f,	1.0f);
		
			//board cursor
		 bPlayerCursorPosOrigin		= glm::vec3(	0.0f,	0.0f,	0.0f);
		 bPlayerCursorPosOffset		= glm::vec3(	0.0f,	0.0f,	0.0f);
		 bEnemyCursorPosOrigin		= glm::vec3(	0.0f,	0.0f,	0.0f);
		 bEnemyCursorPosOffset		= glm::vec3(	0.0f,	0.0f,	0.0f);
		    //turnover cam
		 bToELookDownView			= glm::vec3(	-1.0f,	2.0f,	0.0f);
		 bToPLookDownView			= glm::vec3(	1.0f,	2.0f,	0.0f);
		 bToEmidPosition			= glm::vec3(	3.0f,	-6.0f,	0.0f);
		 bToPmidPosition			= glm::vec3(	-3.0f,	-6.0f,	0.0f);
			//hand positional stuff
			//player
		 pHandCardPosOrigin			= glm::vec3(	-0.760f, 0.075f, 2.5f);
		 pHandCardPosOffset			= glm::vec3(	0.38f,	0.1f,	0.0f);
		 pHandMarkingOffset			= glm::vec3(	-0.15f,	0.2f,	0.01f);
		 pHandMarkingScale			= glm::vec3(	0.9f,	0.9f,	1.0f);
		 pHandCardOffPos			= glm::vec3(	5.0f,	0.075f,	2.5f);
		 pHandCamOrigin				= glm::vec3(	0.0f,	1.1f,	 5.0f);
		 pHandCamView				= glm::vec3(	0.0f,-0.225f,	-1.0f);
			//cursor
		 pHandCursorPosOrigin		= glm::vec3(	-0.7f,	-0.29f,	-2.0f);
		 pHandCursorPosOffset		= glm::vec3(	0.287f,	0.0f,	0.0f);
			//board
		 pHandBoardOrigin			= glm::vec3(	-0.74f,	-0.4f,	1.05f);
		 pHandBoardOffset			= glm::vec3(	0.37f,	0.0f,	0.4f);
		 pHandBoardJump				= glm::vec3(	0.0f,	0.0f,	-0.7f);
		 pHandBoardMarkOffset		= glm::vec3(	-0.15f,	0.01f,	-0.2f);
		
			//enemy
		 eHandCardPosOrigin			= glm::vec3(	0.74f, 0.075f, -2.5f);
		 eHandCardPosOffset			= glm::vec3(	-0.38f,	0.1f,	0.0f);
		 eHandMarkingOffset			= glm::vec3(	0.15f,	0.2f,	-0.01f);
		 eHandMarkingScale			= glm::vec3(	0.9f,	0.9f,	1.0f);
		 eHandCardOffPos			= glm::vec3(	-5.0f,	0.075f,	-2.5f);
		 eHandCamOrigin				= glm::vec3(	0.0f,	1.1f,	-5.0f);
		 eHandCamView				= glm::vec3(	0.0f,	-0.225f,	1.0f);
			//cursor
		 eHandCursorPosOrigin		= glm::vec3(	-0.7f,	-0.29f,	-2.0f);
		 eHandCursorPosOffset		= glm::vec3(	0.287f,	0.0f,	0.0f);
			//board
		 eHandBoardOrigin			= glm::vec3(	0.74f,	-0.4f,	-1.05f);//-0.85
		 eHandBoardOffset			= glm::vec3(	-0.37f,	0.0f,	-0.4f);
		 eHandBoardJump				= glm::vec3(	0.0f,	0.0f,	-0.8f);
		 eHandBoardMarkOffset		= glm::vec3(	0.15f,	0.01f,	0.2f);


		 //position Unit
		 //face
		 posPCardPosition			= glm::vec3(	0.00f,	0.5f,	2.55f);
		 posECardPosition			= glm::vec3(	0.00f,	0.5f,	-2.55f);
		 posLeftArrowPos			= glm::vec3(	-0.3f,	0.0f,	nc_z[16]);
		 posRightArrowXOffset		= 0.6f;
		 posArrowScale				= glm::vec3(	0.05f,	0.05f,	1.0f);
		 //con
		 posPCardConPos				= glm::vec3(	0.0f,	0.8f,	2.55f);
		 posECardConPos				= glm::vec3(	0.0f,	0.8f,	-2.55f);
		 posConBgPos				= glm::vec3(	0.0f,	-0.3f,	nc_z[16]);
		 posConBgScale				= glm::vec3(	0.7f,	0.2f,	1.0f);
		 posConCursorPos			= glm::vec3(	0.0f,	-0.285f,	nc_z[17]);
		 posConCursorYOffset		= -0.13f;
		 posConCursorScale			= glm::vec3(	0.6f,	0.06f,	1.0f);
		 posPConGuardOffset			= glm::vec3(	0.0f,	0.13f,	0.01f);
		 posConGuardScale			= glm::vec3(	0.3f,	1.0f,	1.0f);
		 posConSymbolOffset			= glm::vec3(	-0.23f,	0.02f,	0.01f);
		 posConSymbolYOffset		= -0.13f;
		 posConSymbolScale			= glm::vec3(	0.8f,	0.8f,	1.0f);
		 posConTextXOffset			= 0.1f;
		 posConTextScale			= glm::vec3(	0.3f,	0.95f,	1.0f);

		 //fusion Unit
			//gen posi
		 fusPos1					= glm::vec3(	-0.8f,	0.5f,	2.4f);//top = 1.1, bottom = -0.1, x = 0.8
		 fusPos2					= glm::vec3(	0.8f,	0.5f,	2.4f);
		 fusPos3					= glm::vec3(	0.0f,	1.1f,	2.4f);//two less
		 fusPos4					= glm::vec3(	-0.8f,	-0.1f,	2.4f);
		 fusPos5					= glm::vec3(	0.8f,	-0.1f,	2.4f);
		 fusPos6					= glm::vec3(	0.0f,	-0.1f,	2.4f);//two less//0.0,-0.1,2.4
		 fusPos7					= glm::vec3(	-0.8f,	0.8f,	2.4f);
		 fusPos8					= glm::vec3(	-0.6f,	-0.1f,	2.4f);
		 fusPos9					= glm::vec3(	0.6f,	-0.1f,	2.4f);
		 fusPos10					= glm::vec3(	0.8f,	0.8f,	2.4f);
		 fusPos11					= glm::vec3(	-0.8f,	0.2f,	2.4f);
		 fusPos12					= glm::vec3(	0.8f,	0.2f,	2.4f);
		 fusPos13					= glm::vec3(	-0.4f,	1.1f,	2.4f);
		 fusPos14					= glm::vec3(	0.4f,	1.1f,	2.4f);

		 fusCentre					= glm::vec3(	0.0f,	0.5f,	2.4f);//orig:z = 2.4
		 fusCentreStartScale		= glm::vec3(	0.5f,	0.5f,	1.0f);
		 weakStrongZOffset			= 0.01f;

		 fusEnPos1					= glm::vec3(	0.8f,	0.5f,	-2.4f);//top = 1.1, bottom = -0.1, x = 0.8
		 fusEnPos2					= glm::vec3(	-0.8f,	0.5f,	-2.4f);
		 fusEnPos3					= glm::vec3(	0.0f,	1.1f,	-2.4f);//two less
		 fusEnPos4					= glm::vec3(	0.8f,	-0.1f,	-2.4f);
		 fusEnPos5					= glm::vec3(	-0.8f,	-0.1f,	-2.4f);
		 fusEnPos6					= glm::vec3(	0.0f,	-0.1f,	-2.4f);//two less
		 fusEnPos7					= glm::vec3(	0.8f,	0.8f,	-2.4f);
		 fusEnPos8					= glm::vec3(	0.4f,	-0.1f,	-2.4f);
		 fusEnPos9					= glm::vec3(	-0.4f,	-0.1f,	-2.4f);
		 fusEnPos10					= glm::vec3(	-0.8f,	0.8f,	-2.4f);
		 fusEnPos11					= glm::vec3(	0.8f,	0.2f,	-2.4f);
		 fusEnPos12					= glm::vec3(	-0.8f,	0.2f,	-2.4f);
		 fusEnPos13					= glm::vec3(	-0.4f,	1.1f,	-2.4f);
		 fusEnPos14					= glm::vec3(	0.4f,	1.1f,	-2.4f);

		 fusEnCentre				= glm::vec3(	0.0f,	0.5f,	-2.4f);
		 fusEnCentreStartScale		= glm::vec3(	0.5f,	0.5f,	1.0f);
		 weakStrongEnZOffset		= -0.01f;


		fusBigCardScale				= glm::vec3(	1.0f,	1.0f,	1.0f);
		fusMassiveCardScale			= glm::vec3(	8.0f,	8.0f,	1.0f);
		fusMassiveCardPosition		= glm::vec3(	0.0f,	-10.0f,	-10.0f);
		fusMassiveCardYOffset		= 40.0f;
		//trap Unit
		trapPCamPos					= glm::vec3(	0.0f,	2.5f,	0.1f);
		trapPCamView				= glm::vec3(	0.0f, -1.00f,	-0.2f);
		trapECamPos					= glm::vec3(	0.0f,	2.5f,	-0.1f);
		trapECamView				= glm::vec3(	0.0f, -1.00f,	0.2f);
		trapParticleOffset			= glm::vec3(	0.0f,	0.0f,	0.1f);



		//Battle Unit
		batPSmallLeftPos			= glm::vec3(	-0.82f,	0.187f,	1.0f);
		batPSmallXOffset			= 1.64f;
		batESmallLeftPos			= glm::vec3(	0.82f,	0.187f,	-1.0f);
		batESmallXOffset			= -1.64f;
		batBigCardScale				= glm::vec3(	0.29f,	0.29f,	1.0f);
		batBigCardPos				= glm::vec3(	-0.16f, 0.0f, 	nc_z[3]);
		batBigCardXOffset			= +0.32f;
		batLeftFireStart			= glm::vec3(	0.0f,	0.0f,	0.0f);
		batRightFireStart			= glm::vec3(	0.0f,	0.0f,	0.0f);
		batFireEndYOffset			= 0.0f;
		batConBoostOrbScale			= glm::vec3(	0.3f,	0.3f,	1.0f);
		batConBoostSymbolScale		= glm::vec3(	0.5f,	0.5f,	1.0f);
		batSlashScale				= glm::vec3(	0.3f,	0.3f,	1.0f);
		batNumberScale				= glm::vec3(	0.4f,	0.8f,	1.0f);
		

		 //CDU
		 cduCardScale =			glm::vec3(0.32f,0.32f,1.0f);
		 cduBoardScale =		glm::vec3(0.167f,0.235f,1.0f);
		 cduCardPos =			glm::vec3(-0.17f, 0.0f, -0.8f);
		 cduBoardPos =			glm::vec3(0.157f, 0.0f, -0.8f);
		 cduCardOffPos =		glm::vec3(-0.75f,0.0f,-0.8f);
		 cduBoardOffPos =		glm::vec3(0.75f,0.0f,-0.8f);
		 //actual type
		 cduTypeOffset =		glm::vec3(-0.13f,0.192f,0.01f);
		 cduTypeTextXOffset =	0.031f;
		 cduTypeSymbolScale =	glm::vec3(0.3f,0.3f,1.0f);
		 cduTypeTextScale =		glm::vec3(0.1f,0.35f,1.0f);
		 //guardian header
		 cduGuardianOffset =	glm::vec3(-0.13f,0.135f,0.01f);
		 cduGuardainScale =		glm::vec3(0.1f, 0.35f, 1.0f);
		 //constellations
		 cduFirstConOffset =	glm::vec3(-0.13f,0.10f,0.01f);
		 cduSecondConOffset =	glm::vec3(-0.13f,0.06f,0.01f);
		 cduConSymbolScale =	glm::vec3(0.2f,0.2f,1.0f);
		 cduConTextXOffset =	0.031f;
		 cduConTextScale =		glm::vec3(0.1f, 0.35f, 1.0f);
		 //blurb
		 cduFirstTextOffset =	glm::vec3(-0.137f,0.0f,0.01f);
		 cduTextYOffset =		-0.026f;
		 cduTextScale = 		glm::vec3(0.085f, 0.3f, 1.0f);


		return true;
	}
//= glm::vec3(	0.0f,	0.0f,	0.0f);
	bool VectorUnit::shutdown(){

		return true;
	}
}