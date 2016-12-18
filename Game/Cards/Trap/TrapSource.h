#pragma once
namespace Card{

	class TrapSource
	{
	public:
		virtual void startup();
		virtual void cleanup();
		virtual void update();
		virtual void render();
		virtual bool listen(int info);


	};

}
