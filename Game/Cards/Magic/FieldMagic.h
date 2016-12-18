#pragma once
#include <Game\Cards\Magic\MagicSource.h>
#include <Game\WaitUnit.h>

#define YUG_MAG_FD_CH_START 0
#define YUG_MAG_FD_CH_GLOW1 1
#define YUG_MAG_FD_CH_GLOW2 2
#define YUG_MAG_FD_CH_RETURN 3
#define YUG_MAG_FD_CH_END 4
#define YUG_MAG_FD_CH_IDLE 5
namespace Card{

	class FieldMagic  : public MagicSource, public Game::WaitUnit
	{
	public:
		int chain;
		
		virtual void startup();
		virtual void cleanup(){}
		virtual void update();
		virtual void render(){}
		virtual void setNewFieldTexture(){}
		virtual void setFieldBoosts(){}
		virtual void setFieldWeakens(){}
		virtual void setFieldAmtran(){}

		void startUpdate();
		void glow1Update();
		void glow2Update();
		void returnUpdate();
		void endUpdate();
	};

}