#pragma once

#include "defines.h"
#include "ByteBuffer.h"

namespace popbin {
	class BinArchive;
	class AbstractModel;

	enum EntryType {
		EMPTY,
		UNKNOWN,
		WOW,
		DESCRIPTOR,
		GAO,
		GEOMETRY,
		TEXTURE_PACK,
		TEXTURE_INFO,
		TEXTURE_HEADER,
		TEXTURE_PALETTE,
		TEXTURE,
		TERMINATOR,
		NUM_TYPES
	};

	struct Entry {
		uint4 size;
		int4 magic;
		int4 fileID;
		EntryType type = EntryType::UNKNOWN;
		byte1* data;
		
		BinArchive* parentArchive;

		AbstractModel* model;

		// For debugging
		uint4 entry_beginPos;
		uint4 entry_endPos;
		uint4 data_beginPos;

		//Utils
		char name[64];

		Entry() {
			size = 0;
			magic = 0;
			fileID = 0;
			data = nullptr;
			model = nullptr;

			name[0] = '\0';

			entry_beginPos = entry_endPos = data_beginPos = 0;
		}

	};

	struct EntryLink {
		Entry* LinkedEntry;
		std::vector<uint4> LinkPositions;

		EntryLink() {
			LinkedEntry = nullptr;
		}
	};
}