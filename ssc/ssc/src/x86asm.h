#pragma once

#include <vector>

namespace ssc {
    enum x86_reg {
        reg_eax=0,
        reg_ecx=1,
        reg_edx=2,
        reg_ebx=3,
        reg_esp=4,
        reg_ebp=5,
        reg_esi=6,
        reg_edi=7,
        reg_any,
    };

    class x86operand {

    };

    // 指令 或 标签
    class x86row {

    };

	class x86asm {
        std::vector<x86row> instructions;
    public:

	};
}
