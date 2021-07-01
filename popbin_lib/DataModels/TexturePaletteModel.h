#pragma once

#include "AbstractModel.h"

namespace popbin {

	class TexturePaletteModel : public AbstractModel {
		public:
			TexturePaletteModel(Entry* entry);

			byte1* Data;
			uint4 DataSize;

	};

}
