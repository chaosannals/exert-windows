add_executable(ssc)

set(SSC_RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SCC_RES_TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}")

message(STATUS "scc res dir: ${SSC_RES_DIR} => ${SCC_RES_TARGET_DIR}")

target_sources(
	ssc
	PRIVATE
	"src/main.cpp"
    "src/lexeme.h"
    "src/error.h"
    "src/lexer.cpp"
    "src/x86asm.h"
    "src/coff.h"
    "src/parser.h"
 "src/parser.cpp" "src/symbol.h" "src/scope.h"  "src/type.h")

# 复制资源文件 res
file(
	COPY
	"${SSC_RES_DIR}"
	DESTINATION
	"${SCC_RES_TARGET_DIR}"
)
