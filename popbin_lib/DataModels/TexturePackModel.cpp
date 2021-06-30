#include "TexturePackModel.h"
#include "../BinArchive.h"

namespace popbin {
	TexturePackModel::TexturePackModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;

		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		uint4 texCount = 0;

		bb->Position() += 8;
		bb->Read<uint4>(&texCount);

		for (int i = 0; i < texCount; ++i) {
			uint4 texId;
			bb->Read<uint4>(&texId);

			uint4 texId_search = archive->SearchEntryByID(texId);
			if (texId_search != -1) TextureList.insert({ texId, texId_search });
		}

		std::cout << "Texture Pack " << std::hex << mEntry->fileID << std::dec << " has " << TextureList.size() << "/" << texCount << " textures inside.\n";
	}

}
