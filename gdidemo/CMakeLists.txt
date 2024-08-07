cmake_minimum_required(VERSION 3.24)

include("cmake/RxCpp.cmake")

FetchRxCpp()

project(gdidemo)

add_subdirectory(simpledemo)
add_subdirectory(composedemo)
add_subdirectory(cpprxdemo)
