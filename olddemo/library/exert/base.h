#pragma once
#include <Windows.h>

namespace ext
{
	class handle_t
	{
		const HANDLE handle;

	public:
		handle_t(HANDLE handle) : handle(handle) {}
		handle_t(const handle_t &) = delete;
		~handle_t()
		{
			if (handle != nullptr)
			{
				CloseHandle(handle);
			}
		}
		operator HANDLE() { return handle; }
	};
}