add_library(winmm SHARED)

set_property(TARGET winmm PROPERTY CXX_STANDARD 20)

target_link_libraries(winmm detours)

target_sources(
    winmm
    PRIVATE
    src/main.cpp
    src/api.def
)

if(HIJ_WINMM_DLL_OUT_DIR)
    # 当设置了输出目录时，把生成文件复制过去，方便调试。
    message(STATUS "hij winmm.dll out: ${HIJ_WINMM_DLL_OUT_DIR}")
    add_custom_command(
        TARGET winmm
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy winmm.dll ${HIJ_WINMM_DLL_OUT_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "install to ${HIJ_WINMM_DLL_OUT_DIR}"
    )
endif()
