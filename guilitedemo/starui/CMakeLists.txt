# 由于 GuiLite 命名变量不规范，使用了 windows 的一个 宏名 s_addr 做变量名，导致必须隔离一个库出来编译。
# 非要取个这名字膈应 Windows 端，也是没谁了。。


add_library(starui)

target_sources(
	starui
	PRIVATE
	"uicore.cpp"
)

set(STARUI_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(STARUI_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

message(STATUS "[starui] ${STARUI_SRC_DIR}")

target_include_directories(
    starui
	PRIVATE
	"${GUILITE_SRC_DIR}"
    PUBLIC
	"${STARUI_SRC_DIR}"
)
