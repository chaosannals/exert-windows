#include<iostream>
#include<filesystem>

int main(int argc, char* argv[]) {
	std::cout << std::filesystem::current_path() << std::endl;
	return 0;
}