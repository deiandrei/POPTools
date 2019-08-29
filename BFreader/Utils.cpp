#include "Utils.h"

namespace bf {
	void LinkIds(bf::ResourcesTable& grsTable, bf::FileTable& filePosId, bf::Header& header) {
		std::cout << "Linking FileEntry and size.grs table file Ids...";

		for (int i = 0; i < grsTable.filesCount; ++i) {
			for (int j = 0; j < header.fcount; ++j) {
				if (grsTable.fileId[i] == filePosId.fileId[j]) {
					grsTable.bridge_id_bf[i] = j;
					filePosId.bridge_id_grs[j] = i;
				}
			}
		}

		std::cout << "done!\n";
	}

	std::string GetFullFolderPath(std::vector<bf::Folder>& folderEntries, bf::Folder folder) {
		std::string name = folder.name;

		while (folder.parentFolder != -1) {
			folder = folderEntries[folder.parentFolder];
			name = folder.name + std::string("/") + name;
		}

		return name;
	}

	void ParseFilesData(bf::Header& header, bf::FileTable& fileIdOffsetTable, bf::ResourcesTable& grsTable, std::vector<bf::File>& files) {
		std::ifstream in("prince.bf", std::ios::in | std::ios::binary);

		std::cout << "Parsing files data length...";
		for (uint4 i = 0; i < header.fcount; ++i) {
			in.clear();

			in.seekg(fileIdOffsetTable.filePos[i]);

			byte1* data = new (std::nothrow) byte1[files[i].size];
			int grsId = fileIdOffsetTable.bridge_id_grs[i];
			if (grsId == -1) in.read((char*)(&data[0]), files[i].size);
			else in.read((char*)(&data[0]), grsTable.fileData_length[grsId]);

			files[i].data = (byte1*)data;
			//ParseFileData(header, fileIdOffsetTable, grsTable, files, i);
		}
		std::cout << "done!\n";
	}

	void SaveFile(bf::File& file) {
		std::ofstream out(file.name, std::ios::binary);

		out.write((char*)(&file.data[0]), sizeof(byte1) * file.size);
	}
}
