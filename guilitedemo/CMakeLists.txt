cmake_minimum_required(VERSION 3.24)

include("cmake/GuiLite.cmake")

set(CMAKE_CXX_STANDARD 14)

FetchGuiLite()

project(guilitedemo)

add_subdirectory(starui)
add_subdirectory(star)
