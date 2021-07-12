#include "BinArchive.h"

namespace popbin {

	std::string Entry::GetDebugName(int id) {
		std::stringstream strr;
		strr << (id != -1 ? std::to_string(id) + "_" : "") << BinArchive::EntryTypeToString(type) << "_" << std::hex << fileID << std::dec << (strlen(name) > 0 ? "_" + std::string(name) : "");
		return strr.str();
	}

}
