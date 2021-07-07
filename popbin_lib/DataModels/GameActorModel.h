#pragma once

#include "AbstractModel.h"

#include "GeometryModel.h"

namespace popbin {

	class GameActorModel : public AbstractModel {
		public:
			GameActorModel(Entry* entry);

			float Matrix[16];
			std::vector<vec3> Vectors;

	};

}
