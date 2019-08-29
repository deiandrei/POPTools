#ifndef BFFILETABLE_H
#define BFFILETABLE_H

#include "Header.h"

namespace bf {
	struct FileTable {
		std::vector<uint4> filePos;
		std::vector<uint4> fileId;

		std::vector<int> bridge_id_grs;

		int entry;
		int data_length;
		int total_length;

		void ReadFromFile(std::ifstream& in, Header& header);

	};
}

#endif
