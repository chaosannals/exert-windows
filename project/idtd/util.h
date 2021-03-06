#pragma once

template<typename T> class closer_t {
	const T handle;
public:
	closer_t(T handle) :handle(handle) {}
	closer_t(const closer_t&) = delete;
	~closer_t() { CloseHandle(handle); }
	operator T() { return handle; }
};