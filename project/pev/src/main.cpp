#include<iostream>
#include<filesystem>
#include<fstream>
#include "petype.h"

int main(int argc, char *argv[]) {
	std::cout << std::filesystem::current_path() << std::endl;
	
	std::ifstream ceknf("cekn.exe", std::ifstream::binary);
	if (ceknf) {
		ceknf.seekg(0, ceknf.end);
		std::streamoff length = ceknf.tellg();
		std::cout << length << std::endl;
		ceknf.seekg(0, ceknf.beg);
		char* buffer = new char[length];
		ceknf.read(buffer, length);
		dos_header_t* dosh = reinterpret_cast<dos_header_t*>(buffer);
		nt_header_t* nth = reinterpret_cast<nt_header_t*>(buffer + dosh->new_header_offset);
		std::cout << dosh->new_header_offset << std::endl;
		std::cout << nth->magic_number[0] << nth->magic_number[1] << std::endl;
		std::cout << nth->pe_header.characteristics << std::endl;
	}
	return 0;
}