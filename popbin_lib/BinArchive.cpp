#include "BinArchive.h"

#include "DataModels/DescriptorModel.h"
#include "DataModels/TexturePackModel.h"
#include "DataModels/GeometryModel.h"
#include "DataModels/TextureModel.h"
#include "DataModels/TexturePaletteModel.h"
#include "DataModels/GameActorModel.h"

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
		ParseEntries();
	}

	uint4 BinArchive::GetStreamSize(std::istream& in) {
		uint4 lastPos = (uint4)in.tellg();
		in.seekg(0, std::ios_base::end);
		uint4 length = (uint4)in.tellg();
		in.seekg(lastPos);
		return length;
	}

	EntryType BinArchive::IntToEntryType(int4 type) {
		if (type == 1) { //=1
			return EntryType::GEOMETRY;
		}
		else if (type == 4) { //texturepack
			return EntryType::TEXTURE_PACK;
		}
		else if (type == 5) { //textureinfo ??
			return EntryType::TEXTURE_INFO;
		}
		else if (type == 0x0FF7C0DE) {
			return EntryType::TERMINATOR;
		}
		else if (type == 0x6F61672E) { //.gao
			return EntryType::GAO;
		}

		return EntryType::UNKNOWN;
	}

	std::string BinArchive::EntryTypeToString(EntryType type) {
		if (type == EntryType::EMPTY) {
			return "Empty";
		}
		else if (type == EntryType::GAO) {
			return "GameActorObject";
		}
		else if (type == EntryType::GEOMETRY) {
			return "Geometry";
		}
		else if (type == EntryType::WOW) {
			return "WOW";
		}
		else if (type == EntryType::TEXTURE_PACK) {
			return "TexturePack";
		}
		else if (type == EntryType::TEXTURE_INFO) {
			return "TextureInfo";
		}
		else if (type == EntryType::TEXTURE_HEADER) {
			return "TextureHeader";
		}
		else if (type == EntryType::TEXTURE_PALETTE) {
			return "TexturePalette";
		}
		else if (type == EntryType::TEXTURE) {
			return "Texture";
		}
		else if (type == EntryType::DESCRIPTOR) {
			return "Descriptor";
		}
		else if (type == EntryType::TERMINATOR) {
			return "Terminator";
		}
		else if (type == EntryType::UNKNOWN) {
			return "Unknown";
		}

		return "Undefined";
	}

	int BinArchive::SearchEntryByID(int4 id) {
		for (int i = 0; i < (int)Entries.size(); ++i) {
			if (Entries[i].fileID == id) return i;
		}

		return -1;
	}

	int BinArchive::SearchEntryByIDandType(int4 id, EntryType type) {
		for (int i = 0; i < (int)Entries.size(); ++i) {
			if (Entries[i].fileID == id && Entries[i].type == type) return i;
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

			entry.parentArchive = this;

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

	void BinArchive::ParseEntries() {
		for (int i = 0; i < (int)Entries.size(); i++) {
			Entry& entry = Entries[i];

			if (entry.size) {
				int4 temp;
				memcpy(&temp, entry.data, sizeof(int4));

				if (i == 3) { // special case for the descriptor
					entry.type = EntryType::DESCRIPTOR;
				}
				else {

					if (temp == 1) { //=1
						entry.type = EntryType::GEOMETRY;
					}
					else if (temp == 4) { //texturepack
						entry.type = EntryType::TEXTURE_PACK;
					}
					else if (temp == 5) { //textureinfo ??
						entry.type = EntryType::TEXTURE_INFO;
					}
					else if (temp == 0x0FF7C0DE) {
						entry.type = EntryType::TERMINATOR;
					}
					else if (temp == 0x6F61672E) { //.gao
						entry.type = EntryType::GAO;
					}
					else if (temp == entry.fileID) { //this is interesting
						// decide what kind of texture info it is
						int4 next_bytes;
						memcpy(&next_bytes, entry.data + 4, sizeof(int4));

						if (next_bytes == 0xFFFFFFFF) {
							if (entry.size <= 64) {
								entry.type = EntryType::TEXTURE_HEADER;
							}
							else {
								entry.type = EntryType::TEXTURE;
							}
						}
						else {
							entry.type = EntryType::TEXTURE_PALETTE;
						}

					}
					else {
						entry.type = EntryType::UNKNOWN;
					}

				}
			}
			else {
				entry.type = EntryType::EMPTY;
			}

			ParseEntry(i, entry.type);
		}

	}

	void BinArchive::ParseEntry(int id, EntryType type) {
		Entry& entry = Entries[id];
		
		if (type == EntryType::DESCRIPTOR) {
			entry.model = new DescriptorModel(&entry);
		}
		else if (type == EntryType::TEXTURE_PACK) {
			entry.model = new TexturePackModel(&entry);
		}
		else if (type == EntryType::TEXTURE) {
			entry.model = new TextureModel(&entry);
		}
		else if (type == EntryType::TEXTURE_PALETTE) {
			entry.model = new TexturePaletteModel(&entry);
		}
		else if (type == EntryType::GAO) {
			entry.model = new GameActorModel(&entry);
		}
		else if (type == EntryType::GEOMETRY) {
			entry.model = new GeometryModel(&entry);
		}
	}

}