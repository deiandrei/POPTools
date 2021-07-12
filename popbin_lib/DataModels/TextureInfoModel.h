#pragma once

#include "AbstractModel.h"
#include <map>

namespace popbin {

	class TextureInfoModel : public AbstractModel {
		public:
			TextureInfoModel(Entry* entry);

			std::vector<std::pair<uint4, int>> TextureList;

	};

}
