#pragma once
#include <cstdint>

#pragma pack(1)
struct dos_header_t {
	char magic_number[2];
	std::uint16_t last_page_bytes_count;
	std::uint16_t pages_count;
	std::uint16_t relocations;
	std::uint16_t paragraphs_header_size;
	std::uint16_t paragraphs_minimum_extra;
	std::uint16_t paragraphs_maximum_extra;
	std::uint16_t initial_ss;
	std::uint16_t initial_sp;
	std::uint16_t check_sum;
	std::uint16_t initial_ip;
	std::uint16_t initial_cs;
	std::uint16_t table_offset;
	std::uint16_t overlay_number;
	std::uint16_t reserved_words_1[4];
	std::uint16_t oem_identifier;
	std::uint16_t oem_information;
	std::uint16_t reserved_words_2[10];
	std::uint32_t new_header_offset;
};
#pragma pack()

enum pe_machine_e : std::uint16_t {
	i386 = 0x014c,
	r3000 = 0x0162,
	r4000 = 0x0166,
	r10000 = 0x0168,
	wcemipsv2 = 0x0169,
	alpha = 0x0184,
	sh3 = 0x01a2,
	sh3dsp = 0x01a3,
	sh3e = 0x01a4,
	sh4 = 0x01a6,
	sh5 = 0x01a8,
	arm = 0x01c0,
	thumb = 0x01c2,
	am33 = 0x01d3,
	powerpc = 0x01f0,
	powerpcfp = 0x01f1,
	ia64 = 0x0200,
	mips16 = 0x0266,
	alpha64 = 0x0284,
	mipsfpu = 0x0366,
	mipsfpu16 = 0x0466,
	axp64 = alpha64,
	tricore = 0x520,
	cef = 0x0cef,
	ebc = 0x0ebc,
	amd64 = 0x8664,
	m32r = 0x9041,
	cee = 0xc0ee,
};

enum pe_characteristic_e : std::uint16_t {
	relocs_stripped = 0x0001,
	executable_image = 0x0002,
	line_nums_stripped = 0x0004,
	local_syms_stripped = 0x0008,
	aggresive_ws_trim = 0x0010,
	large_address_aware = 0x0020,
	bytes_reversed_l0 = 0x0080,
	machine_32bit = 0x0100,
	debug_stripped = 0x0200,
	removable_run_from_swap = 0x0400,
	net_run_from_swap = 0x0800,
	sys = 0x1000,
	dll = 0x2000,
	up_system_only = 0x4000,
	bytes_reversed_hi = 0x8000,
};

#pragma pack(1)
struct pe_header_t {
	char magic_number[4];
	pe_machine_e machine;
	std::uint16_t sections_count; // 节数
	std::uint32_t create_timestamp; // 创建时间戳
	std::uint32_t symbol_table; //
	std::uint32_t symbols_count;
	std::uint16_t optional_header_size;
	pe_characteristic_e characteristics;
};
#pragma pack()

enum pe_optional_magic_e : std::uint16_t {
	nt_optional_32 = 0x10b, // 32bit
	nt_optional_64 = 0x20b, // 64bit
	rom_optional = 0x107, // ROM
};

enum pe_optional_subsystem_e : std::uint16_t {
	unknown = 0,
	native_or_driver = 1,
	gui = 2,
	cui = 3,
	posix = 7,
	ce = 9,
	efi_app = 10,
	efi_boot_driver = 11,
	efi_runtime_driver = 12,
	efi_rom = 13,
	xbox = 14,
};

enum pe_optional_dll_characteristic_e : std::uint16_t {
	loading_redirect = 7,
	force_check = 8,
	nx = 9,
	unuse_se = 11,
	wdm_driver = 14,
	can_use_console_server = 16,
};

#pragma pack(1)
struct pe_optional_header_32_t {
	pe_optional_magic_e magic;
	std::uint8_t linker_major_version;
	std::uint8_t linker_minor_version;
	std::uint32_t code_size;
	std::uint32_t data_init_size;
	std::uint32_t data_uninit_size;
	std::uint32_t entrypoint_rva; // 程序入口相对虚拟内存地址
	std::uint32_t code_rva; // 代码基址 RVA
	std::uint32_t data_rva; // 数据基址 RVA
	std::uint32_t image_rva; // 镜像基址 RVA
	std::uint32_t section_align; // 内存对齐
	std::uint32_t file_align; // 文件对齐
	std::uint16_t system_major_version;
	std::uint16_t system_minor_version;
	std::uint16_t image_major_version;
	std::uint16_t image_minor_version;
	std::uint16_t subsystem_major_version;
	std::uint16_t subsystem_minor_version;
	std::uint32_t win_version;
	std::uint32_t image_size;
	std::uint32_t headers_size;
	std::uint32_t checksum;
	pe_optional_subsystem_e subsystem;
	pe_optional_dll_characteristic_e dll_characteristics;
	std::uint32_t stack_reserve_size;
	std::uint32_t stack_commit_size;
	std::uint32_t heap_reserver_size;
	std::uint32_t heap_commit_size;
	std::uint32_t loader_flags;
	std::uint32_t number_of_rva_and_sizes;
};
#pragma pack()

#pragma pack(1)
struct pe_optional_header_64_t {
	pe_optional_magic_e magic;
	std::uint8_t linker_major_version;
	std::uint8_t linker_minor_version;
	std::uint32_t code_size;
	std::uint32_t data_init_size;
	std::uint32_t data_uninit_size;
	std::uint32_t entrypoint_rva; // 程序入口相对虚拟内存地址
	std::uint32_t code_rva; // 代码基址 RVA
	std::uint64_t image_rva; // 镜像基址 RVA
	std::uint32_t section_align; // 内存对齐
	std::uint32_t file_align; // 文件对齐
	std::uint16_t system_major_version;
	std::uint16_t system_minor_version;
	std::uint16_t image_major_version;
	std::uint16_t image_minor_version;
	std::uint16_t subsystem_major_version;
	std::uint16_t subsystem_minor_version;
	std::uint32_t win_version;
	std::uint32_t image_size;
	std::uint32_t headers_size;
	std::uint32_t checksum;
	pe_optional_subsystem_e subsystem;
	pe_optional_dll_characteristic_e dll_characteristics;
	std::uint64_t stack_reserve_size;
	std::uint64_t stack_commit_size;
	std::uint64_t heap_reserver_size;
	std::uint64_t heap_commit_size;
	std::uint32_t loader_flags;
	std::uint32_t data_directory_count; // 数据目录个数 固定 16 个
};
#pragma pack()

#pragma pack(1)
struct pe_data_directory_t {
	std::uint32_t rva;
	std::uint32_t size;
};
#pragma pack()

#pragma pack(1)
struct pe_data_directory_list_t {
	pe_data_directory_t export_table;
	pe_data_directory_t import_table;
	pe_data_directory_t resource_table;
	pe_data_directory_t exception_table;
	pe_data_directory_t certificate_table;
	pe_data_directory_t base_relocation_table;
	pe_data_directory_t debug;
	pe_data_directory_t architecture;
	pe_data_directory_t global_pointer;
	pe_data_directory_t tls_table;
	pe_data_directory_t load_config_table;
	pe_data_directory_t bound_import;
	pe_data_directory_t import_address_table;
	pe_data_directory_t delay_import_descriptor;
	pe_data_directory_t clr_runtime_header;
	pe_data_directory_t reserve;
};
#pragma pack()
