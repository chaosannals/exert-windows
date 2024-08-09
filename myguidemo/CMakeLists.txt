cmake_minimum_required(VERSION 3.24)

# 配置梯子
set(ENV{HTTP_PROXY} "http://127.0.0.1:10809") 
set(ENV{HTTPS_PROXY} "http://127.0.0.1:10809")

include("cmake/MyGUI.cmake")

# 开发环境太乱，不玩。
FetchMyGUI()

project(myguidemo)

add_subdirectory(simpledemo)
