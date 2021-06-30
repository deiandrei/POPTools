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
			~BinArchive() { delete mDataBuffer; }

			std::vector<Entry> Entries;
			std::map<Entry*, std::vector<EntryLink>> EntriesLinks;

			static uint4 GetStreamSize(std::istream& in);

			static EntryType IntToEntryType(int4 type);
			static std::string EntryTypeToString(EntryType type);

			int SearchEntryByID(int4 id);

			ByteBuffer* GetBuffer() { return mDataBuffer; }

			void ExtractEntry(int id, const std::string& savePath);
			void ExtractAllEntries(const std::string& savePath) { for (int i = 0; i < (int)Entries.size(); ++i) ExtractEntry(i, savePath); }

			void TryParseLinksHeader();

			void ParseEntries();

		private:
			ByteBuffer* DecompressLzoBuffer(ByteBuffer* buff);
			void AddEntryLink(Entry* entry, Entry* linkEntry, uint4 linkPos);

			void ReadEntries();
			void CheckEntriesLinks();
			
			void ParseEntry(int id, EntryType type);

			ByteBuffer* mDataBuffer;
			std::map<int4, int> mEntriesFileIDs;


	};
}
