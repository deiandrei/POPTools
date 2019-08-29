#include "defines.h"
#include "Header.h"
#include "FileTable.h"
#include "File.h"
#include "Folder.h"
#include "ResourcesTable.h"

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
	std::ifstream in("test.bf", std::ios::in | std::ios::binary);

	std::cout << "Parsing files data length...";
	for (uint4 i = 0; i < header.fcount; ++i) {
		in.clear();

		in.seekg(fileIdOffsetTable.filePos[i]);

		byte1* data = new byte1[files[i].size];
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

int main(int argc, char** argv) {
	std::ifstream in("test.bf", std::ios::in | std::ios::binary);

	int headerTotalLength, folderEntryUnkOffset, folderEntryUnk;

	std::cout << sizeof(byte1);

	bf::Header header;
	header.ReadFromFile(in);

	bf::FileTable table;
	table.ReadFromFile(in, header);

	std::vector<bf::File> files = ReadFileEntriesFromFile(in, header);
	
	std::vector<bf::Folder> folders = ReadFolderEntriesFromFile(in, header, folderEntryUnkOffset, folderEntryUnk, headerTotalLength);

	bf::ResourcesTable sizeGrs;
	sizeGrs.ReadFromFile(in, header, table, files);

	LinkIds(sizeGrs, table, header);

	for (uint4 i = 0; i < table.bridge_id_grs.size(); ++i) {
		if (table.bridge_id_grs[i] == -1) {
			std::cout << "File without grs: " << GetFullFolderPath(folders, folders[files[i].folder]) << "/" << files[i].name << "\n";
		}
	}

	in.close();

	ParseFilesData(header, table, sizeGrs, files);

	SaveFile(files[2]);
	SaveFile(files[8]);
	SaveFile(files[9]);

	return 0;
}
