#pragma once

#include "AbstractModel.h"

#include "GeometryModel.h"

namespace popbin {

	using LinkedEntry = std::pair<uint4, int>;

	class GameActorModel : public AbstractModel {
		public:
			GameActorModel(Entry* entry);

			virtual void Export(const std::string& file) override;

			float Matrix[16];
			std::vector<vec3> Vectors;

			bool Has12Floats;
			bool AllFloatsTheSame;
			bool HasParentGao;

			LinkedEntry GeometryEntry;
			LinkedEntry TextureEntry;

			// Unknowns
			uint4 MaybeScriptEntry;
			byte1 UnknownColorBytes1[6];

			uint4 WhateverCounter1;

	};

}
