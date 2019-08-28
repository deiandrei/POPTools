#pragma once

#include "BFHeader.h"

struct FileIdOffsetTable {
	std::vector<uint4> filePos;
	std::vector<uint4> fileId;
	std::vector<uint4> filePos_offset;
	std::vector<uint4> fileId_offset;
	std::vector<int> id_grs;
	int entry;
	int data_length;
	int total_length;

	void ReadFromFile(std::ifstream& in, BFHeader& header) {
		filePos.resize(header.fcount);
		fileId.resize(header.fcount);
		filePos_offset.resize(header.fcount);
		fileId_offset.resize(header.fcount);
		id_grs.resize(header.fcount, -1);
		entry = (int)in.tellg();

		std::cout << "Reading FileIdOffsetTable...";
		for (uint4 id = 0; id < header.fcount; ++id) {
			filePos_offset[id] = (uint4)in.tellg();
			in.read((char*)(&filePos[id]), sizeof(uint4));
			fileId_offset[id] = (uint4)in.tellg();
			in.read((char*)(&fileId[id]), sizeof(uint4));
		}
		std::cout << "done!\n";

		std::cout << "Skipping FileIdOffsetTable dummy area...";
		data_length = (int)in.tellg() - entry;
		in.seekg(in.tellg() + (std::streampos)(header.capacity - header.fcount) * 8);
		total_length = (int)in.tellg();
		std::cout << "done!\n";
	}

};
