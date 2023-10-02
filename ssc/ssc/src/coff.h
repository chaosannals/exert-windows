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
        image_scn_lnk_info=0x00000200, // 此节包含注释或其他信息。.drectve 节具有这种属性，此标记只对目标文件合法。
        image_scn_lnk_remove=0x00000800, // 此节不会成为最终形成的映像文件的一部分，此标志仅对目标文件合法。
        image_scn_lnk_comdat=0x00001000, // 此节包含 comdat 数据，此标志仅对目标文件合法。
        image_scn_gprel=0x00008000, // 此节包含通过全局指针（GP）引用的数据。
        image_scn_mem_purgeable=0x00020000, // 保留
        image_scn_mem_16bit=0x00020000,//保留
        image_scn_mem_locked=0x00040000, // 保留
        image_scn_mem_preload=0x00080000, // 保留
        image_scn_align_1bytes=0x00100000, // 按1字节对齐，此标志只对目标文件合法。
        image_scn_align_2bytes = 0x00200000, // 按2字节对齐，此标志只对目标文件合法。
        image_scn_align_4bytes = 0x00300000, // 按4字节对齐，此标志只对目标文件合法。
        image_scn_align_8bytes = 0x00400000, // 按8字节对齐，此标志只对目标文件合法。
        image_scn_align_16bytes = 0x00500000, // 按16字节对齐，此标志只对目标文件合法。
        image_scn_align_32bytes = 0x00600000, // 按32字节对齐，此标志只对目标文件合法。
        image_scn_align_64bytes = 0x00700000, // 按64字节对齐，此标志只对目标文件合法。
        image_scn_align_128bytes = 0x00800000, // 按128字节对齐，此标志只对目标文件合法。
        image_scn_align_256bytes = 0x00900000, // 按256字节对齐，此标志只对目标文件合法。
        image_scn_align_512bytes = 0x00A00000, // 按512字节对齐，此标志只对目标文件合法。
        image_scn_align_1024bytes = 0x00B00000, // 按1024字节对齐，此标志只对目标文件合法。
        image_scn_align_2048bytes = 0x00C00000, // 按2048字节对齐，此标志只对目标文件合法。
        image_scn_align_4096bytes = 0x00D00000, // 按4096字节对齐，此标志只对目标文件合法。
        image_scn_align_8192bytes = 0x00E00000, // 按8192字节对齐，此标志只对目标文件合法。
        image_scn_lnk_nreloc_ovfl=0x01000000, // 此节包含扩展的重定位信息
        image_scn_mem_discardable=0x02000000, // 此节可以在需要时候丢弃
        image_scn_mem_not_cached=0x04000000, // 此节不能被缓存
        image_scn_mem_not_paged=0x08000000, // 此节不能被交换到页面文件中
        image_scn_mem_shared=0x10000000, // 此节可以在内存中共享
        image_scn_mem_execute=0x20000000, // 此节可以作为代码执行
        image_scn_mem_read=0x40000000, // 此节可读
        image_scn_mem_write=0x80000000, // 此节可写
    };


#pragma pack(1)
    // COFF 节头
    // IMAGE_SECTION_HEADER
    // 特殊节
    // .bss 未初始化的数据（自由格式） image_scn_cnt_uninitialized_data | image_scn_mem_read | image_scn_mem_write
    // .cormeta CLR 元数据，目标文件包含托管代码 image_scn_lnk_info
    // .data 已初始化的数据（自由格式） image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_write
    // .debug$F 生成的FPO 调试信息（仅适合目标文件，仅用于 x86 平台，已弃用）image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_discardable
    // .debug$P 预编译的调试类型信息（仅适用于目标文件）image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_discardable
    // .debug$S 调试符号信息（仅适用于目标文件）image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_discardable
    // .debug$T 调试类型信息（仅适用于目标文件）image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_discardable
    // .drective 链接器选项 image_scn_lnk_info
    // .edata 导出表 image_scn_cnt_initialized_data | image_scn_mem_read
    // .idata 导入表 image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_write
    // .idlsym 包含已注册的 SEH（仅使用于映像文件），用以支持 IDL 属性。 image_scn_lnk_info
    // .pdata 异常信息 image_scn_cnt_initialized_data | image_scn_mem_read
    // .rdata 只读的已初始化数据 image_scn_cnt_initialized_data | image_scn_mem_read
    // .reloc 映像文件的重定位信息 image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_discardable
    // .rsrc 资源目录 image_scn_cnt_initialized_data | image_scn_mem_read
    // .sbss 与 GP 相关的未初始化数据（自由格式） image_scn_cnt_uninitialized_data | image_scn_mem_read | image_scn_mem_write | image_scn_gprel ，其中 image_scn_gprel 只在 IA64 使用。此标志只能用于目标文件。
    // .sdata 与 GP 相关的已初始化数据（自由格式） image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_write | image_scn_gprel ，其中 image_scn_gprel 只在 IA64 使用。此标志只能用于目标文件。
    // .srdata 与 GP 相关的只读数据（自由格式） image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_gprel ，其中 image_scn_gprel 只在 IA64 使用。此标志只能用于目标文件。
    // .sxdata 已注册的异常处理程序数据（自由格式，仅目标文件，仅适用 x86） image_scn_lnk_info
    // .text 可执行代码（自由格式） image_scn_cnt_code | image_scn_mem_execute | image_scn_mem_read
    // .tls 线程局部存储（仅适用目标格式） image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_write
    // .tls$ 线程局部存储（仅适用目标格式） image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_write
    // .vsdata 与 GP 相关的已初始化数据（自由格式，仅适用 ARM、SH4 和 Thumb ）image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_write
    // .xdata 异常信息（自由格式） image_scn_cnt_initialized_data | image_scn_mem_read | image_scn_mem_write
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
