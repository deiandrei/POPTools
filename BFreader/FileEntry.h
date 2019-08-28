#pragma once

#include "BFHeader.h"

struct FileEntry {
	uint4 size;
	int next;
	int prev;
	uint4 folder;
	uint4 timestamp;
	char name[64];

	uint4 size_offset;
	uint4 next_offset;
	uint4 prev_offset;
	uint4 folder_offset;
	uint4 timestamp_offset;
	uint4 name_offset;

	int entry;
	int data_length;
	int total_length;

	int fileData_length;

	byte1* data;
};

std::vector<FileEntry> ReadFileEntriesFromFile(std::ifstream& in, BFHeader& header) {
	std::vector<FileEntry> fileEntries;
	fileEntries.resize(header.fcount);

	std::cout << "Reading FileEntry table...";
	for (uint4 id = 0; id < header.fcount; ++id) {
		fileEntries[id].entry = (int)in.tellg();
		fileEntries[id].size_offset = (uint4)in.tellg();
		in.read((char*)(&fileEntries[id].size), sizeof(uint4));
		fileEntries[id].next_offset = (uint4)in.tellg();
		in.read((char*)(&fileEntries[id].next), sizeof(int));
		fileEntries[id].prev_offset = (uint4)in.tellg();
		in.read((char*)(&fileEntries[id].prev), sizeof(int));
		fileEntries[id].folder_offset = (uint4)in.tellg();
		in.read((char*)(&fileEntries[id].folder), sizeof(uint4));
		fileEntries[id].timestamp_offset = (uint4)in.tellg();
		in.read((char*)(&fileEntries[id].timestamp), sizeof(uint4));
		fileEntries[id].name_offset = (uint4)in.tellg();
		in.read((char*)(&fileEntries[id].name), sizeof(fileEntries[id].name));
	}

	fileEntries[0].data_length = (int)in.tellg() - fileEntries[0].entry;
	std::cout << "done!\n";

	std::cout << "Skipping FileEntry table dummy area...";
	in.seekg(in.tellg() + (std::streampos)(header.capacity - header.fcount) * 84);
	fileEntries[0].total_length = (int)in.tellg() - fileEntries[0].entry;
	std::cout << "done!\n";

	return fileEntries;
}
