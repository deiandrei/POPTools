#pragma once

#include "AbstractModel.h"
#include <map>

static char tga_header[] = { '0x00', '0x00', '0x02', '0x00', '0x00', '0x00', '0x00', '0x00', '0x00', '0x00', '0x00', '0x00', '0x20', '0x00', '0x20', '0x00', '0x20', '0x00' };

namespace popbin {

	enum TextureImageType {
		TEXTURE_TGA,
		TEXTURE_DDS,
		TEXTURE_8BIT,
		TEXTURE_UNKNOWN,
		NUM_IMG_TYPES
	};

	class TextureModel : public AbstractModel {
		public:
			TextureModel(Entry* entry);

			byte1* Data;
			uint4 DataSize;
			uint2 Width;
			uint2 Height;

			TextureImageType Type;
			uint4 PaletteID;


	};

}
