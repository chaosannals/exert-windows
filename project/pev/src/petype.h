#pragma once
#include <cstdint>

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

struct pe_header_t {
	pe_machine_e machine;
	std::uint16_t sections_count; // 节数
	std::uint32_t create_timestamp; // 创建时间戳
	std::uint32_t symbol_table; //
	std::uint32_t symbols_count;
	std::uint16_t optional_header_size;
	pe_characteristic_e characteristics;
};

enum pe_optional_magic_e : std::uint16_t {
	nt_optional_32 = 0x10b,
	nt_optional_64 = 0x20b,
	rom_optional = 0x107,
};

struct pe_optional_header_t {
	
};

struct nt_header_t {
	char magic_number[4];
	pe_header_t pe_header;
};