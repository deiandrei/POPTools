#include "TextureInfoModel.h"
#include "../BinArchive.h"

namespace popbin {
	TextureInfoModel::TextureInfoModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;

		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		byte1 flags1[4];

		bb->Position() += 4;
		bb->ReadArray<byte1>(&flags1[0], 4);

		int textureChunkSize = 0;

		for (uint4 i = 0; i < 8; i++) {
			std::cout << ((flags1[0] >> 8 - (i + 1)) & 1);
		}
		std::cout << "\n";

		if ((flags1[0] >> 3) & 1) textureChunkSize += 26; // base texture chunk?
		if ((flags1[0]) & 1) textureChunkSize += 13; // some more bytes
		
		if (!textureChunkSize) return;

		bb->Position() = 32; // skip every flags and info from the beginning, we only need the texture ids

		uint4 debug = bb->Size() - bb->Position();
		uint4 texCount = (bb->Size() - bb->Position() + 1) / textureChunkSize;

		for (uint4 i = 0; i < texCount; i++) {
			bb->Position() += textureChunkSize - 4;

			int4 temp_id;
			bb->Read<int4>(&temp_id);

			TextureList.push_back({ temp_id, archive->SearchEntryByIDandType(temp_id, EntryType::TEXTURE) });
		}
	}

}
