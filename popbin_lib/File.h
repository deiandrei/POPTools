#pragma once

#include "defines.h"
#include "ByteBuffer.h"

namespace popbin {
	struct File {
		int4 size;
		int4 magic;
		uint4 fileId;

		int4 data_entry;
		int4 data_end;

		uint4 data;
		char name[64];
		uint4 name_len;

		char description[64];
		char type[64];
	};

	std::vector<File> ReadFilesFromBuffer(ByteBuffer* buffer);

}