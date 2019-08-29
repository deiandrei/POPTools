#pragma once

#include "Header.h"

namespace bf {
	struct FileTable {
		std::vector<uint4> filePos;
		std::vector<uint4> fileId;

		std::vector<int> bridge_id_grs;

		int entry;
		int data_length;
		int total_length;

		void ReadFromFile(std::ifstream& in, Header& header) {
			filePos.resize(header.fcount);
			fileId.resize(header.fcount);
			bridge_id_grs.resize(header.fcount, -1);

			entry = (int)in.tellg();

			std::cout << "Reading FileIdOffsetTable...";
			for (uint4 id = 0; id < header.fcount; ++id) {
				in.read((char*)(&filePos[id]), sizeof(uint4));
				in.read((char*)(&fileId[id]), sizeof(uint4));
			}
			std::cout << "done!\n";

			data_length = (int)in.tellg() - entry;

			std::cout << "Skipping FileIdOffsetTable dummy area...";
			in.seekg(in.tellg() + (std::streampos)(header.capacity - header.fcount) * 8);
			std::cout << "done!\n";

			total_length = (int)in.tellg();
		}

	};
}
