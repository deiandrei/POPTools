#ifndef BFUTILS_H
#define BFUTILS_H

#include "ResourcesTable.h"
#include "Header.h"
#include "File.h"
#include "Folder.h"

namespace bf {
	void LinkIds(bf::ResourcesTable& grsTable, bf::FileTable& filePosId, bf::Header& header);

	std::string GetFullFolderPath(std::vector<bf::Folder>& folderEntries, bf::Folder folder);

	void ParseFilesData(bf::Header& header, bf::FileTable& fileIdOffsetTable, bf::ResourcesTable& grsTable, std::vector<bf::File>& files);

	void SaveFile(bf::File& file);
}

#endif
