add_library(wininet SHARED)

set_property(TARGET wininet PROPERTY CXX_STANDARD 20)

target_link_libraries(wininet detours)

target_sources(
    wininet
    PRIVATE
    "src/main.cpp"
    "src/wininet.def"
)

if(HIJ_WININET_DLL_OUT_DIR)
    # 当设置了输出目录时，把生成文件复制过去，方便调试。
    message(STATUS "hij wininet.dll out: ${HIJ_WININET_DLL_OUT_DIR}")
    add_custom_command(
        TARGET wininet
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy wininet.dll ${HIJ_WININET_DLL_OUT_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "install to ${HIJ_WININET_DLL_OUT_DIR}"
    )
endif()
