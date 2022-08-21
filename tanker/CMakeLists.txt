cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)

project(tanker LANGUAGES CXX)

if (MSVC)
    # 静态链接 MT
    option(VS_LINK_STATIC_MT "static link mt." OFF)

    if (VS_LINK_STATIC_MT)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

add_executable(tanker WIN32)

target_sources(
    tanker
    PRIVATE
    "./src/main.cpp"
    "./src/window.cpp"
)