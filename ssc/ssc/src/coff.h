#pragma once

#include <cstdint>

namespace ssc {
    // 和 PE 那边时一致的。
    enum coff_machine_kind : std::uint16_t {
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

    // 和 PE 那边时一致的。
    enum coff_characteristic : std::uint16_t {
        relocs_stripped = 0x0001, // 仅适用映像文件，此文件不包含基地址重定位信息，因此必须被加载到首选基地址上。
        executable_image = 0x0002, // 仅使用映像文件，可被运行。
        line_nums_stripped = 0x0004, // COFF 行号信息，被遗弃，该位应为 0。
        local_syms_stripped = 0x0008, // COFF 符号表相关，被遗弃，该位应为 0。
        aggresive_ws_trim = 0x0010, // Windows 2000 后，被遗弃，该位应为 0。
        large_address_aware = 0x0020, // 允许地址大于 2G
        // 0x0040 保留标志位
        bytes_reversed_l0 = 0x0080, // 小头 ，不赞成使用，该位应为 0。
        machine_32bit = 0x0100, // 32bit 
        debug_stripped = 0x0200, // 调试信息已经移除。
        removable_run_from_swap = 0x0400, // 如果映像文件在移动介质上，完全加载它并复制到交换文件中。
        net_run_from_swap = 0x0800,// 如果映像文件在网络介质上，完全加载它并复制到交换文件中。
        sys = 0x1000, // 系统文件
        dll = 0x2000, // 是 DLL
        up_system_only = 0x4000, // 仅能运行于单处理器机器上
        bytes_reversed_hi = 0x8000, // 大头，不赞成使用此标志。
    };

#pragma pack(1)
    // 只比 PE 头少了 魔数 PE\0\0
    // IMAGE_FILE_HEADER
    struct coff_header {
        coff_machine_kind machine; // 机器类型
        std::uint16_t number_of_sections; // 节数目
        std::uint32_t time_date_stamp; // UTC 1970-01-01 起时间戳
        std::uint32_t pointer_to_symbol_table; // 符号表文件偏移，没有时为 0
        std::uint32_t number_of_symbols; // 符号表元素数，字符串表紧跟符号表，用该字段可算出字符串表头，映射文件此值为0
        std::uint16_t size_of_optional_header; // 可选头的大小，可执行文件需要可选头，目标文件不需要，目标文件此值为0
        coff_characteristic characteristics; // 文件属性标志
    };
#pragma pack()
    // COFF 节特征信息标志
    enum coff_section_characteristics : std::uint32_t {
        // 0x00000000 - 0x00000004 保留位
        image_scn_type_no_pad = 0x00000008, // 不能填充，被弃用，被 image_scn_align_1bytes 取代。
        // 0x00000010 保留
        image_scn_cnt_code=0x00000020, // 此节包含可执行代码
        image_scn_cnt_initialized=0x00000040, // 此节包含初始化数据
        image_scn_cnt_uninitialized_data=0x00000080, // 此节包含未初始化数据
        image_scn_lnk_other=0x00000100, // 保留


    };


#pragma pack(1)
    // COFF 节头
    // IMAGE_SECTION_HEADER
    struct coff_section_header {
        char name[8]; // UTF-8 8字节长节名，空位 \0 填充
        std::uint32_t virtual_size; // 可执行文件加载内存时节总大小，如果比 size_of_raw_data 大，多余部分填充 \0 ,对于目标文件应为 0。
        std::uint32_t virtual_address; // 可执行文件时，为该节加载内存基址的偏移地址。对于目标文件为没有重定位前第一个字节的地址，编译器应设该值为0，方便。
        std::uint32_t size_of_raw_data; // 目标文件表示节大小。可执行文件该值必须对齐，所以是可选文件头中 file_alignment 倍数。当节中仅包含未初始化数据时，该值为0。
        std::uint32_t pointer_of_raw_data; // COFF 文件中第一个页面的文件指针。可执行文件必须是  file_alignment 倍数。目标文件最好是 4 字节对齐。当节中仅包含未初始化数据时，该值为0。
        std::uint32_t pointer_of_relocations; // 重定位开头文件指针。可执行文件和没有重定位项该值应为0。
        std::uint32_t pointer_of_line_numbers; // 节行号开头文件指针。如果没有行号信息此值为0。对于映像文件应为0，不赞成使用 COFF 调试信息。
        std::uint16_t number_of_relocations; // 节重定位项个数。可执行文件该值应为0。
        std::uint16_t number_of_line_numbers; // 节行号项个数。对于映像文件应为0，不赞成使用 COFF 调试信息。
        coff_section_characteristics characteristics; // 节特征标志
    };
#pragma pack()
}
