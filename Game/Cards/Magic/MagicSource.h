#pragma once
#include <Game\YugiohUnit.h>
namespace Card{

	class MagicSource : public Game::YugiohUnit
	{
	public:
		virtual void startup(){}
		virtual void cleanup(){}
		virtual void input(){}
		virtual void update(){}
		virtual void render(){}
		virtual void trapActivationUpdate(){}
	};

}