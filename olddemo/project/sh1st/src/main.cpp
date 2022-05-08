#include <iostream>
#include <fstream>
#include<filesystem>
#include<Windows.h>
#include <map>
#include <memory>
#include "pack.h"

int make_newpe(const std::string path) {
	std::ifstream reader(path, std::ios::binary);
	if (reader.is_open()) {
		reader.seekg(0, std::ios::end);
		auto length = reader.tellg();
		reader.seekg(0, std::ios::beg);
		std::unique_ptr<char[]> buffer(new char[length]);
		reader.read(buffer.get(), length);
		std::cout << "cekn length: " << length << std::endl;
		IMAGE_DOS_HEADER* dosh = reinterpret_cast<IMAGE_DOS_HEADER*>(buffer.get());
		std::cout << "dos magic:" << std::hex << dosh->e_magic << std::endl;
		if (dosh->e_magic != IMAGE_DOS_SIGNATURE) {
			std::cout << "不是有效的 DOS 头" << std::endl;
			return -1;
		}
		// 有 64位 和 32位的头部区分，这里类型定义由宏控制了。
		IMAGE_NT_HEADERS* nth = reinterpret_cast<IMAGE_NT_HEADERS*>(buffer.get() + dosh->e_lfanew);
		std::cout << "pe signature: " << nth->Signature << std::endl;
		if (nth->Signature != IMAGE_NT_SIGNATURE) {
			std::cout << "不是有效的 PE 头" << std::endl;
			return -2;
		}

		// 输入表
		DWORD iida = nth->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
		DWORD iids = nth->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
		DWORD iide = iida + iids;
		std::cout << "iid: " << iida << " : " << iids << std::endl;

		// 块信息
		IMAGE_SECTION_HEADER* iidsh = nullptr;
		std::map<int, DWORD> sections;
		IMAGE_SECTION_HEADER* sh = reinterpret_cast<IMAGE_SECTION_HEADER*>(buffer.get() + dosh->e_lfanew + sizeof(IMAGE_NT_HEADERS));
		for (int i = 0; i < nth->FileHeader.NumberOfSections; ++i) {
			std::cout << "section " << i << ": " << sh->Name << " size:" << sh->SizeOfRawData << " va: " << sh->VirtualAddress << std::endl;
			DWORD se = sh->VirtualAddress + sh->SizeOfRawData;
			if (iida >= sh->VirtualAddress && iida <= se) {
				std::cout << "  iid section: " << sh->Name << std::endl;
				if (iide > se) {
					std::cout << "  iid error" << std::endl;
				}
				else {
					iidsh = sh;
				}
			}
			++sh;
		}
		if (iidsh != nullptr) {
			// 导入表信息。
			DWORD iido = iidsh->PointerToRawData + (iida - iidsh->VirtualAddress);
			std::cout << "iido: " << iido << std::endl;
			IMAGE_IMPORT_DESCRIPTOR* iidp = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(buffer.get() + iido);
			while (iidp->Name != NULL) {
				char* np = reinterpret_cast<char*>(buffer.get() + (iidsh->PointerToRawData + (iidp->Name - iidsh->VirtualAddress)));
				std::cout << "import dll name: " << np << std::endl;
				++iidp;
			}
		}
		else {
			std::cout << "iidsh is null" << std::endl;
		}
	}
	return 0;
}

void apcompress(const std::string path) {
	std::ifstream reader(path, std::ios::binary);
	if (reader.is_open()) {
		reader.seekg(0, std::ios::end);
		auto length = reader.tellg();
		reader.seekg(0, std::ios::beg);
		std::unique_ptr<char[]> buffer(new char[length]);
		reader.read(buffer.get(), length);
		std::cout << std::dec << "cekn length: " << length << std::endl;

		auto d = pack(buffer.get(), length);
		std::cout << "packed size: " << d.get_length() << std::endl;
		auto ud = unpack(d.get_pointer(), d.get_length());
		std::cout << "unpacked size: " << ud.get_length() << std::endl;
		const char* udp = reinterpret_cast<char*>(ud.get_pointer());
		for (int i = 0; i < length; ++i) {
			if (buffer[i] != udp[i]) {
				std::cout << "compress error: " << std::endl;
				break;
			}
		}
		std::cout << "compress final." << std::endl;
	}
}


int main(int argc, char* argv[]) {
	auto dir = std::filesystem::current_path();
	auto path = dir / "cekng.exe";
	std::cout << "cekn: " << path << std::endl;
	make_newpe(path.string());
	apcompress(path.string());
	return 0;
}