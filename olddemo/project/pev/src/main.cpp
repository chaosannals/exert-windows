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

		std::cout << sizeof(dos_header_t) << " "
			<< sizeof(pe_header_t) << " "
			<< sizeof(pe_optional_header_32_t) << " "
			<< sizeof(pe_optional_header_64_t) << std::endl;

		dos_header_t* dosh = reinterpret_cast<dos_header_t*>(buffer);
		pe_header_t* peh = reinterpret_cast<pe_header_t*>(buffer + dosh->new_header_offset);
		pe_optional_header_64_t* peoh64 = reinterpret_cast<pe_optional_header_64_t*>(buffer + dosh->new_header_offset + sizeof(pe_header_t));
		pe_data_directory_list_t* peddl = reinterpret_cast<pe_data_directory_list_t*>(buffer + dosh->new_header_offset + sizeof(pe_header_t) + sizeof(pe_optional_header_64_t));
		std::cout << dosh->magic_number[0] << dosh->magic_number[1] << std::endl;
		std::cout << peh->magic_number[0] << peh->magic_number[1] << std::endl;
		std::cout << std::dec;
		std::cout << peh->sections_count << std::endl;
		std::cout << peh->optional_header_size << std::endl;
		std::cout << std::hex;
		std::cout << std::setw(8) << std::right << std::setfill('0') << peh->characteristics << std::endl;
		std::cout << std::setw(8) << std::right << std::setfill('0') << peoh64->magic << std::endl;
		std::cout << std::setw(8) << std::right << std::setfill('0') << peoh64->dll_characteristics << std::endl;
		std::cout << std::dec;
		std::cout << peoh64->subsystem << std::endl;
	}
	return 0;
}