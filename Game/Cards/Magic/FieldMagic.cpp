#include <Game\Cards\Magic\FieldMagic.h>
#include <Game\Duel\Board.h>
#include <Game\Animation\MovementUnit.h>
#include <Game\Cards\Magic\MagicUnit.h>
#include <iostream>


namespace Card{

	void FieldMagic::startup(){
		Game::WaitUnit::startup();
		chain = YUG_MAG_FD_CH_START;
		theBoard.cP[0] = 3;
		theBoard.cP[1] = 0;
	}

	void FieldMagic::startUpdate(){
		chain = YUG_MAG_FD_CH_GLOW1;
		wait(1.0f);
	}
	void FieldMagic::glow1Update(){
		theBoard.boardModel.amtran = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		theBoard.boardModel.interpolateAmtran(glm::vec4(3.0f,3.0f,3.0f,1.0f),1.0f);
		chain = YUG_MAG_FD_CH_GLOW2;
		wait(1.0f);

	}
	void FieldMagic::glow2Update(){
		theBoard.field.undoFieldAll();
		setNewFieldTexture();
		setFieldBoosts();
		setFieldWeakens();
		setFieldAmtran();
		theBoard.field.applyFieldAll();

		theBoard.boardModel.interpolateAmtran(glm::vec4(1.0f,1.0f,1.0f,1.0f),5.0f);
		chain = YUG_MAG_FD_CH_RETURN;
		wait(3.0f);
	}
	void FieldMagic::returnUpdate(){
		chain = YUG_MAG_FD_CH_END;
		//std::cout<<"Field: Is it blank yet?\n\n";
		wait(2.0f);
	}
	void FieldMagic::endUpdate(){
		chain = YUG_MAG_FD_CH_IDLE;
		magicUnit.chain = YUG_MAG_CH_SPECIFC_FINISHED;
	}
	
	void FieldMagic::update(){
		if(!isWaiting){
			switch(chain){
			case YUG_MAG_FD_CH_START:
				startUpdate();
				break;
			case YUG_MAG_FD_CH_GLOW1:
				glow1Update();
				break;
			case YUG_MAG_FD_CH_GLOW2:
				glow2Update();
				break;
			case YUG_MAG_FD_CH_RETURN:
				returnUpdate();
				break;
			case YUG_MAG_FD_CH_END:
				endUpdate();
				break;
			}
		}else{
			continueWaiting();
		}

	}

}
