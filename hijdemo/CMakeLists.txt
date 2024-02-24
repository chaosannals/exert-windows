cmake_minimum_required (VERSION 3.24)

set(HIJ_WINMM_DLL_OUT_DIR "" CACHE PATH "hij winmm output dir.")

project(hijdemo)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(winmm)
