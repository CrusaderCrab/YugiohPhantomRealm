#pragma once
#include <Game\Cards\Magic\FieldMagic.h>

namespace Card{

	class Forest : public FieldMagic
	{
	public:
		void setNewFieldTexture();
		void setFieldBoosts();
		void setFieldWeakens();
		void setFieldAmtran();
	};

}