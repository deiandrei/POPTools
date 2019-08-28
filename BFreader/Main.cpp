#include "defines.h"
#include "BFHeader.h"
#include "FileIdOffsetTable.h"
#include "FileEntry.h"
#include "FolderEntry.h"
#include "SizeGrsTable.h"

void LinkIds(SizeGrsTable& grsTable, FileIdOffsetTable& filePosId, BFHeader& header) {
	std::cout << "Linking FileEntry and size.grs table file Ids...";

	for (int i = 0; i < grsTable.filesCount; ++i) {
		for (int j = 0; j < header.fcount; ++j) {
			if (grsTable.fileId[i] == filePosId.fileId[j]) {
				grsTable.id_bf[i] = j;
				filePosId.id_grs[j] = i;
			}
		}
	}

	std::cout << "done!\n";
}

std::string GetFullFolderPath(std::vector<FolderEntry>& folderEntries, FolderEntry folder) {
	std::string name = folder.name;

	while (folder.parentFolder != -1) {
		folder = folderEntries[folder.parentFolder];
		name = folder.name + std::string("/") + name;
	}

	return name;
}

void ParseFileData(BFHeader& header, FileIdOffsetTable& fileIdOffsetTable, SizeGrsTable& grsTable, std::vector<FileEntry>& files, int id) {
	std::stringstream in;
	in.write((char*)(&files[id].data), sizeof(byte1) * files[id].size);

	uint4 magic = 4009738393; // 0x99C0FFEE

	in.seekg((std::streampos)0);
	uint4 dec_size; in.read((char*)(&dec_size), sizeof(uint4));
	uint4 enc_size; in.read((char*)(&enc_size), sizeof(uint4));
	in.seekg((std::streampos)13);
	uint4 file_magic; in.read((char*)(&file_magic), sizeof(uint4));
	in.seekg((std::streampos)14);
	uint4 file_magic2; in.read((char*)(&file_magic2), sizeof(uint4));

	if ((dec_size != enc_size && file_magic == magic) || (dec_size != enc_size && file_magic2 == magic)) {
		in.seekg((std::streampos)0);

		int size_dec;

		do {
			in.read((char*)(&size_dec), sizeof(int));
			int size_enc; in.read((char*)(&size_enc), sizeof(int));
			byte1* source = new byte1[size_enc];
			in.read((char*)(&source), sizeof(byte1) * size_enc);
			delete source;
		} while (size_dec == 131072);

		files[id].fileData_length = (int)in.tellg();
	}
	else {
		in.seekg((std::streampos)12);
		in.read((char*)(&file_magic), sizeof(int));

		//for non-compressed wol files
		if (dec_size == enc_size && file_magic && magic) {
			in.seekg((std::streampos)4);
			in.read((char*)(&files[id].fileData_length), sizeof(int));
			files[id].fileData_length += 12;
		}
		//for the other file types
		else {
			in.seekg((std::streampos)0);

			byte1* buf = new byte1[files[id].size];
			in.read((char*)(&buf), sizeof(byte1) * files[id].size);

			for (int j = files[id].size - 1; j > 0; j--) {
				if (buf[j] != 0) {
					if (files[id].folder == 1) files[id].fileData_length = j + 7;
					else if (files[id].folder == 3) {
						files[id].fileData_length = j + 4;

						//workaround for this mismatch stupidity
						//wtf dude tho
						if (std::string(files[id].name) == "ps2.ico") {
							files[id].fileData_length = (int)grsTable.fileData_length[fileIdOffsetTable.id_grs[id]];
						}
					}
					else if (files[id].folder == 0 || files[id].folder == 2 || files[id].folder == 4) files[id].fileData_length = j + 1;

					j = 0;
				}
			}
		}
	}
}

void ParseFilesData(std::ifstream& in, BFHeader& header, FileIdOffsetTable& fileIdOffsetTable, SizeGrsTable& grsTable, std::vector<FileEntry>& files) {
	std::cout << "Parsing files data length...";

	for (uint4 i = 0; i < header.fcount; ++i) {
		in.seekg((std::streampos)(fileIdOffsetTable.filePos[i] + 4));
		byte1* data = new byte1[files[i].size];
		in.read((char*)(&data), sizeof(byte1) * files[i].size);

		files[i].data = data;
		ParseFileData(header, fileIdOffsetTable, grsTable, files, i);
	}
	std::cout << "done!\n";
}

int main(int argc, char** argv) {
	std::ifstream in("test.bf", std::ios::binary);

	int headerTotalLength, folderEntryUnkOffset, folderEntryUnk;

	std::cout << sizeof(byte1);

	BFHeader header;
	header.ReadFromFile(in);

	FileIdOffsetTable table;
	table.ReadFromFile(in, header);

	std::vector<FileEntry> files = ReadFileEntriesFromFile(in, header);
	
	std::vector<FolderEntry> folders = ReadFolderEntriesFromFile(in, header, folderEntryUnkOffset, folderEntryUnk, headerTotalLength);

	SizeGrsTable sizeGrs;
	sizeGrs.ReadFromFile(in, header, table, files);

	LinkIds(sizeGrs, table, header);
	ParseFilesData(in, header, table, sizeGrs, files);

	std::string folderName = GetFullFolderPath(folders, folders[4]);

	return 0;
}
