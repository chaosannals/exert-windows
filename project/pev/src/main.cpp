#include<iostream>
#include<filesystem>
#include<fstream>

int main(int argc, char *argv[]) {
	std::cout << std::filesystem::current_path() << std::endl;
	
	std::system("cekn");
	return 0;
}