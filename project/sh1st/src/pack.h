#pragma once
#include <memory>
#include <aplib.h>

class data_t {
	std::size_t length;
	char* pointer;
public:
	data_t(char* p, std::size_t l):pointer(p), length(l) {}
	~data_t() {
		if (pointer) delete[] pointer;
	}
	void* get_pointer()const {
		return pointer;
	}
	std::size_t get_length() const {
		return length;
	}
};

data_t pack(void* buffer, std::size_t length) {
	auto mps = aP_max_packed_size(length);
	auto ws = aP_workmem_size(length);
	std::unique_ptr<char[]> target(new char[mps]);
	std::unique_ptr<char[]> workmem(new char[ws]);
	auto ts = aPsafe_pack(buffer, target.get(), length, workmem.get(), nullptr, nullptr);
	return data_t(target.release(), ts);
}

data_t unpack(void *buffer, std::size_t length) {
	auto os = aPsafe_get_orig_size(buffer);
	std::unique_ptr<char[]> target(new char[os]);
	auto ws = aPsafe_depack(buffer, length, target.get(), os);
	return data_t(target.release(), os);
}