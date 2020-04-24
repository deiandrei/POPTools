#include "defines.h"

#include "../popbin_lib/BinArchive.h"

#include <sstream>

int main(int argc, char** argv) {
	std::ifstream in1("test2.dec", std::ios::binary);
	
	int length = popbin::BinArchive::GetStreamSize(in1);

	char* buffer = new char[length];

	in1.read(buffer, length);
	in1.close();

	std::istringstream lol((std::string(buffer, length)), std::ios::binary);

	popbin::BinArchive* archive1 = new popbin::BinArchive(lol);
	
	
	std::ifstream in2("test.bin", std::ios::binary);
	popbin::BinArchive* archive2 = new popbin::BinArchive(in2);

	return 0;
}
