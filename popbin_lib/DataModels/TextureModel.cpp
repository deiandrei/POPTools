#include "TextureModel.h"
#include "../BinArchive.h"

#include "TexturePaletteModel.h"

namespace popbin {
	TextureModel::TextureModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;
		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		int idd = archive->SearchEntryByID(entry->fileID);

		uint4 temp, type;
		
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

		bb->Read<uint4>(&temp); // width again
		bb->Read<uint4>(&temp); // height again
		bb->Read<uint4>(&temp); // maybe numComponents?

		if (Type == TextureImageType::TEXTURE_8BIT) {
			bb->Read<uint4>(&PaletteID); // read the palette ID for the 8bit texture
		}

		DataSize = mEntry->size - bb->Position();
		Data = new byte1[DataSize];
		bb->ReadArray<byte1>(&Data[0], DataSize);

		// now lets decode the texture
		if (Type == TextureImageType::TEXTURE_TGA) {
			byte1 tga_header[18] = { 0 };

			tga_header[2] = 2;
			tga_header[16] = 32; // 4 components

			tga_header[12] = (byte1)(Width % 0x100);
			tga_header[13] = (byte1)(Width / 0x100);
			tga_header[14] = (byte1)(Height % 0x100);
			tga_header[15] = (byte1)(Height / 0x100);

			byte1* newimg = new byte1[sizeof(tga_header) + DataSize];
			memcpy(newimg, tga_header, sizeof(tga_header));
			memcpy(newimg + sizeof(tga_header), Data, DataSize);

			DataSize += sizeof(tga_header);
			delete[] Data;
			Data = newimg;
		}
		else if (Type == TextureImageType::TEXTURE_DDS) {
			byte1 dds_header[128] = { 0 };

			dds_header[0] = 0x44; dds_header[1] = 0x44; dds_header[2] = 0x53; dds_header[3] = 0x20;
			dds_header[4] = 0x7C; dds_header[8] = 0x07; dds_header[9] = 0x10; dds_header[10] = 0x08;
			dds_header[22] = 0x04; dds_header[76] = 0x20; dds_header[80] = 0x04; dds_header[84] = 0x44;
			dds_header[85] = 0x58; dds_header[86] = 0x54; dds_header[87] = 0x35;

			dds_header[12] = (byte1)(Height % 0x100);
			dds_header[13] = (byte1)(Height / 0x100);
			dds_header[16] = (byte1)(Width % 0x100);
			dds_header[17] = (byte1)(Width / 0x100);

			byte1* newimg = new byte1[sizeof(dds_header) + DataSize];
			memcpy(newimg, dds_header, sizeof(dds_header));
			memcpy(newimg + sizeof(dds_header), Data, DataSize);

			DataSize += sizeof(dds_header);
			delete[] Data;
			Data = newimg;
		}
		else if (Type == TextureImageType::TEXTURE_8BIT) {
			int paletteEntryID = archive->SearchEntryByIDandType(PaletteID, EntryType::TEXTURE_PALETTE);

			if (paletteEntryID != -1) {
				Entry& paletteEntry = archive->Entries[paletteEntryID];
				TexturePaletteModel* model = static_cast<TexturePaletteModel*>(paletteEntry.model);

				// use the tga header again
				byte1 tga_header[18] = { 0 };

				tga_header[2] = 2;
				tga_header[16] = 32; // 4 components

				tga_header[12] = (byte1)(Width % 0x100);
				tga_header[13] = (byte1)(Width / 0x100);
				tga_header[14] = (byte1)(Height % 0x100);
				tga_header[15] = (byte1)(Height / 0x100);

				uint4 tga_header_size = sizeof(tga_header);

				byte1* decoded_texture = new byte1[Width * Height * 4 + tga_header_size];
				memcpy(decoded_texture, tga_header, tga_header_size);

				for (int i = 0; i < Width * Height; ++i) {
					// decode the color
					byte1 b = model->Data[4 * Data[i]];
					byte1 g = model->Data[4 * Data[i] + 1];
					byte1 r = model->Data[4 * Data[i] + 2];
					byte1 a = model->Data[4 * Data[i] + 3];

					decoded_texture[tga_header_size + i * 4] = b;
					decoded_texture[tga_header_size + i * 4 + 1] = g;
					decoded_texture[tga_header_size + i * 4 + 2] = r;
					decoded_texture[tga_header_size + i * 4 + 3] = a;
				}

				DataSize = Width * Height * 4 + tga_header_size;
				delete[] Data;
				Data = decoded_texture;
			}
		}
	}

}
