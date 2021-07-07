#include "GameActorModel.h"
#include "../BinArchive.h"

namespace popbin {
	GameActorModel::GameActorModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;
		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		bb->Position() = 16; // skip the first 16 bytes

		uint4 name_len;
		bb->Read<uint4>(&name_len);
		bb->ReadArray<char>(&entry->name[0], name_len);

		bb->Position() += 10; // skip the next unknown 10 bytes
		bb->ReadArray<float>(&Matrix[0], 16);

		uint4 data_flags;
		bb->Read<uint4>(&data_flags);

		/*for (int i = 0; i < pos_count; i += 3) {
			float temp1, temp2, temp3;
			bb->Read<float>(&temp1);
			bb->Read<float>(&temp2);
			bb->Read<float>(&temp3);

			Vectors.push_back(vec3(temp1, temp2, temp3));
		}*/
	}

}
