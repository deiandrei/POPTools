#include "TexturePaletteModel.h"
#include "../BinArchive.h"

namespace popbin {
	TexturePaletteModel::TexturePaletteModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;
		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		bb->Position() += 4; // skip the fileType which is equal to the fileID
		DataSize = bb->Size() - bb->Position();
		Data = new byte1[DataSize];
		bb->ReadArray<byte1>(Data, bb->Size() - bb->Position());
	}

}
