#pragma once

#include "FileIdOffsetTable.h"
#include "FileEntry.h"

struct SizeGrsTable {
	std::vector<uint4> fileId;
	std::vector<uint4> fileData_length;
	std::vector<uint4> fileId_offset;
	std::vector<uint4> fileData_length_offset;
	int filesCount = 0;
	std::vector<int> id_bf;

	int entry;
	int data_length;
	int total_length;

	void ReadFromFile(std::ifstream& in, BFHeader& header, FileIdOffsetTable& filePosId, std::vector<FileEntry>& files) {
		std::cout << "Looking for size.grs table offset";
		int pos = 0;
		for (int i = 0; i < (int)header.fcount; ++i) {
			if (std::string(files[i].name) == "size.grs") {
				pos = i;
				break;
			}
		}

		in.seekg((std::streampos)(filePosId.filePos[pos] + 4));
		std::cout << "done!\n";

		//first read blocks of 8bytes one by one to count the files ( keep in mind that these 8byte blocks are basically 2 blocks of 4bytes representing (fileId - 4bytes, fileData_length - 4bytes)
		filesCount = 0;
		std::cout << "Checking files count in size.grs ..";
		int8 tmp;
		in.read((char*)(&tmp), sizeof(int8));
		while (tmp != 0) {
			filesCount++;
			in.read((char*)(&tmp), sizeof(int8));
		}
		std::cout << "done!\n";

		fileId.resize(filesCount);
		fileData_length.resize(filesCount);
		fileId_offset.resize(filesCount);
		fileData_length_offset.resize(filesCount);

		id_bf.resize(filesCount);

		entry = (int)filePosId.filePos[pos] + 4;
		total_length = (int)files[pos].size;

		//now come back to the same starting position and read the blocks independently
		in.seekg((std::streampos)(filePosId.filePos[pos] + 4));

		std::cout << "Reading size.grs table...";
		for (uint4 id = 0; id < (uint4)filesCount; ++id) {
			fileId_offset[id] = (uint4)in.tellg();
			in.read((char*)(&fileId[id]), sizeof(uint4));
			fileData_length_offset[id] = (uint4)in.tellg();
			in.read((char*)(&fileData_length[id]), sizeof(uint4));
		}
		data_length = (int)in.tellg() - entry;
		std::cout << "done!\n";
	}
};
