#pragma once

#include "FileTable.h"
#include "File.h"

namespace bf {
	struct ResourcesTable {
		std::vector<uint4> fileId;
		std::vector<uint4> fileData_length;

		int filesCount = 0;
		std::vector<int> bridge_id_bf; //file index in FileTable where this->fileId == FileTable.fileId

		int bf_entry;
		int data_length;
		int total_length;

		void ReadFromFile(std::ifstream& in, Header& header, FileTable& filePosId, std::vector<File>& files) {
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

			//first read blocks of 8bytes one by one to count the files ( keep in mind that these 8byte blocks are basically 2 blocks of 4bytes representing (fileId - 4bytes, fileData_length - 4bytes) )
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
			bridge_id_bf.resize(filesCount);

			bf_entry = (int)filePosId.filePos[pos] + 4;
			total_length = (int)files[pos].size;

			//now come back to the same starting position and read the blocks independently
			in.seekg((std::streampos)(filePosId.filePos[pos] + 4));

			std::cout << "Reading size.grs table...";
			for (uint4 id = 0; id < (uint4)filesCount; ++id) {
				in.read((char*)(&fileId[id]), sizeof(uint4));
				in.read((char*)(&fileData_length[id]), sizeof(uint4));
			}
			data_length = (int)in.tellg() - bf_entry;
			std::cout << "done!\n";
		}
	};
}
