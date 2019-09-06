#include "File.h"

namespace popbin {
	std::vector<File> ReadFilesFromBuffer(ByteBuffer* buffer) {
		std::vector<File> files;
		int objCount = 0;

		int tmpSize;
		buffer->Position() = 0;
		while (buffer->Position() < buffer->Size() - 1) {
			tmpSize = buffer->Read<int>();
			buffer->Position() += tmpSize + 8;
			objCount++;
		}

		files.resize(objCount);
		buffer->Position() = 0;
		for (size_t i = 0; i < files.size(); ++i) {
			files[i].size = buffer->Read<int4>();
			files[i].magic = buffer->Read<int4>();
			files[i].fileId = buffer->Read<uint4>();
			files[i].data_entry = (int4)buffer->Position();

			if (files[i].size != 0) {
				files[i].data = (uint4)buffer->Read<int4>();
			}

			buffer->Position() = files[i].data_entry + files[i].size;
			files[i].data_end = buffer->Position();
		}

		return files;
	}

}

