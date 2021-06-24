#pragma once

#include <istream>
#include <map>

#include "ByteBuffer.h"
#include "../Dependencies/lzolib/include/lzo/lzodefs.h"
#include "../Dependencies/lzolib/include/lzo/lzoconf.h"
#include "../Dependencies/lzolib/include/lzo/lzo1x.h"

#include "Entry.h"

namespace std {
	class binarystream : public istringstream {
		public:
			binarystream(byte1* buffer, uint4 size) : istringstream((std::string((char*)buffer, size)), std::ios::binary) { mSize = size; }
			binarystream(popbin::Entry& entry) : istringstream((std::string((char*)entry.data, entry.size)), std::ios::binary) { mSize = entry.size; }

			uint4 size() { return mSize; }

		protected:
			uint4 mSize;

	};
}

namespace popbin {
	class BinArchive {
		public:
			BinArchive(std::istream& in);
			~BinArchive() { }

			std::vector<Entry> Entries;
			std::map<Entry*, std::vector<EntryLink>> EntriesLinks;

			static uint4 GetStreamSize(std::istream& in);

			int SearchEntryByID(int4 id);

			byte1* GetData() { return mData; }
			uint4 GetDataSize() { return mDataSize; }

		private:
			byte1* DecompressLzoBuffer(std::istream& in, uint4& bufferSize);
			void AddEntryLink(Entry* entry, Entry* linkEntry, uint4 linkPos);

			void ReadEntries(std::istream* in);
			void CheckEntriesLinks();
			//void ParseFiles(ByteBuffer* buffer);

			byte1* mData;
			uint4 mDataSize;

	};
}
