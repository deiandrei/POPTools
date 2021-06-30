#pragma once

#include "AbstractModel.h"
#include <map>

namespace popbin {

	class TexturePackModel : public AbstractModel {
		public:
			TexturePackModel(Entry* entry);

			std::map<uint4, int> TextureList;

	};

}
