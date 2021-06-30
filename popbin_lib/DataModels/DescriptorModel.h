#pragma once

#include "AbstractModel.h"

namespace popbin {

	class DescriptorModel : public AbstractModel {
		public:
			DescriptorModel(Entry* entry);

			std::vector<uint4> ActorList;

	};

}
