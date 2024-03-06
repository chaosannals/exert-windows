cmake_minimum_required (VERSION 3.24)
include(FetchContent)

project(hijdemo)

# 配置
set(HIJ_WINMM_DLL_OUT_DIR "" CACHE PATH "hij winmm output dir.")
set(HIJ_WININET_DLL_OUT_DIR "" CACHE PATH "hij wininet output dir.")
set(HIJ_TARGET_WND_CLASS "button" CACHE STRING "hij target wnd class.")
set(HIJ_TARGET_WND_TITLE "" CACHE STRING "hij target wnd title.")

add_compile_definitions(
    HIJ_TARGET_WND_CLASS="${HIJ_TARGET_WND_CLASS}"
    HIJ_TARGET_WND_TITLE="${HIJ_TARGET_WND_TITLE}"
)

# 引入 Detours
FetchContent_Declare(
    detours
    GIT_REPOSITORY git@github.com:microsoft/Detours.git
    GIT_TAG 4b8c659f549b0ab21cf649377c7a84eb708f5e68
    GIT_SHALLOW ON # 浅克隆，git 参数加 --depth=1
    GIT_PROGRESS ON # 打印下载进度
)

FetchContent_GetProperties(detours)
#if (NOT detours_POPULATED)
    message(STATUS "populate detours")
    FetchContent_Populate(detours)

    include_directories(
        AFTER
        "${detours_SOURCE_DIR}/src"
    )
    message(STATUS "detours src: ${detours_SOURCE_DIR}")

    add_library(
        detours
        STATIC
    )
    file(GLOB DETOURS_SRC "${detours_SOURCE_DIR}/src/*.cpp")
    file(GLOB DETOURS_INC "${detours_SOURCE_DIR}/src/*.h")

    set_target_properties(
        detours
        PROPERTIES
        CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /Zi /MT /Gy /Gm- /Zl /Od"
    )

    list(FILTER DETOURS_SRC EXCLUDE REGEX "disol")
    # CMAKE_SIZEOF_VOID_P 需要在 project 之后，必须有项目
    message(STATUS "pointer size: ${CMAKE_SIZEOF_VOID_P}")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	    add_definitions(-D_AMD64_)
        message(STATUS "disolx64")
        list(APPEND DETOURS_SRC "${detours_SOURCE_DIR}/src/disolx64.cpp")
    else()
	    add_definitions(-D_X86_)
        message(STATUS "disolx86")
        list(APPEND DETOURS_SRC "${detours_SOURCE_DIR}/src/disolx86.cpp")
    endif()

    set_property (SOURCE "${detours_SOURCE_DIR}/src/uimports.cpp" APPEND PROPERTY HEADER_FILE_ONLY true)

    message(STATUS "detours cpp: ${DETOURS_SRC}")

    target_sources(
        detours
        PRIVATE
        ${DETOURS_SRC}
    )

    target_include_directories(
        detours
        PUBLIC
        "${detours_SOURCE_DIR}/src"
    )
#endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

FetchContent_Declare(
    imgui
    GIT_REPOSITORY git@github.com:ocornut/imgui.git
    GIT_TAG v1.90.4
    GIT_SHALLOW ON
    GIT_PROGRESS ON
)

FetchContent_GetProperties(imgui)

if (NOT imgui_POPULATED)
    message(STATUS "populate imgui")
    FetchContent_Populate(imgui)
    message(STATUS "imgui src: ${imgui_SOURCE_DIR}")

    add_library(imgui STATIC)
    set_target_properties(imgui PROPERTIES CXX_STANDARD 20)

    file(GLOB IMGUI_SRC "${imgui_SOURCE_DIR}/*.cpp")

    
    set(
        IMGUI_BACKEND_SRC
        "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.cpp"
    )
    set(IMGUI_BACKEND_LIBS "d3d9.lib")

    message(STATUS "imgui cpp: ${IMGUI_BACKEND_SRC} ${IMGUI_SRC}")
    target_sources(
        imgui
        PRIVATE
        ${IMGUI_SRC}
        ${IMGUI_BACKEND_SRC}
    )

    message(STATUS "imgui libs: ${IMGUI_BACKEND_LIBS}")
    target_link_libraries(imgui ${IMGUI_BACKEND_LIBS})

    target_include_directories(
        imgui
        PUBLIC
        "${imgui_SOURCE_DIR}"
    )
endif()

add_subdirectory(cd3d9)
add_subdirectory(winmm)
add_subdirectory(wininet)
