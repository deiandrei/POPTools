#include "BinArchive.h"

namespace popbin {
	BinArchive::BinArchive(const std::string& fileName) {
		mData = nullptr;
		mDataSize = 0;

		std::ifstream in(fileName, std::ios::binary);

		uint4 magic = 4009738393;
		in.seekg(13, in.beg);
		uint4 file_magic; in.read((char*)(&file_magic), sizeof(uint4));
		in.seekg(14, in.beg);
		uint4 file_magic2; in.read((char*)(&file_magic2), sizeof(uint4));

		//compressed vs uncompressed files
		if (file_magic == magic || file_magic2 == magic)
		{
			mData = DecompressLzoBuffer(in, mDataSize);
		}
		else {
			in.seekg(0, in.beg);
			mDataSize = GetStreamSize(in);
			mData = new byte1[mDataSize];
			in.read((char*)(&mData[0]), mDataSize);
		}

		in.close();

		ByteBuffer* bbuffer = new ByteBuffer(mData, mDataSize);

		Files = ReadFilesFromBuffer(bbuffer);
		std::cout << "Objcount " << Files.size();

		ParseFiles(bbuffer);
	}

	void BinArchive::ParseFiles(ByteBuffer* buffer) {
		bool effect_table_found = false;

		for (File& file : Files) {
			//.wow object
			if (file.data == 0x776F772E) {
				uint4 name_offset = file.data_entry + 12;
				
				buffer->Position() = name_offset;


			}
		}
	}

	uint4 BinArchive::GetStreamSize(std::istream& in) {
		uint4 lastPos = (uint4)in.tellg();
		in.seekg(0, std::ios_base::end);
		uint4 length = (uint4)in.tellg();
		in.seekg(lastPos);
		return length;
	}

	byte1* BinArchive::DecompressLzoBuffer(std::ifstream& in, uint4& bufferSize) {
		uint4 magic = 4009738393;

		in.seekg(0, in.beg);
		uint4 dec_size, enc_size;
		in.read((char*)(&dec_size), sizeof(uint4));
		in.read((char*)(&enc_size), sizeof(uint4));
		in.seekg(13, in.beg);
		uint4 file_magic; in.read((char*)(&file_magic), sizeof(uint4));
		in.seekg(14, in.beg);
		uint4 file_magic2; in.read((char*)(&file_magic2), sizeof(uint4));

		//blacklisting non-compressed files
		if ((dec_size != enc_size && file_magic == magic) || (dec_size != enc_size && file_magic2 == magic))
		{
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
		}

		return nullptr;
	}
}