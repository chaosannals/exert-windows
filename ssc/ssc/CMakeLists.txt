add_executable(ssc)

set(SSC_RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")

target_sources(
	ssc
	PRIVATE
	src/main.cpp
 "src/lexeme.h" "src/error.h" "src/lexer.cpp" "src/x86asm.h" "src/coff.h" "src/parser.h")

file(
	COPY
	"${SSC_RES_DIR}"
	DESTINATION
	"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CONFIGURATION_TYPES}"
)