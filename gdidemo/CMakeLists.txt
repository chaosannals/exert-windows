cmake_minimum_required(VERSION 3.24)

# 配置梯子
set(ENV{HTTP_PROXY} "http://127.0.0.1:10809") 
set(ENV{HTTPS_PROXY} "http://127.0.0.1:10809")

include("cmake/RxCpp.cmake")

FetchRxCpp()

project(gdidemo)

add_subdirectory(simpledemo)
add_subdirectory(composedemo)
add_subdirectory(cpprxdemo)
add_subdirectory(rxdemo)
add_subdirectory(asyncdemo)
