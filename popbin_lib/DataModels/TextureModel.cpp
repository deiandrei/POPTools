#include "TextureModel.h"
#include "../BinArchive.h"

namespace popbin {
	TextureModel::TextureModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;
		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		uint4 temp, type;
		uint4 width, height, numComponents;
		
		bb->Position() += 4; // skip the type, which is equal to the fileID
		bb->Read<uint4>(&temp); // unknown
		bb->Read<uint4>(&temp); // unknown
		bb->Read<uint2>(&Width); // img width
		bb->Read<uint2>(&Height); // img height

		bb->Position() += 24; // skip these blocks, dunno what they are but if needed we will find out
		bb->Read<uint4>(&type);

		if (type == 0) Type = TextureImageType::TEXTURE_TGA;
		else if (type == 1) Type = TextureImageType::TEXTURE_8BIT;
		else if (type == 7) Type = TextureImageType::TEXTURE_DDS;
		else type = TextureImageType::TEXTURE_UNKNOWN;

		bb->Read<uint4>(&width); // temp params
		bb->Read<uint4>(&height);
		bb->Read<uint4>(&numComponents);

		if (Type == TextureImageType::TEXTURE_8BIT) {
			bb->Read<uint4>(&PaletteID); // read the palette ID for the 8bit texture
		}

		DataSize = mEntry->size - bb->Position();
		Data = new byte1[DataSize];
		bb->ReadArray<byte1>(&Data[0], DataSize);

		// now lets decode the texture
		if (Type == TextureImageType::TEXTURE_TGA) {
			byte1 tga_header_temp[18];
			strcpy((char*)&tga_header_temp[0], tga_header);

			tga_header_temp[12] = (byte1)(Width % 0x100);
			tga_header_temp[13] = (byte1)(Width / 0x100);
			tga_header_temp[14] = (byte1)(Height % 0x100);
			tga_header_temp[15] = (byte1)(Height / 0x100);

			byte1* newimg = new byte1[sizeof(tga_header_temp) + DataSize];
			memcpy(newimg, tga_header_temp, sizeof(tga_header_temp));
			memcpy(newimg + sizeof(tga_header_temp), Data, DataSize);

			DataSize += sizeof(tga_header_temp);
			delete Data;
			Data = newimg;
		}
	}

}
