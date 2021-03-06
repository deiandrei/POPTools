#include "File.h"

namespace bf {
	std::vector<File> ReadFilesFromFile(std::istream& in, Header& header) {
		std::vector<File> fileEntries;
		fileEntries.resize(header.fcount);

		std::cout << "Reading FileEntry table...";
		for (uint4 id = 0; id < header.fcount; ++id) {
			fileEntries[id].entry = (int)in.tellg();
			in.read((char*)(&fileEntries[id].size), sizeof(uint4));
			in.read((char*)(&fileEntries[id].next), sizeof(int));
			in.read((char*)(&fileEntries[id].prev), sizeof(int));
			in.read((char*)(&fileEntries[id].folder), sizeof(uint4));
			in.read((char*)(&fileEntries[id].timestamp), sizeof(uint4));
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
}