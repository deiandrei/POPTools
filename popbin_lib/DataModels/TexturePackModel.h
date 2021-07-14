#pragma once

#include "AbstractModel.h"
#include <map>

namespace popbin {

	class TexturePackModel : public AbstractModel {
		public:
			TexturePackModel(Entry* entry);

			virtual void Export(const std::string& file) override;

			std::map<uint4, int> TextureList;

	};

}
