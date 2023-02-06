cmake_minimum_required(VERSION 3.20)

option(VS_LINK_STATIC_MT "static link mt." ON)

include(FetchContent)

set(CMAKE_C_STANDARD_REQUIRED ON)

project(msys2do)

add_executable(
    msys2do
    WIN32
)

FetchContent_Declare(
  jsonc
  GIT_REPOSITORY git@github.com:douglascrockford/JSON-c.git
  GIT_TAG        bb3e8d18a33e8335619519e62462c666370e232d # 
)

FetchContent_GetProperties(jsonc) # 获取属性
if (NOT jsonc_POPULATED)
    message(STATUS "not jsonc populated.")
    FetchContent_Populate(jsonc)
    message(STATUS "jsonc src: ${jsonc_SOURCE_DIR}")
    message(STATUS "jsonc bin: ${jsonc_BINARY_DIR}")
    # 这个库只用到源码，所以只在这里引入源码
    target_include_directories(
        msys2do
        PUBLIC
        ${jsonc_SOURCE_DIR}
    )
    file(GLOB JSONC_SRC_FILES "${jsonc_SOURCE_DIR}/*.c")
    target_sources(
        msys2do
        PUBLIC
        ${JSONC_SRC_FILES}
    )
    set(MSYS2DO_CONF_PATH "${PROJECT_SOURCE_DIR}/conf.json")
    set(MSYS2DO_CONF_EXAMPLE_PATH "${PROJECT_SOURCE_DIR}/conf.example.json")
    set(MSYS2DO_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CONFIGURATION_TYPES}")
    if (EXISTS ${MSYS2DO_CONF_PATH})
        message(STATUS "conf.json is found, copy.")
        add_custom_command(
            OUTPUT "${MSYS2DO_OUTPUT_DIR}/conf.json"
            COMMAND copy /y ${MSYS2DO_CONF_PATH} "${MSYS2DO_OUTPUT_DIR}/conf.json"
        )
    else()
        message(STATUS "conf.json is not found, copy conf.example.json")
        add_custom_command(
            OUTPUT "${MSYS2DO_OUTPUT_DIR}/conf.json"
            COMMAND copy /y ${MSYS2DO_CONF_EXAMPLE_PATH} "${MSYS2DO_OUTPUT_DIR}/conf.json"
        )
    endif()
    #file(COPY ${MSYS2DO_CONF_PATH} DESTINATION ${MSYS2DO_OUTPUT_DIR})
    
else()
    message(STATUS "has jsonc populated.")
    # 起效，jsonc 这个不是 cmake 项目，这句无效
    FetchContent_MakeAvailable(jsonc)
endif()

set_target_properties(
    msys2do
    PROPERTIES
    C_STANDARD 11
)

target_sources(
    msys2do
    PRIVATE
    "src/main.c"
    "src/msys2.c"
)