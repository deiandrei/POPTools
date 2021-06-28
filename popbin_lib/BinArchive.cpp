#include "BinArchive.h"

namespace popbin {
	BinArchive::BinArchive(std::istream& in) {
		uint4 dataSize = GetStreamSize(in);
		byte1* dataBuff = new byte1[dataSize];
		in.seekg(0, std::ios_base::beg);
		in.read((char*)dataBuff, dataSize);
		
		mDataBuffer = new ByteBuffer(dataBuff, dataSize);

		// check compressed vs uncompressed library
		uint4 magic = 4009738393;
		
		uint4 dec_size, enc_size, file_magic, file_magic2;
		mDataBuffer->Read<uint4>(&dec_size);
		mDataBuffer->Read<uint4>(&enc_size);
		mDataBuffer->Position() = 13;
		mDataBuffer->Read<uint4>(&file_magic);
		mDataBuffer->Position() = 14;
		mDataBuffer->Read<uint4>(&file_magic2);

		if ((file_magic == magic || file_magic2 == magic) && dec_size != enc_size)
		{
			ByteBuffer* decompressed_data = DecompressLzoBuffer(mDataBuffer);

			delete mDataBuffer;
			mDataBuffer = decompressed_data;
		}

		ReadEntries();
		//CheckEntriesLinks();
		
	}

	uint4 BinArchive::GetStreamSize(std::istream& in) {
		uint4 lastPos = (uint4)in.tellg();
		in.seekg(0, std::ios_base::end);
		uint4 length = (uint4)in.tellg();
		in.seekg(lastPos);
		return length;
	}

	std::string BinArchive::EntryType(int4 type) {
		switch (type) {
			case 2003793710:
				return ".wow";
				break;

			default:
				return std::string();
				break;
		}
	}

	int BinArchive::SearchEntryByID(int4 id) {
		for (int i = 0; i < (int)Entries.size(); ++i) {
			if (Entries[i].fileID == id) return i;
		}

		return -1;
	}

	void BinArchive::ExtractEntry(int id, const std::string& savePath) {
		if (id < 0 || id >= (int)Entries.size()) return;

		std::stringstream entryName;
		entryName << id << "_" << std::hex << Entries[id].fileID << ".rawbin";

		std::ofstream writer(savePath + "/" + entryName.str(), std::ios::binary);

		if (!writer.is_open()) return;

		writer.write((char*)(&Entries[id].data[0]), sizeof(byte1) * Entries[id].size);

		writer.close();
	}

	ByteBuffer* BinArchive::DecompressLzoBuffer(ByteBuffer* buff) {
		std::vector<byte1> memoryBuffer;
		uint4 size_dec, size_enc;

		buff->Position() = 0;

		do {
			buff->Read<uint4>(&size_dec);
			buff->Read<uint4>(&size_enc);
			
			byte1* compressed = new byte1[size_enc];
			buff->ReadArray<byte1>(compressed, size_enc);

			//wtf are those data blocks?!
			if (size_dec == size_enc) {
				std::copy(compressed, compressed + size_enc, std::back_inserter(memoryBuffer));
			}

			else if (size_enc < 131072) {
				int origlen = size_dec;
				byte1* dst = new byte1[origlen];
				uint8 outlen = origlen;

				int status = lzo1x_decompress(compressed, size_enc, dst, &outlen, nullptr);
				if (status != 0) {
					std::cout << "Error at LZO!\n";
					return nullptr;
				}

				std::copy(dst, dst + size_dec, std::back_inserter(memoryBuffer));
				delete[] dst;
			}

			delete[] compressed;
		} while (size_dec == 131072);

		ByteBuffer* finalBuff = new ByteBuffer(memoryBuffer.data(), (uint4)memoryBuffer.size(), true);
		return finalBuff;
	}

	void BinArchive::AddEntryLink(Entry* entry, Entry* linkEntry, uint4 linkPos) {
		if(EntriesLinks.find(entry) == EntriesLinks.end()) {
			EntriesLinks.insert(std::pair<Entry*, std::vector<EntryLink>>(entry, std::vector<EntryLink>()));
		}
		
		auto& links = EntriesLinks[entry];
		int linkID = -1;
		for (int i = 0; i < (int)links.size(); ++i) {
			if (links[i].LinkedEntry == linkEntry) {
				linkID = i;
				break;
			}
		}
		if (linkID == -1) {
			EntryLink link;
			link.LinkedEntry = linkEntry;
			links.push_back(link);
			linkID = (int)links.size() - 1;
		}

		//Add link position
		links[linkID].LinkPositions.push_back(linkPos);
	}

	void BinArchive::ReadEntries() {
		mDataBuffer->Position() = 0;

		while (!mDataBuffer->EndReached()) {
			Entry entry;

			//for easy debugging
			entry.entry_beginPos = mDataBuffer->Position();
			//

			mDataBuffer->Read<uint4>(&entry.size);
			mDataBuffer->Read<int4>(&entry.magic);
			mDataBuffer->Read<int4>(&entry.fileID);

			mEntriesFileIDs.insert({ entry.fileID, (int)Entries.size() });

			if (entry.size > 0) {
				entry.data_beginPos = mDataBuffer->Position();

				entry.data = new byte1[entry.size];
				mDataBuffer->ReadArray<byte1>(entry.data, entry.size);
			}

			//for easy debugging
			entry.entry_endPos = mDataBuffer->Position();
			//
			
			Entries.push_back(entry);
		}

		std::cout << Entries.size() << " Entries in this bin.\n";
	}

	void BinArchive::CheckEntriesLinks() {
		for (auto& entry : Entries) {
			std::binarystream dataStream(entry);
			int4 buffer;

			for (auto& linkEntry : Entries) {
				if (linkEntry.fileID == entry.fileID) continue;

				dataStream.seekg(0, std::ios::beg);
				
				uint4 streamPos = (uint4)dataStream.tellg();
				while (streamPos < entry.size) {
					dataStream.read((char*)&buffer, sizeof(int4));

					//if link found
					if (buffer == entry.fileID) {
						AddEntryLink(&entry, &linkEntry, (uint4)dataStream.tellg() - sizeof(int4));
					}

					dataStream.seekg(-3, std::ios::cur);
					streamPos = (uint4)dataStream.tellg();
				}
			}
		}

		int linkposCounts = 0;
		for (auto itr = EntriesLinks.begin(); itr != EntriesLinks.end(); itr++) {
			for (auto link : itr->second) {
				linkposCounts += (int)link.LinkPositions.size();
			}
		}

		std::cout << EntriesLinks.size() << " Links found and " << linkposCounts << " positions. \n";
	}

	void BinArchive::TryParseLinksHeader() {
		std::binarystream bs(Entries[3]);

		int linksCount = (bs.size() / 4) - 1;
		std::vector<int4> fileIds;
		for (int i = 0; i < linksCount; ++i) {
			int4 temp;
			bs.read((char*)&temp, sizeof(int4));
			fileIds.push_back(temp);
		}

		int found = 0;
		for (auto& id : fileIds) {
			if (mEntriesFileIDs.find(id) != mEntriesFileIDs.end()) {
				found++;
			}
		}

		std::cout << "\nFound " << found << " out of " << (int)mEntriesFileIDs.size() << " in the links header! \n";
	}
}