#ifndef BFFILE_H
#define BFFILE_H

#include "Header.h"

namespace bf {
	struct File {
		uint4 size;
		int next;
		int prev;
		uint4 folder;
		uint4 timestamp;
		char name[64];

		int entry;
		int data_length;
		int total_length;

		byte1* data;
		uint4 data_size;
	};

	std::vector<File> ReadFileEntriesFromFile(std::ifstream& in, Header& header);

	using FileList = std::vector<File>;
}

#endif