#include "defines.h"
#include "Header.h"
#include "FileTable.h"
#include "File.h"
#include "Folder.h"
#include "ResourcesTable.h"
#include "Utils.h"

int main(int argc, char** argv) {
	std::ifstream in("versiontag.bin", std::ios::in | std::ios::binary);
	char buf[32], buf1[32];
	long long a1, a2, a3, a4;
	in.read((char*)(&a1), sizeof(long long));
	in.read((char*)(&a2), sizeof(long long));
	in.read((char*)(&a3), sizeof(long long));
	in.read((char*)(&a4), sizeof(long long));
	in.seekg(in.tellg() + (std::streampos)128);
	in.read((char*)(&buf1[0]), sizeof(buf1));
	
	int x = 0;

	return 0;
}
