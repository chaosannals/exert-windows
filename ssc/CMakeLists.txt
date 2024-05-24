cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

project(simple_similar_c)

if (MSVC)
    # 静态链接 MT
    option(VS_LINK_STATIC_MT "static link mt." ON)

    if (VS_LINK_STATIC_MT)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

add_subdirectory(ssc)

