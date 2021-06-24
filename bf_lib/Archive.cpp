#include "Archive.h"

namespace bf {
	Archive::Archive(const std::string& fileName) {
		std::ifstream reader(fileName, std::ios::in | std::ios::binary);
		mIsLoaded = true;

		if (!reader.is_open()) {
			mIsLoaded = false;
			return;
		}

		mHeader.ReadFromFile(reader);
		FileTables.ReadFromFile(reader, mHeader);
		Files = ReadFilesFromFile(reader, mHeader);
		Folders = ReadFoldersFromFile(reader, mHeader);
		ResourcesTables.ReadFromFile(reader, mHeader, FileTables, Files);

		LinkIds(mHeader, ResourcesTables, FileTables);
		ParseFilesData(reader, mHeader, FileTables, ResourcesTables, Files);

		reader.close();
	}

	Archive::~Archive() {
		if (!IsLoaded()) return;

		for (size_t i = 0; i < Files.size(); ++i) {
			delete Files[i].data;
		}
	}

	void Archive::LinkIds(bf::Header& header, bf::ResourcesTable& resTable, bf::FileTable& fileTable) {
		for (int i = 0; i < resTable.filesCount; ++i) {
			for (uint4 j = 0; j < header.fcount; ++j) {
				if (resTable.fileId[i] == fileTable.fileId[j]) {
					resTable.bridge_id_bf[i] = j;
					fileTable.bridge_id_grs[j] = i;
				}
			}
		}
	}

	void Archive::ParseFilesData(std::ifstream& in, bf::Header& header, bf::FileTable& fileTable, bf::ResourcesTable& resTable, std::vector<bf::File>& files) {
		for (uint4 i = 0; i < header.fcount; ++i) {
			in.clear();

			in.seekg((long)(fileTable.filePos[i] + 4));

			int grsId = fileTable.bridge_id_grs[i]; //if the resourceTable knows the actual size of this, get that
			uint4 size = grsId != -1 ? resTable.fileData_length[grsId] : files[i].size; //otherwise, fallback to the full block size

			files[i].data = new byte1[size];
			files[i].data_size = size;
			in.read((char*)(&files[i].data[0]), size);
		}
	}

	std::string Archive::FullFolderPath(int id) {
		if (id < 0 || id >= (int)Folders.size()) return "";

		std::string name = Folders[id].name;
		Folder* folder = &Folders[id];

		while (folder->parentFolder != -1) {
			folder = &Folders[folder->parentFolder];
			name = folder->name + std::string("/") + name;
		}

		return name;
	}

	void Archive::ExtractFileRaw(int id, const std::string& savePath) {
		if (id < 0 || id >= (int)Files.size()) return;
		
		std::ofstream writer(savePath + "/" + Files[id].name, std::ios::binary);

		if (!writer.is_open()) return;

		writer.write((char*)(&Files[id].data[0]), sizeof(byte1) * Files[id].data_size);

		writer.close();
	}

	void Archive::ExtractFolder(int id, const std::string& savePath) {
		if (id < 0 || id >= (int)Folders.size()) return;

		for (size_t i = 0; i < Folders.size(); ++i) {
			if (Folders[i].parentFolder == id) ExtractFolder(i, (savePath + "/" + std::string(Folders[id].name) + "/"));
		}

		for (size_t i = 0; i < Files.size(); ++i) {
			if (Files[i].folder == id) ExtractFileRaw(i, savePath);
		}
	}
}