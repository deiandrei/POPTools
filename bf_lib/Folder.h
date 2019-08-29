#pragma once

#include "Header.h"

namespace bf {
	struct Folder {
		uint4 file;
		int lastChild;
		int nextFolder;
		int prevFolder;
		int parentFolder;
		char name[64];

		int entry;
		int data_length;
		int total_length;
	};

	std::vector<Folder> ReadFoldersFromFile(std::ifstream& in, Header& header);

	using FolderList = std::vector<Folder>;
}