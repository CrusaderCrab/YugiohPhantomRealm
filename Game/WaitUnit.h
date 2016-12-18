#pragma once

namespace Game{

	class WaitUnit
	{
	public:
		virtual void startup();
		virtual void wait(float duration);
		virtual void continueWaiting();
		float waitDuration;
		float currentlyWaited;
		bool isWaiting;
	};


}
