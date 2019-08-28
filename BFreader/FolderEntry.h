#pragma once

#include "BFHeader.h"

struct FolderEntry {
	uint4 file;
	int lastChild;
	int nextFolder;
	int prevFolder;
	int parentFolder;
	char name[64];

	uint4 file_offset;
	uint4 child_offset;
	uint4 next_offset;
	uint4 prev_offset;
	uint4 parent_offset;
	uint4 name_offset;

	int entry;
	int data_length;
	int total_length;
};

std::vector<FolderEntry> ReadFolderEntriesFromFile(std::ifstream& in, BFHeader& header, int& folderEntryUnkOffset, int& folderEntryUnk, int& headerTotalLength) {
	std::vector<FolderEntry> folderEntries;

	std::cout << "Checking folders count in FolderEntry table...";
	uint4 folders_count = 0;
	uint4 startpos = (uint4)in.tellg();
	in.seekg(in.tellg() + (std::streampos)20);

	int temp;
	in.read((char*)(&temp), sizeof(int));
	while (temp != 0) {
		in.seekg(in.tellg() + (std::streampos)80);
		folders_count++;
		in.read((char*)(&temp), sizeof(int));
	}
	in.seekg((std::streampos)startpos);
	std::cout << "done!\n";

	folderEntries.resize(folders_count);

	std::cout << "Reading FolderEntry table...";
	for (uint4 id = 0; id < folders_count; ++id) {
		folderEntries[id].entry = (int)in.tellg();
		folderEntries[id].file_offset = (uint4)in.tellg();
		in.read((char*)(&folderEntries[id].file), sizeof(uint4));
		folderEntries[id].child_offset = (uint4)in.tellg();
		in.read((char*)(&folderEntries[id].lastChild), sizeof(int));
		folderEntries[id].next_offset = (uint4)in.tellg();
		in.read((char*)(&folderEntries[id].nextFolder), sizeof(int));
		folderEntries[id].prev_offset = (uint4)in.tellg();
		in.read((char*)(&folderEntries[id].prevFolder), sizeof(int));
		folderEntries[id].parent_offset = (uint4)in.tellg();
		in.read((char*)(&folderEntries[id].parentFolder), sizeof(int));
		folderEntries[id].name_offset = (uint4)in.tellg();
		in.read((char*)(&folderEntries[id].name), sizeof(folderEntries[id].name));
	}
	folderEntries[0].data_length = (int)in.tellg() - folderEntries[0].entry;
	std::cout << "done!\n";

	std::cout << "Skipping FolderEntry table dummy area...";
	in.seekg(in.tellg() + (std::streampos)(header.capacity - folders_count) * 84);
	folderEntries[0].total_length = (int)in.tellg() - folderEntries[0].entry;

	//wtf is this
	in.seekg(in.tellg() - (std::streampos)68);
	folderEntryUnkOffset = (int)in.tellg() - 68;
	in.read((char*)(&folderEntryUnk), sizeof(int));
	in.seekg(in.tellg() + (std::streampos)64);
	
	std::cout << "done!\n";

	std::cout << "Skipping remaining dummy area and reading header total size block...";
	in.read((char*)(&temp), sizeof(int));
	while (temp == 0) {
		in.read((char*)(&temp), sizeof(int));
	}

	in.seekg(in.tellg() - (std::streampos)4);
	in.read((char*)(&headerTotalLength), sizeof(int));
	std::cout << "done!\n";

	return folderEntries;
}
