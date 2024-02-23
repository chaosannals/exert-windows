cmake_minimum_required (VERSION 3.24)

project(hijdemo)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(winmm)
