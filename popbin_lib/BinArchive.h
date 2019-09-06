#pragma once

#include "ByteBuffer.h"
#include "../Dependencies/lzolib/include/lzo/lzodefs.h"
#include "../Dependencies/lzolib/include/lzo/lzoconf.h"
#include "../Dependencies/lzolib/include/lzo/lzo1x.h"

#include "File.h"

namespace popbin {
	class BinArchive {
		public:
			BinArchive(const std::string& fileName);
			~BinArchive() { }

			std::vector<File> Files;

		private:
			byte1* DecompressLzoBuffer(std::ifstream& in, uint4& bufferSize);
			uint4 GetStreamSize(std::istream& in);
			void ParseFiles(ByteBuffer* buffer);

		protected:
			byte1* mData;
			uint4 mDataSize;

	};
}
