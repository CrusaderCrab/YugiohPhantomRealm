#pragma once
#include <Game\Cards\Magic\FieldMagic.h>

namespace Card{

	class Umi : public FieldMagic
	{
	public:
		void setNewFieldTexture();
		void setFieldBoosts();
		void setFieldWeakens();
		void setFieldAmtran();
	};

}

