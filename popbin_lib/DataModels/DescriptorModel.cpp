#include "DescriptorModel.h"
#include "../BinArchive.h"

namespace popbin {
	DescriptorModel::DescriptorModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;

		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		int idsFound = 0;

		for (int i = 0; i < mEntry->size / 4; ++i) {
			uint4 id_temp;
			bb->Read<uint4>(&id_temp);

			uint4 id_search = archive->SearchEntryByID(id_temp);
			if (id_search != -1) idsFound++;
		}

		std::cout << "Descriptor Ids Found: " << idsFound << "/" << mEntry->size / 4 << "\n";
	}

}
