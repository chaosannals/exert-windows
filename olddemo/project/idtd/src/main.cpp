#include <Windows.h>
#include <cstddef>
#include <exert/base.h>
#include <exert/exploit/runtime.h>
#include <exert/exploit/process.h>
#include <exert/exploit/filesystem.h>

int WINAPI WinMain(HINSTANCE self, HINSTANCE prev, LPSTR argv, int argc)
{
	ext::init_heap();
	ext::file_put(L"idtd.log", L"Hello");
	ext::vector_t<ext::process_t> processes = ext::list_processes();
	for (std::size_t i = 0; i <= processes.get_length(); ++i)
	{
		ext::file_putln(L"ppps.log", L"1");
	}
	for (ext::process_t &p : processes)
	{
		ext::file_putln(L"ps.log", p.filename);
	}
	return 0;
}