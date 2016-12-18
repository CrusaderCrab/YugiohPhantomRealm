#pragma once
#include <glm.hpp>


namespace Game{
	class VectorUnit
	{
	public:
		bool initialize();
		bool shutdown();
		//z planes
		//camera

		//no camera
		int noOfnc_zPlanes;
		static float nc_z[20];

		//wait times
		int noOfWaitTimes;
		static float wait[10];

					//------------card positional stuff
					//normal rotations
		glm::mat4 atkFaceupUp;
		glm::mat4 atkFacedownUp;
		glm::mat4 atkFaceupFlat;
		glm::mat4 atkFacedownFlat;
		glm::mat4 defFaceupFlat;
		glm::mat4 defFacedownFlat;
		glm::mat4 atkFaceupUpLeftLook;
		glm::mat4 atkFaceupUpRightLook;
					//enemy rotations
		glm::mat4 eAtkFaceupUp;
		glm::mat4 eAtkFacedownUp;
		glm::mat4 eAtkFaceupFlat;
		glm::mat4 eAtkFacedownFlat;
		glm::mat4 eDefFaceupFlat;
		glm::mat4 eDefFacedownFlat;
					//sideways rotations
		glm::mat4 cardBatAtkBackLeft;
		glm::mat4 cardBatDefBackLeft;

		glm::mat4 cardPBackLeft;
		glm::mat4 cardPBackRight;
		glm::mat4 cardEBackLeft;
		glm::mat4 cardEBackRight;

					//-----------board postional info
		glm::vec3 bCardScale;
		glm::vec3 bBoardPosition;
		glm::vec3 bBoardScale;
		glm::mat4 bBoardRotation;
		glm::vec3 bBoardCursorScale;
		glm::vec3 bBoardCursorOffset;
					//board cam
		glm::vec3 bPlayerCamViewDirection;
		glm::vec3 bPlayerCamPosOrigin;
		float	  bPlayerCamPosPMonZ;
		float	  bPlayerCamPosPMagZ;
		float	  bPlayerCamPosEMonZ;
		float	  bPlayerCamPosEMagZ;
		glm::vec3 bEnemyCamViewDirection;
		glm::vec3 bEnemyCamPosOrigin;
		float	  bEnemyCamPosPMonZ;
		float	  bEnemyCamPosPMagZ;
		float	  bEnemyCamPosEMonZ;
		float	  bEnemyCamPosEMagZ;
					//board cards
		glm::vec3 bPlayerCardPosOrigin;
		glm::vec3 bPlayerCardPosOffset;
		glm::vec3 bEnemyCardPosOrigin;
		glm::vec3 bEnemyCardPosOffset;
		glm::vec3 bShadowPosOffset;
					//board magics
		glm::vec3 bPlayerMagicConfirmPos;
		glm::vec3 bEnemyMagicConfirmPos;

					//lifepoints
		glm::vec3 bLPModelPos;
		glm::vec3 bLPModelOffPos;
		glm::vec3 bLPMModelScale;
		glm::vec3 bLPPlayerLPOffset;
		glm::vec3 bLPPlayerCardOffset;
		glm::vec3 bLPEnemyLPOffset;
		glm::vec3 bLPEnemyCardOffset;
		glm::vec3 bLPDeckScale;
		glm::vec3 bLPLPNumScale;
		glm::vec3 bLPLPScale;

					//field
		glm::vec3 bFieldModelPos;
		glm::vec3 bFieldModelOffPos;
		glm::vec3 bFieldModelScale;
		glm::vec3 bFieldTextOffset;
		glm::vec3 bFieldTextScale;

					//underlays
		glm::vec3 bUnder1ShownPos;
		glm::vec3 bUnder1HiddenPos;
		glm::vec3 bUnder1Scale;	
		glm::vec3 bUnder1TextScale;
		glm::vec3 bUnder1TextOffset;
		glm::vec3 bUnder2ShownPos;
		glm::vec3 bUnder2HalfShownPos;
		glm::vec3 bUnder2HiddenPos;
		glm::vec3 bUnder2Scale;
		glm::vec3 bUnder2TextOffset;
		glm::vec3 bUnder2GuardOffset1;
		glm::vec3 bUnder2GuardOffset2;
		glm::vec3 bUnderguardWarnOffset;
		glm::vec3 bUnderguardWarnScale;

					//board cursor
		glm::vec3 bPlayerCursorPosOrigin;
		glm::vec3 bPlayerCursorPosOffset;
		glm::vec3 bEnemyCursorPosOrigin;
		glm::vec3 bEnemyCursorPosOffset;
					//turnover camera
		glm::vec3 bToELookDownView;
		glm::vec3 bToPLookDownView;
		glm::vec3 bToEmidPosition;
		glm::vec3 bToPmidPosition;

					//hand positional stuff
					//player
		glm::vec3 pHandCardPosOrigin;
		glm::vec3 pHandCardPosOffset;
		glm::vec3 pHandMarkingOffset;
		glm::vec3 pHandMarkingScale;
		glm::vec3 pHandCardOffPos;
		glm::vec3 pHandCamOrigin;
		glm::vec3 pHandCamView;
					//cursor
		glm::vec3 pHandCursorPosOrigin;
		glm::vec3 pHandCursorPosOffset;
					//board
		glm::vec3 pHandBoardOrigin;
		glm::vec3 pHandBoardOffset;
		glm::vec3 pHandBoardJump;
		glm::vec3 pHandBoardMarkOffset;

					//enemy
		glm::vec3 eHandCardPosOrigin;
		glm::vec3 eHandCardPosOffset;
		glm::vec3 eHandMarkingOffset;
		glm::vec3 eHandMarkingScale;
		glm::vec3 eHandCardOffPos;
		glm::vec3 eHandCamOrigin;
		glm::vec3 eHandCamView;
					//cursor
		glm::vec3 eHandCursorPosOrigin;
		glm::vec3 eHandCursorPosOffset;
					//board
		glm::vec3 eHandBoardOrigin;
		glm::vec3 eHandBoardOffset;
		glm::vec3 eHandBoardJump;
		glm::vec3 eHandBoardMarkOffset;

					//----------positionalUnit
		//face
		glm::vec3 posPCardPosition;
		glm::vec3 posECardPosition;
		glm::vec3 posLeftArrowPos;
		float	  posRightArrowXOffset;
		glm::vec3 posArrowScale;
		//con
		glm::vec3 posPCardConPos;
		glm::vec3 posECardConPos;
		glm::vec3 posConBgPos;
		glm::vec3 posConBgScale;
		glm::vec3 posConCursorPos;
		float	  posConCursorYOffset;
		glm::vec3 posConCursorScale;
		glm::vec3 posPConGuardOffset;
		glm::vec3 posConGuardScale;
		glm::vec3 posConSymbolOffset;
		float	  posConSymbolYOffset;
		glm::vec3 posConSymbolScale;
		float	  posConTextXOffset;
		glm::vec3 posConTextScale;



					//----------fusionUnit
		glm::vec3  fusPos1;
		glm::vec3  fusPos2;
		glm::vec3  fusPos3;
		glm::vec3  fusPos4;
		glm::vec3  fusPos5;
		glm::vec3  fusPos6;
		glm::vec3  fusPos7;
		glm::vec3  fusPos8;
		glm::vec3  fusPos9;
		glm::vec3  fusPos10;
		glm::vec3  fusPos11;
		glm::vec3  fusPos12;
		glm::vec3  fusPos13;
		glm::vec3  fusPos14;

		glm::vec3  fusCentre;
		glm::vec3  fusCentreStartScale;
		float	   weakStrongZOffset;

		glm::vec3  fusEnPos1;
		glm::vec3  fusEnPos2;
		glm::vec3  fusEnPos3;
		glm::vec3  fusEnPos4;
		glm::vec3  fusEnPos5;
		glm::vec3  fusEnPos6;
		glm::vec3  fusEnPos7;
		glm::vec3  fusEnPos8;
		glm::vec3  fusEnPos9;
		glm::vec3  fusEnPos10;
		glm::vec3  fusEnPos11;
		glm::vec3  fusEnPos12;
		glm::vec3  fusEnPos13;
		glm::vec3  fusEnPos14;

					 
		glm::vec3  fusEnCentre;
		glm::vec3  fusEnCentreStartScale;
		float	   weakStrongEnZOffset;

		/*float particleSpeed;
		float fadeInSpeed;
		float fadeOutSpeed;*/

		glm::vec3 fusBigCardScale;
		glm::vec3 fusMassiveCardScale;
		glm::vec3 fusMassiveCardPosition;
		float	  fusMassiveCardYOffset;
						
					//---------Trap Unit
		glm::vec3 trapPCamPos;
		glm::vec3 trapPCamView;
		glm::vec3 trapECamPos;
		glm::vec3 trapECamView;
		glm::vec3 trapParticleOffset;

					
					//----------Battle Unit
		glm::vec3 batPSmallLeftPos;
		float	  batPSmallXOffset;
		glm::vec3 batESmallLeftPos;
		float	  batESmallXOffset;
		glm::vec3 batBigCardScale;
		glm::vec3 batBigCardPos;
		float     batBigCardXOffset;
		glm::vec3 batLeftFireStart;
		glm::vec3 batRightFireStart;
		float	  batFireEndYOffset;
		glm::vec3 batConBoostOrbScale;
		glm::vec3 batConBoostSymbolScale;
		glm::vec3 batSlashScale;
		glm::vec3 batNumberScale;
		


					//----------Card Display Unit
		glm::vec3 cduCardScale;
		glm::vec3 cduBoardScale;
		glm::vec3 cduCardPos;
		glm::vec3 cduBoardPos;
		glm::vec3 cduCardOffPos;
		glm::vec3 cduBoardOffPos;

		glm::vec3 cduTypeOffset;
		float     cduTypeTextXOffset;
		glm::vec3 cduTypeSymbolScale;
		glm::vec3 cduTypeTextScale;

		glm::vec3 cduGuardianOffset;
		glm::vec3 cduGuardainScale;

		glm::vec3 cduFirstConOffset;
		glm::vec3 cduSecondConOffset;
		glm::vec3 cduConSymbolScale;
		float	  cduConTextXOffset;
		glm::vec3 cduConTextScale;

		glm::vec3 cduFirstTextOffset;
		float	  cduTextYOffset;
		glm::vec3 cduTextScale; 

		


		//----done
		glm::vec3 t1;
		glm::vec3 t2;
		glm::vec3 t3;

		static VectorUnit vectorUnitInstance;
	};
#define pos Game::VectorUnit::vectorUnitInstance
}

