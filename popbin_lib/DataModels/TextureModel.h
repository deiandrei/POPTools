#pragma once

#include "AbstractModel.h"
#include <map>

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

			void Export(const std::string& file) override;
			
			std::string FileExtension();

			byte1* Data;
			uint4 DataSize;
			uint2 Width;
			uint2 Height;

			TextureImageType Type;
			uint4 PaletteID;


	};

}
