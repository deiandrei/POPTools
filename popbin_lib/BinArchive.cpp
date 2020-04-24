#include "BinArchive.h"

namespace popbin {
	BinArchive::BinArchive(std::istream& in) {
		std::istream* dataStream = nullptr;

		// check compressed vs uncompressed library
		uint4 magic = 4009738393;

		in.seekg(0, in.beg);
		uint4 dec_size, enc_size;
		in.read((char*)(&dec_size), sizeof(uint4));
		in.read((char*)(&enc_size), sizeof(uint4));
		in.seekg(13, in.beg);
		uint4 file_magic; in.read((char*)(&file_magic), sizeof(uint4));
		in.seekg(14, in.beg);
		uint4 file_magic2; in.read((char*)(&file_magic2), sizeof(uint4));

		if ((file_magic == magic || file_magic2 == magic) && dec_size != enc_size)
		{
			uint4 size;
			byte1* decompressed_data = DecompressLzoBuffer(in, size);
			dataStream = new std::binarystream(decompressed_data, size);
		}
		else {
			dataStream = &in;
		}

		ReadEntries(dataStream);
		CheckEntriesLinks();
		
	}

	uint4 BinArchive::GetStreamSize(std::istream& in) {
		uint4 lastPos = (uint4)in.tellg();
		in.seekg(0, std::ios_base::end);
		uint4 length = (uint4)in.tellg();
		in.seekg(lastPos);
		return length;
	}

	int BinArchive::SearchEntryByID(int4 id) {
		for (int i = 0; i < (int)Entries.size(); ++i) {
			if (Entries[i].fileID == id) return i;
		}

		return -1;
	}

	byte1* BinArchive::DecompressLzoBuffer(std::istream& in, uint4& bufferSize) {
		std::vector<byte1> memoryBuffer;
		in.seekg(0, std::ios::beg);
		int size_dec;

		do {
			in.read((char*)(&size_dec), sizeof(int));
			int size_enc; in.read((char*)(&size_enc), sizeof(int));
			byte1* compressed = new byte1[size_enc];
			in.read((char*)& compressed[0], size_enc);

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

		byte1* finalBuff = new byte1[memoryBuffer.size()];
		bufferSize = (uint4)memoryBuffer.size();
		memcpy(&finalBuff[0], memoryBuffer.data(), memoryBuffer.size());
		return finalBuff;

		return nullptr;
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

	void BinArchive::ReadEntries(std::istream* in) {
		in->seekg(0, std::ios::beg);
		uint4 streamSize = GetStreamSize(*in);

		while ((uint4)in->tellg() < streamSize - 1) {
			Entry entry;

			//for easy debugging
			entry.entry_beginPos = (uint4)in->tellg();
			//

			in->read((char*)&entry.size, sizeof(uint4));
			in->read((char*)&entry.magic, sizeof(int4));
			in->read((char*)&entry.fileID, sizeof(int4));

			if (entry.size > 0) {
				entry.data_beginPos = (uint4)in->tellg();

				entry.data = new byte1[entry.size];
				in->read((char*)entry.data, entry.size);
			}

			//for easy debugging
			entry.entry_endPos = (uint4)in->tellg();
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
}